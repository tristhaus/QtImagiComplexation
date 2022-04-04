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

#ifndef TST_BASEZ_H
#define TST_BASEZ_H

#include <gmock/gmock-matchers.h>
#include <gtest/gtest.h>

#include "../Backend/basez.h"

TEST(BackendTest, BaseZShallEvaluateCorrectly)
{
    using namespace std::complex_literals;

    // Arrange
    Backend::BaseZ z;

    // Act
    auto result1 = z.Evaluate(0.0+0.0i);
    auto result2 = z.Evaluate(-4.5-3.1i);

    // Assert
    ASSERT_TRUE(result1.has_value());
    ASSERT_TRUE(result2.has_value());

    EXPECT_DOUBLE_EQ(0.0, result1.value().real());
    EXPECT_DOUBLE_EQ(0.0, result1.value().imag());
    EXPECT_DOUBLE_EQ(-4.5, result2.value().real());
    EXPECT_DOUBLE_EQ(-3.1, result2.value().imag());
}

#endif // TST_BASEX_H
