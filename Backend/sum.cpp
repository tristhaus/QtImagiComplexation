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

#include "sum.h"

#include <utility>

namespace Backend
{
    Sum::Sum(std::vector<Summand> summands)
        : summands(std::move(summands))
    {
    }

    Sum::~Sum()
    {
        // paranoid: remove possible source for circular references
        summands.clear();
    }

    int Sum::GetLevel() const
    {
        return 1;
    }

    bool Sum::IsMonadic() const
    {
        return false;
    }

    std::optional<complex> Sum::Evaluate(complex input) const
    {
        complex retval(0.0);

        auto expressionIterator = summands.begin();
        auto expressionEnd = summands.end();

        for(;expressionIterator != expressionEnd; ++expressionIterator)
        {
            auto subResult = (*expressionIterator).expression->Evaluate(input);

            if(!subResult.has_value())
            {
                return {};
            }

            switch ((*expressionIterator).sign)
            {
            case Sum::Sign::Plus:
                retval += subResult.value();
                break;
            case Sum::Sign::Minus:
                retval -= subResult.value();
                break;
            default:
                throw std::exception(u8"programming mistake in Sum switch");
            }
        }

        return retval;
    }

    bool Sum::operator==(const Expression &other) const
    {
        if (const auto * b = dynamic_cast<const Sum*>(&other))
        {
            if(b == nullptr)
            {
                return false;
            }

            auto thisTemp = std::vector<Summand>(this->summands);
            auto otherTemp = std::vector<Summand>(b->summands);

            while(!thisTemp.empty()) {

                bool matched = false;

                auto thisSummand = thisTemp.back();

                auto otherIterator = otherTemp.begin();
                auto otherEnd = otherTemp.end();

                while(otherIterator != otherEnd)
                {
                    if(*otherIterator == thisSummand)
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

    bool Sum::operator!=(const Expression &other) const
    {
        return !(*this == other);
    }

    Sum::Summand::Summand(Sum::Sign sign, std::shared_ptr<Expression> expression)
        : sign(sign), expression(std::move(expression))
    {
    }

    bool Sum::Summand::operator==(const Sum::Summand& other) const
    {
        return this->sign == other.sign && *(this->expression) == *(other.expression);
    }

    bool Sum::Summand::operator!=(const Sum::Summand& other) const
    {
        return !(*this == other);
    }
}
