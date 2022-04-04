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

#include "constant.h"

namespace Backend {

    Constant::Constant(double input) : value(input)
    {
    }

    int Constant::GetLevel() const
    {
        return 0;
    }

    bool Constant::IsMonadic() const
    {
        return true;
    }

    std::optional<double> Constant::Evaluate(double input) const //NOLINT(misc-unused-parameter)
    {
        return this->value;
    }

    bool Constant::operator==(const Expression &other) const
    {
        if (const auto * b = dynamic_cast<const Constant*>(&other))
        {
            return b != nullptr && this->value == b->value;
        }
        else
        {
            return false;
        }
    }

    bool Constant::operator!=(const Expression &other) const
    {
        return !this->operator==(other);
    }

}
