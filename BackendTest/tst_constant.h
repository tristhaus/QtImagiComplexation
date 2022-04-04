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

#ifndef TST_CONSTANT_H
#define TST_CONSTANT_H

#include <gmock/gmock-matchers.h>
#include <gtest/gtest.h>

#include "../Backend/constant.h"

TEST(BackendTest, ConstantShallEvaluateCorrectly)
{
    // Arrange
    Backend::Constant c(1.0);

    // Act
    auto result1 = c.Evaluate(0.0);
    auto result2 = c.Evaluate(-4.5);

    // Assert
    ASSERT_TRUE(result1.has_value());
    ASSERT_TRUE(result2.has_value());

    EXPECT_DOUBLE_EQ(1.0, result1.value());
    EXPECT_DOUBLE_EQ(1.0, result2.value());
}

#endif // TST_CONSTANT_H
