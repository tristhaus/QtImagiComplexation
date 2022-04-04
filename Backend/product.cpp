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

#include "product.h"
#include <cfenv>
#include <cmath>
#include <utility>

namespace Backend
{
    Product::Product(std::vector<Factor> factors)
        : factors(std::move(factors))
    {
    }

    Product::~Product()
    {
        // paranoid: remove possible source for circular references
        factors.clear();
    }

    int Product::GetLevel() const
    {
        return 2;
    }

    bool Product::IsMonadic() const
    {
        return false;
    }

    std::optional<complex> Product::Evaluate(complex input) const
    {
        complex retval(1.0);

        auto expressionIterator = factors.begin();
        auto expressionEnd = factors.end();

        for(;expressionIterator != expressionEnd; ++expressionIterator)
        {
            auto subResult = (*expressionIterator).expression->Evaluate(input);

            if(!subResult.has_value())
            {
                return {};
            }

            auto value = subResult.value();

            switch ((*expressionIterator).exponent)
            {
            case Product::Exponent::Positive:
                retval *= value;
                break;
            case Product::Exponent::Negative:

                if(std::fabs(value.real()) < this->epsilon && std::fabs(value.imag()) < this->epsilon)
                {
                    return {};
                }

                std::feclearexcept(FE_ALL_EXCEPT); //NOLINT(hicpp-signed-bitwise)
                retval /= value;

                if(std::fetestexcept(FE_DIVBYZERO | FE_OVERFLOW) != 0) //NOLINT(hicpp-signed-bitwise)
                {
                    return {};
                    std::feclearexcept(FE_ALL_EXCEPT); //NOLINT(hicpp-signed-bitwise)
                }

                break;
            default:
                throw std::exception("programming mistake in Product switch");
            }
        }

        return retval;
    }

    bool Product::operator==(const Expression &other) const
    {
        if (const auto * b = dynamic_cast<const Product*>(&other))
        {
            if(b == nullptr)
            {
                return false;
            }

            auto thisTemp = std::vector<Factor>(this->factors);
            auto otherTemp = std::vector<Factor>(b->factors);

            while(!thisTemp.empty()) {

                bool matched = false;

                auto thisFactor = thisTemp.back();

                auto otherIterator = otherTemp.begin();
                auto otherEnd = otherTemp.end();

                while(otherIterator != otherEnd)
                {
                    if(*otherIterator == thisFactor)
                    {
                        matched = true;
                        break;
                    }

                    ++otherIterator;
                }

                if(matched)
                {
                    thisTemp.pop_back();
                    otherTemp.erase(otherIterator);
                }
                else
                {
                    return false;
                }
            }

            return otherTemp.empty();
        }
        else
        {
            return false;
        }
    }

    bool Product::operator!=(const Expression &other) const
    {
        return !(*this == other);
    }

    Product::Factor::Factor(Product::Exponent exponent, std::shared_ptr<Expression> expression)
        : exponent(exponent), expression(std::move(expression))
    {
    }

    bool Product::Factor::operator==(const Product::Factor& other) const
    {
        return this->exponent == other.exponent && *(this->expression) == *(other.expression);
    }

    bool Product::Factor::operator!=(const Product::Factor& other) const
    {
        return !(*this == other);
    }
}
