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

#include "basez.h"

namespace Backend {

    int BaseZ::GetLevel() const
    {
        return 0;
    }

    bool BaseZ::IsMonadic() const
    {
        return true;
    }

    std::optional<double> BaseZ::Evaluate(double input) const
    {
        return input;
    }

    bool BaseZ::operator==(const Expression &other) const
    {
        if (const auto * b = dynamic_cast<const BaseZ*>(&other))
        {
            return b != nullptr;
        }
        else
        {
            return false;
        }
    }

    bool BaseZ::operator!=(const Expression &other) const
    {
        return !this->operator==(other);
    }

}
