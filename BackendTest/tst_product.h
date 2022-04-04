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

#ifndef TST_PRODUCT_H
#define TST_PRODUCT_H

#include <gmock/gmock-matchers.h>
#include <gtest/gtest.h>
#include "ComplexMatcher.h"

#include "../Backend/constant.h"
#include "../Backend/basez.h"
#include "../Backend/sum.h"
#include "../Backend/product.h"

TEST(BackendTest, ProductShallEvaluateCorrectly)
{
    using namespace std::complex_literals;

    // Arrange
    std::shared_ptr<Backend::BaseZ> z = std::make_shared<Backend::BaseZ>();
    std::shared_ptr<Backend::Constant> c1 = std::make_shared<Backend::Constant>(3.0+2.0i);
    std::shared_ptr<Backend::Constant> c2 = std::make_shared<Backend::Constant>(2.0-1.0i);

    std::vector<Backend::Sum::Summand> summands;
    summands.emplace_back(Backend::Sum::Sign::Plus, z);
    summands.emplace_back(Backend::Sum::Sign::Minus, c1);

    std::shared_ptr<Backend::Sum> sum = std::make_shared<Backend::Sum>(summands); // z - (3.0+2.0i)

    Backend::Product product({Backend::Product::Factor(Backend::Product::Exponent::Positive, z), Backend::Product::Factor(Backend::Product::Exponent::Negative, c2), Backend::Product::Factor(Backend::Product::Exponent::Positive, sum)}); // z / (2.0-1.0i) * (z - (3.0+2.0i)

    // Act
    auto result1 = product.Evaluate(0.0);
    auto result2 = product.Evaluate(-4.5+1.0i);

    // Assert
    ASSERT_TRUE(result1.has_value());
    ASSERT_TRUE(result2.has_value());

    EXPECT_THAT(result1.value(), COMPLEX_NEAR(0.0+0.0i));
    EXPECT_THAT(result2.value(), COMPLEX_NEAR(14.5+5.75i));
}

TEST(BackendTest, ProductShallEvaluateUndefinedDivision)
{
    using namespace std::complex_literals;

    // Arrange
    std::shared_ptr<Backend::Constant> c = std::make_shared<Backend::Constant>(1.0);
    std::shared_ptr<Backend::BaseZ> z = std::make_shared<Backend::BaseZ>();

    Backend::Product product({Backend::Product::Factor(Backend::Product::Exponent::Positive, c), Backend::Product::Factor(Backend::Product::Exponent::Negative, z)}); // 1 / z

    // Act
    auto result1 = product.Evaluate(0.0);
    auto result2 = product.Evaluate(-1.0e-10);
    auto result3 = product.Evaluate(-1.0e-9);
    auto result4 = product.Evaluate(-1.0e-10i);
    auto result5 = product.Evaluate(-1.0e-9i);

    // Assert
    EXPECT_FALSE(result1.has_value());
    EXPECT_FALSE(result2.has_value());
    ASSERT_TRUE(result3.has_value());
    EXPECT_FALSE(result4.has_value());
    ASSERT_TRUE(result5.has_value());

    EXPECT_THAT(result3.value(), COMPLEX_NEAR(-1.0e9+0.0i));
    EXPECT_THAT(result5.value(), COMPLEX_NEAR(1.0e9i));
}

#endif // TST_PRODUCT_H
