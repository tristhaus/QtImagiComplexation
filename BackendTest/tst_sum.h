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

#ifndef TST_SUM_H
#define TST_SUM_H

#include <gmock/gmock-matchers.h>
#include <gtest/gtest.h>

#include "ComplexMatcher.h"

#include "../Backend/basez.h"
#include "../Backend/constant.h"
#include "../Backend/sum.h"

TEST(BackendTest, SumShallEvaluateCorrectly)
{
    using namespace std::complex_literals;

    // Arrange
    std::shared_ptr<Backend::BaseZ> z = std::make_shared<Backend::BaseZ>();
    std::shared_ptr<Backend::Constant> c = std::make_shared<Backend::Constant>(3.0+2.0i);

    Backend::Sum sum({Backend::Sum::Summand(Backend::Sum::Sign::Plus, z), Backend::Sum::Summand(Backend::Sum::Sign::Minus, c)});

    // Act
    auto result1 = sum.Evaluate(0.0);
    auto result2 = sum.Evaluate(-4.5+2.5i);

    // Assert
    ASSERT_TRUE(result1.has_value());
    ASSERT_TRUE(result2.has_value());

    EXPECT_THAT(result1.value(), COMPLEX_NEAR(-3.0-2.0i));
    EXPECT_THAT(result2.value(), COMPLEX_NEAR(-7.5+0.5i));
}

TEST(BackendTest, SumShallDetermineConstantnessCorrectly)
{
    using namespace std::complex_literals;

    // Arrange
    std::shared_ptr<Backend::BaseZ> z = std::make_shared<Backend::BaseZ>();
    std::shared_ptr<Backend::Constant> c1 = std::make_shared<Backend::Constant>(3.0+2.0i);
    std::shared_ptr<Backend::Constant> c2 = std::make_shared<Backend::Constant>(2.0-1.0i);

    std::vector<Backend::Sum::Summand> summands1;
    summands1.emplace_back(Backend::Sum::Sign::Plus, z);
    summands1.emplace_back(Backend::Sum::Sign::Plus, c1);

    std::vector<Backend::Sum::Summand> summands2;
    summands2.emplace_back(Backend::Sum::Sign::Plus, c1);
    summands2.emplace_back(Backend::Sum::Sign::Plus, c2);

    auto sum1 = std::make_shared<Backend::Sum>(summands1);
    auto sum2 = std::make_shared<Backend::Sum>(summands2);

    // Act
    auto result1 = sum1->IsConstant();
    auto result2 = sum2->IsConstant();

    // Assert
    EXPECT_FALSE(result1);
    EXPECT_TRUE(result2);
}

#endif // TST_SUM_H
