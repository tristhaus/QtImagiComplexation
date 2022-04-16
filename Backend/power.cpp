/*
 * This file is part of QtImagiComplexation.
 *
 * QtImagiComplexation is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * QtImagiComplexation is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with QtImagiComplexation.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

#include "power.h"
#include <cfenv>
#include <cmath>
#include <utility>

namespace Backend
{
    Power::Power(std::shared_ptr<Expression> base, std::shared_ptr<Expression> exponent)
        : base(std::move(base)),
          exponent(std::move(exponent))
    {
    }

    Power::~Power()
    {
        // paranoid: remove possible source for circular references
        base.reset();
        exponent.reset();
    }

    int Power::GetLevel() const
    {
        return 3;
    }

    bool Power::IsMonadic() const
    {
        return false;
    }

    std::optional<complex> Power::Evaluate(complex input) const
    {
        auto baseResult = base->Evaluate(input);
        auto exponentResult = exponent->Evaluate(input);

        if (!baseResult.has_value() || !exponentResult.has_value())
        {
            return {};
        }

        std::feclearexcept(FE_ALL_EXCEPT);
        auto retval = std::pow(baseResult.value(), exponentResult.value());

        if (!(std::isfinite(retval.real()) || std::isfinite(retval.imag())) || (std::fetestexcept(FE_DIVBYZERO | FE_OVERFLOW | FE_INVALID) != 0))
        {
            return {};
            std::feclearexcept(FE_ALL_EXCEPT);
        }

        return retval;
    }

    bool Power::operator==(const Expression& other) const
    {
        if (const auto * b = dynamic_cast<const Power*>(&other))
        {
            if(b == nullptr)
            {
                return false;
            }

            return *(this->base) == *(b->base) && *(this->exponent) == *(b->exponent);
        }
        else
        {
            return false;
        }
    }

    bool Power::operator!=(const Expression& other) const
    {
        return !(*this == other);
    }

}
