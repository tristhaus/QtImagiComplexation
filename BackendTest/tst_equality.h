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

#ifndef TST_EQUALITY_H
#define TST_EQUALITY_H

#include <gmock/gmock-matchers.h>
#include <gtest/gtest.h>
#include <memory>

#include "../Backend/basez.h"
#include "../Backend/constant.h"
#include "../Backend/expression.h"

TEST(BackendTest, EqualityShallWorkCorrectlyUsingRawPointers)
{
    using namespace std::complex_literals;

    // Arrange
    Backend::Constant constant1(2.3+4.8i);
    Backend::Constant constant2(2.3+4.8i);
    Backend::BaseZ baseZ1;
    Backend::BaseZ baseZ2;

    const Backend::Expression * const pC1 = &constant1;
    const Backend::Expression * const pC2 = &constant2;
    const Backend::Expression * const pB1 = &baseZ1;
    const Backend::Expression * const pB2 = &baseZ2;

    // Act, Assert
    ASSERT_TRUE(*pC1 == *pC2);
    ASSERT_TRUE(*pB1 == *pB2);
    ASSERT_FALSE(*pB1 == *pC2);
    ASSERT_FALSE(*pC1 != *pC2);
    ASSERT_FALSE(*pC1 != *pC1);

    ASSERT_EQ(*pC1, *pC2);
    ASSERT_EQ(*pB1, *pB2);
    ASSERT_NE(*pC1, *pB1);
}

TEST(BackendTest, EqualityShallWorkCorrectlyUsingSmartPointers)
{
    using namespace std::complex_literals;

    // Arrange
    std::shared_ptr<Backend::Constant> constant1 = std::make_shared<Backend::Constant>(2.3+4.8i);
    std::shared_ptr<Backend::Constant> constant2 = std::make_shared<Backend::Constant>(2.3+4.8i);
    std::shared_ptr<Backend::BaseZ> baseZ1 = std::make_shared<Backend::BaseZ>();
    std::shared_ptr<Backend::BaseZ> baseZ2 = std::make_shared<Backend::BaseZ>();

    std::shared_ptr<Backend::Expression> pC1(constant1);
    std::shared_ptr<Backend::Expression> pC2(constant2);
    std::shared_ptr<Backend::Expression> pB1(baseZ1);
    std::shared_ptr<Backend::Expression> pB2(baseZ2);

    // Act, Assert
    ASSERT_TRUE(*pC1 == *pC2);
    ASSERT_TRUE(*pB1 == *pB2);
    ASSERT_FALSE(*pB1 == *pC2);
    ASSERT_FALSE(*pC1 != *pC2);
    ASSERT_FALSE(*pC1 != *pC1);

    ASSERT_EQ(*pC1, *pC2);
    ASSERT_EQ(*pB1, *pB2);
    ASSERT_NE(*pC1, *pB1);
}

TEST(BackendTest, EqualityShallWorkCorrectlyUsingReferences)
{
    using namespace std::complex_literals;

    // Arrange
    Backend::Constant constant1(2.3+4.8i);
    Backend::Constant constant2(2.3+4.8i);
    Backend::BaseZ baseZ1;
    Backend::BaseZ baseZ2;

    const Backend::Expression& pC1 = constant1;
    const Backend::Expression& pC2 = constant2;
    const Backend::Expression& pB1 = baseZ1;
    const Backend::Expression& pB2 = baseZ2;

    // Act, Assert
    ASSERT_TRUE(pC1 == pC2);
    ASSERT_TRUE(pB1 == pB2);
    ASSERT_FALSE(pB1 == pC2);
    ASSERT_FALSE(pC1 != pC2);
    ASSERT_FALSE(pC1 != pC1);

    ASSERT_EQ(pC1, pC2);
    ASSERT_EQ(pB1, pB2);
    ASSERT_NE(pC1, pB1);
}

#endif // TST_EQUALITY_H
