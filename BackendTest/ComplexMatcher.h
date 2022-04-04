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

#ifndef COMPLEXMATCHER_H
#define COMPLEXMATCHER_H

#include <gmock/gmock-matchers.h>

#include "../Backend/expression.h"

#ifdef _MSC_VER
#pragma warning(push)
#pragma warning(disable : 4100)
#endif

MATCHER_P(COMPLEX_NEAR, ref, "") {
    const double epsilon = 1e-6;
    return std::fabs(arg.real()-ref.real()) < epsilon && std::fabs(arg.imag()-ref.imag()) < epsilon;
}

#ifdef _MSC_VER
#pragma warning(push)
#pragma warning(default : 4100)
#endif

#endif // COMPLEXMATCHER_H
