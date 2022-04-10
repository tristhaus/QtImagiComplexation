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

#ifndef TST_POWER_H
#define TST_POWER_H

#include <gmock/gmock-matchers.h>
#include <gtest/gtest.h>

#include "tst_complexmatcher.h"

#include "../Backend/basez.h"
#include "../Backend/constant.h"
#include "../Backend/expression.h"
#include "../Backend/power.h"
#include "../Backend/product.h"
#include "../Backend/sum.h"

TEST(BackendTest, PowerShallEvaluateRealCorrectly)
{
    using namespace std::complex_literals;

    // Arrange
    std::shared_ptr<Backend::BaseZ> z = std::make_shared<Backend::BaseZ>();
    std::shared_ptr<Backend::Constant> c1 = std::make_shared<Backend::Constant>(3.0);
    std::shared_ptr<Backend::Constant> c2 = std::make_shared<Backend::Constant>(2.0);

    std::shared_ptr<Backend::Power> q1 = std::make_shared<Backend::Power>(c1, z);
    std::shared_ptr<Backend::Power> q2 = std::make_shared<Backend::Power>(z, c2);

    // Act
    auto result1 = q1->Evaluate(0.0);
    auto result2 = q2->Evaluate(-4.5);

    // Assert
    ASSERT_TRUE(result1.has_value());
    ASSERT_TRUE(result2.has_value());

    EXPECT_THAT(result1.value(), COMPLEX_NEAR(1.0+0.0i));
    EXPECT_THAT(result2.value(), COMPLEX_NEAR(20.25+0.0i));
}

TEST(BackendTest, PowerShallEvaluateDifficultRealCasesCorrectly)
{
    using namespace std::complex_literals;

    // Arrange
    std::shared_ptr<Backend::BaseZ> z = std::make_shared<Backend::BaseZ>();
    std::shared_ptr<Backend::Constant> c1 = std::make_shared<Backend::Constant>(-2.0);
    std::shared_ptr<Backend::Constant> c2 = std::make_shared<Backend::Constant>(0.5);
    std::shared_ptr<Backend::Constant> c3 = std::make_shared<Backend::Constant>(0.333333333);

    std::shared_ptr<Backend::Power> q1 = std::make_shared<Backend::Power>(c1, z); // (-2.0) ^ z
    std::shared_ptr<Backend::Power> q2 = std::make_shared<Backend::Power>(z, c2); // z ^ 0.5
    std::shared_ptr<Backend::Power> q3 = std::make_shared<Backend::Power>(z, c3); // z ^ 0.333333333

    // Act
    auto result1 = q1->Evaluate(0.0);
    auto result2 = q1->Evaluate(2.0);
    auto result3 = q1->Evaluate(1.5);

    auto result4 = q2->Evaluate(1.0);
    auto result5 = q2->Evaluate(9.0);
    auto result6 = q2->Evaluate(-9.0);

    auto result7 = q3->Evaluate(8.0);
    auto result8 = q3->Evaluate(-8.0);

    // Assert
    ASSERT_TRUE(result1.has_value());
    ASSERT_TRUE(result2.has_value());
    ASSERT_TRUE(result3.has_value());

    ASSERT_TRUE(result4.has_value());
    ASSERT_TRUE(result5.has_value());
    ASSERT_TRUE(result6.has_value());

    ASSERT_TRUE(result7.has_value());
    ASSERT_TRUE(result8.has_value());

    EXPECT_THAT(result1.value(), COMPLEX_NEAR(1.0+0.0i));
    EXPECT_THAT(result2.value(), COMPLEX_NEAR(4.0+0.0i));
    EXPECT_THAT(result3.value(), COMPLEX_NEAR(0.0-2.82842712i)); // WolframAlpha

    EXPECT_THAT(result4.value(), COMPLEX_NEAR(1.0+0.0i));
    EXPECT_THAT(result5.value(), COMPLEX_NEAR(3.0+0.0i));
    EXPECT_THAT(result6.value(), COMPLEX_NEAR(0.0+3.0i));

    EXPECT_THAT(result7.value(), COMPLEX_NEAR(2.0+0.0i));
    EXPECT_THAT(result8.value(), COMPLEX_NEAR(1.0+1.73205080757i)); // WolframAlpha
}

TEST(BackendTest, PowerShallEvaluateComplexCasesCorrectly)
{
    using namespace std::complex_literals;

    // Arrange
    std::shared_ptr<Backend::BaseZ> z = std::make_shared<Backend::BaseZ>();
    std::shared_ptr<Backend::Constant> c1 = std::make_shared<Backend::Constant>(0.0+1.0i);
    std::shared_ptr<Backend::Constant> c2 = std::make_shared<Backend::Constant>(0.5+0.3i);
    std::shared_ptr<Backend::Constant> c3 = std::make_shared<Backend::Constant>(0.0+0.333333333i);

    std::shared_ptr<Backend::Power> q1 = std::make_shared<Backend::Power>(c1, z); // i ^ z
    std::shared_ptr<Backend::Power> q2 = std::make_shared<Backend::Power>(z, c2); // z ^ (0.5+0.3i)
    std::shared_ptr<Backend::Power> q3 = std::make_shared<Backend::Power>(z, c3); // z ^ 0.333333333i

    // Act
    auto result1 = q1->Evaluate(0.0+0.0i);
    auto result2 = q1->Evaluate(2.0+0.0i);
    auto result3 = q1->Evaluate(3.0+0.0i);
    auto result4 = q1->Evaluate(0.0+1.0i);

    auto result5 = q2->Evaluate(1.0-2.0i);

    auto result6 = q3->Evaluate(8.0+0.1i);

    // Assert
    ASSERT_TRUE(result1.has_value());
    ASSERT_TRUE(result2.has_value());
    ASSERT_TRUE(result3.has_value());
    ASSERT_TRUE(result4.has_value());

    ASSERT_TRUE(result5.has_value());

    ASSERT_TRUE(result6.has_value());

    EXPECT_THAT(result1.value(), COMPLEX_NEAR(1.0+0.0i));
    EXPECT_THAT(result2.value(), COMPLEX_NEAR(-1.0-0.0i));
    EXPECT_THAT(result3.value(), COMPLEX_NEAR(0.0-1.0i));
    EXPECT_THAT(result4.value(), COMPLEX_NEAR(0.20787957635+0.0i)); // WolframAlpha

    EXPECT_THAT(result5.value(), COMPLEX_NEAR(1.98371256965-0.64016255131i)); // WolframAlpha

    EXPECT_THAT(result6.value(), COMPLEX_NEAR(0.766024004703597+0.636324561250512i)); // WolframAlpha
}

#endif // TST_POWER_H
