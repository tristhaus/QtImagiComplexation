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

#ifndef TST_FUNCTIONS_H
#define TST_FUNCTIONS_H

#include <gmock/gmock-matchers.h>
#include <gtest/gtest.h>
#include <memory>

#include "ComplexMatcher.h"

#include "../Backend/basez.h"
#include "../Backend/constant.h"
#include "../Backend/expression.h"
#include "../Backend/functions.h"

TEST(BackendTest, MagnitudeShallEvaluateCorrectly)
{
    using namespace std::complex_literals;

    // Arrange
    auto baseZ = std::make_shared<Backend::BaseZ>();
    auto tangent = std::make_shared<Backend::Magnitude>(baseZ);

    // Act
    auto optional = tangent->Evaluate(-1.0+1.0i);
    auto value = optional.value_or(-99.0);

    // Assert
    ASSERT_TRUE(optional.has_value());

    EXPECT_THAT(value, COMPLEX_NEAR(1.41421356237+0.0i));
}

TEST(BackendTest, RealPartShallEvaluateCorrectly)
{
    using namespace std::complex_literals;

    // Arrange
    auto baseZ = std::make_shared<Backend::BaseZ>();
    auto tangent = std::make_shared<Backend::RealPart>(baseZ);

    // Act
    auto optional = tangent->Evaluate(-1.0+1.0i);
    auto value = optional.value_or(-99.0);

    // Assert
    ASSERT_TRUE(optional.has_value());

    EXPECT_THAT(value, COMPLEX_NEAR(-1.0+0.0i));
}

TEST(BackendTest, ImaginaryPartShallEvaluateCorrectly)
{
    using namespace std::complex_literals;

    // Arrange
    auto baseZ = std::make_shared<Backend::BaseZ>();
    auto tangent = std::make_shared<Backend::ImaginaryPart>(baseZ);

    // Act
    auto optional = tangent->Evaluate(-1.0+1.0i);
    auto value = optional.value_or(-99.0);

    // Assert
    ASSERT_TRUE(optional.has_value());

    EXPECT_THAT(value, COMPLEX_NEAR(1.0+0.0i));
}


TEST(BackendTest, NormShallEvaluateCorrectly)
{
    using namespace std::complex_literals;

    // Arrange
    auto baseZ = std::make_shared<Backend::BaseZ>();
    auto tangent = std::make_shared<Backend::Norm>(baseZ);

    // Act
    auto optional = tangent->Evaluate(-2.0+2.0i);
    auto value = optional.value_or(-99.0);

    // Assert
    ASSERT_TRUE(optional.has_value());

    EXPECT_THAT(value, COMPLEX_NEAR(8.0+0.0i));
}

TEST(BackendTest, ConjugateShallEvaluateCorrectly)
{
    using namespace std::complex_literals;

    // Arrange
    auto baseZ = std::make_shared<Backend::BaseZ>();
    auto tangent = std::make_shared<Backend::Conjugate>(baseZ);

    // Act
    auto optional = tangent->Evaluate(-3.0+2.0i);
    auto value = optional.value_or(-99.0);

    // Assert
    ASSERT_TRUE(optional.has_value());

    EXPECT_THAT(value, COMPLEX_NEAR(-3.0-2.0i));
}

TEST(BackendTest, SineShallEvaluateCorrectly)
{
    using namespace std::complex_literals;

    // Arrange
    auto baseZ = std::make_shared<Backend::BaseZ>();
    auto sine = std::make_shared<Backend::Sine>(baseZ);

    // Act
    auto optional = sine->Evaluate(0.8+0.1i);
    auto value = optional.value_or(-99.0);

    // Assert
    ASSERT_TRUE(optional.has_value());

    EXPECT_THAT(value, COMPLEX_NEAR(0.720946+0.0697868i));
}

TEST(BackendTest, CosineShallEvaluateCorrectly)
{
    using namespace std::complex_literals;

    // Arrange
    auto baseZ = std::make_shared<Backend::BaseZ>();
    auto cosine = std::make_shared<Backend::Cosine>(baseZ);

    // Act
    auto optional = cosine->Evaluate(0.8+0.1i);
    auto value = optional.value_or(-99.0);

    // Assert
    ASSERT_TRUE(optional.has_value());

    EXPECT_THAT(value, COMPLEX_NEAR(0.700193-0.0718552i));
}

TEST(BackendTest, TangentShallEvaluateCorrectly)
{
    using namespace std::complex_literals;

    // Arrange
    auto baseZ = std::make_shared<Backend::BaseZ>();
    auto tangent = std::make_shared<Backend::Tangent>(baseZ);

    // Act
    auto optional = tangent->Evaluate(0.8+0.1i);
    auto value = optional.value_or(-99.0);

    // Assert
    ASSERT_TRUE(optional.has_value());

    EXPECT_THAT(value, COMPLEX_NEAR(1.0087866158360751555+0.2031917049743860900i));
}

TEST(BackendTest, SquareRootShallEvaluateCorrectly)
{
    using namespace std::complex_literals;

    // Arrange
    auto baseZ = std::make_shared<Backend::BaseZ>();
    auto tangent = std::make_shared<Backend::SquareRoot>(baseZ);

    // Act
    auto optional = tangent->Evaluate(0.8+0.1i);
    auto value = optional.value_or(-99.0);

    // Assert
    ASSERT_TRUE(optional.has_value());

    EXPECT_THAT(value, COMPLEX_NEAR(0.89616565846662941775023231+0.05579325599861942649778994i));
}

TEST(BackendTest, NaturalExponentialShallEvaluateCorrectly)
{
    using namespace std::complex_literals;

    // Arrange
    auto baseZ = std::make_shared<Backend::BaseZ>();
    auto tangent = std::make_shared<Backend::NaturalExponential>(baseZ);

    // Act
    auto optional = tangent->Evaluate(0.8+0.1i);
    auto value = optional.value_or(-99.0);

    // Assert
    ASSERT_TRUE(optional.has_value());

    EXPECT_THAT(value, COMPLEX_NEAR(2.21442249384673015930+0.22218335477875729789i));
}

TEST(BackendTest, NaturalLogarithmShallEvaluateCorrectly)
{
    using namespace std::complex_literals;

    // Arrange
    auto baseZ = std::make_shared<Backend::BaseZ>();
    auto tangent = std::make_shared<Backend::NaturalLogarithm>(baseZ);

    // Act
    auto optional = tangent->Evaluate(0.8+0.1i);
    auto value = optional.value_or(-99.0);

    // Assert
    ASSERT_TRUE(optional.has_value());

    EXPECT_THAT(value, COMPLEX_NEAR(-0.2153914580462271+0.1243549945467614i));
}

#endif // TST_FUNCTIONS_H
