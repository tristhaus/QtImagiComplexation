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

#ifndef TST_FUNDAMENTAL_H
#define TST_FUNDAMENTAL_H

#include <cmath>

#include <gmock/gmock-matchers.h>
#include <gtest/gtest.h>

TEST(BackendTest, AssumedMathErrorHandlingShallBeInPlace)
{
    // Assert
    ASSERT_TRUE(math_errhandling & MATH_ERREXCEPT);
}

#endif // TST_FUNDAMENTAL_H
