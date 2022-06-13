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

#ifndef TST_GRIDGENERATOR_H
#define TST_GRIDGENERATOR_H

#include <algorithm>
#include <gmock/gmock-matchers.h>
#include <gtest/gtest.h>

#include "../Backend/gridgenerator.h"
#include "doublehelper.h"

TEST(BackendTest, GridGeneratorShouldCreateRectangularGrid1)
{
    // Arrange
    Backend::GridGenerator gridGenerator(1.0, 1.0);

    // Act
    auto rect = gridGenerator.CreateSquare(1.0);

    // Assert
    ASSERT_EQ(9, rect.size());

    auto begin = rect.begin();
    auto end = rect.end();

    EXPECT_EQ(1, std::count_if(begin, end, [](auto & element){ return AreClose(element.real(), -1.0) && AreClose(element.imag(), -1.0); }));
    EXPECT_EQ(1, std::count_if(begin, end, [](auto & element){ return AreClose(element.real(), -1.0) && AreClose(element.imag(),  0.0); }));
    EXPECT_EQ(1, std::count_if(begin, end, [](auto & element){ return AreClose(element.real(), -1.0) && AreClose(element.imag(),  1.0); }));
    EXPECT_EQ(1, std::count_if(begin, end, [](auto & element){ return AreClose(element.real(),  0.0) && AreClose(element.imag(), -1.0); }));
    EXPECT_EQ(1, std::count_if(begin, end, [](auto & element){ return AreClose(element.real(),  0.0) && AreClose(element.imag(),  0.0); }));
    EXPECT_EQ(1, std::count_if(begin, end, [](auto & element){ return AreClose(element.real(),  0.0) && AreClose(element.imag(),  1.0); }));
    EXPECT_EQ(1, std::count_if(begin, end, [](auto & element){ return AreClose(element.real(),  1.0) && AreClose(element.imag(), -1.0); }));
    EXPECT_EQ(1, std::count_if(begin, end, [](auto & element){ return AreClose(element.real(),  1.0) && AreClose(element.imag(),  0.0); }));
    EXPECT_EQ(1, std::count_if(begin, end, [](auto & element){ return AreClose(element.real(),  1.0) && AreClose(element.imag(),  1.0); }));
}

TEST(BackendTest, GridGeneratorShouldCreateRectangularGrid2)
{
    // Arrange
    Backend::GridGenerator gridGenerator(3.0, 2.0);

    // Act
    auto rect = gridGenerator.CreateSquare(1.0);

    // Assert
    ASSERT_EQ(35, rect.size());
}

TEST(BackendTest, GridGeneratorShouldCreateRectangularGrid3)
{
    // Arrange
    Backend::GridGenerator gridGenerator(1.0, 1.0);

    // Act
    auto rect = gridGenerator.CreateSquare(0.5);

    // Assert
    ASSERT_EQ(25, rect.size());
}

TEST(BackendTest, GridGeneratorShouldCreateRectangularGrid4)
{
    // Arrange
    Backend::GridGenerator gridGenerator(10.0, 10.0);

    // Act
    auto rect = gridGenerator.CreateSquare(1.0);

    // Assert
    ASSERT_EQ(441, rect.size());
}

TEST(BackendTest, GridGeneratorShouldCreateAngularGridFromConstantAngle1)
{
    // Arrange
    Backend::GridGenerator gridGenerator(1.1, 1.1);

    // Act
    auto rect = gridGenerator.CreateAngularFromConstantAngle(1.0, 45.0);

    // Assert
    ASSERT_EQ(9, rect.size());

    auto begin = rect.begin();
    auto end = rect.end();
    const double helper = sqrt(0.5);

    EXPECT_EQ(1, std::count_if(begin, end, [&](auto & element){ return AreClose(element.real(), -helper) && AreClose(element.imag(), -helper); }));
    EXPECT_EQ(1, std::count_if(begin, end, [&](auto & element){ return AreClose(element.real(), -1.0) && AreClose(element.imag(),  0.0); }));
    EXPECT_EQ(1, std::count_if(begin, end, [&](auto & element){ return AreClose(element.real(), -helper) && AreClose(element.imag(),  helper); }));
    EXPECT_EQ(1, std::count_if(begin, end, [&](auto & element){ return AreClose(element.real(),  0.0) && AreClose(element.imag(), -1.0); }));
    EXPECT_EQ(1, std::count_if(begin, end, [&](auto & element){ return AreClose(element.real(),  0.0) && AreClose(element.imag(),  0.0); }));
    EXPECT_EQ(1, std::count_if(begin, end, [&](auto & element){ return AreClose(element.real(),  0.0) && AreClose(element.imag(),  1.0); }));
    EXPECT_EQ(1, std::count_if(begin, end, [&](auto & element){ return AreClose(element.real(),  helper) && AreClose(element.imag(), -helper); }));
    EXPECT_EQ(1, std::count_if(begin, end, [&](auto & element){ return AreClose(element.real(),  1.0) && AreClose(element.imag(),  0.0); }));
    EXPECT_EQ(1, std::count_if(begin, end, [&](auto & element){ return AreClose(element.real(),  helper) && AreClose(element.imag(),  helper); }));
}

TEST(BackendTest, GridGeneratorShouldCreateAngularGridFromConstantAngle2)
{
    // Arrange
    Backend::GridGenerator gridGenerator(10.0, 10.0);

    // Act
    auto rect = gridGenerator.CreateAngularFromConstantAngle(1.0, 15.0);

    // Assert
    ASSERT_EQ(265, rect.size());
}

TEST(BackendTest, GridGeneratorShouldCreateAngularGridFromConstantAngle3)
{
    // Arrange
    Backend::GridGenerator gridGenerator(1.1, 1.1);

    // Act
    auto rect = gridGenerator.CreateAngularFromConstantAngle(1.0, 15.0);

    // Assert
    ASSERT_EQ(25, rect.size());
}

TEST(BackendTest, GridGeneratorShouldCreateAngularGridFromConstantAngle4)
{
    // Arrange
    Backend::GridGenerator gridGenerator(1.1, 1.1);

    // Act
    auto rect = gridGenerator.CreateAngularFromConstantAngle(0.5, 45.0);

    // Assert
    ASSERT_EQ(21, rect.size());
}

TEST(BackendTest, GridGeneratorShouldCreateAngularFromApproximateDistance)
{
    // Arrange
    Backend::GridGenerator gridGenerator(10.0, 10.0);

    // Act
    auto rect = gridGenerator.CreateAngularFromApproximateDistance(1.5);

    // Assert
    ASSERT_EQ(171, rect.size());
}

#endif // TST_GRIDGENERATOR_H
