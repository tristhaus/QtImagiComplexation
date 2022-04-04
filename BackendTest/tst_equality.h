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
#include "../Backend/product.h"
#include "../Backend/sum.h"

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

TEST(BackendTest, EqualityShallWorkCorrectlyForSummands)
{
    using namespace std::complex_literals;

    // Arrange
    std::shared_ptr<Backend::Expression> z1 = std::make_shared<Backend::BaseZ>();
    std::shared_ptr<Backend::Expression> z2 = std::make_shared<Backend::BaseZ>();
    std::shared_ptr<Backend::Expression> z3 = std::make_shared<Backend::BaseZ>();

    auto z1Summand = Backend::Sum::Summand(Backend::Sum::Sign::Plus, z1);
    auto z2Summand = Backend::Sum::Summand(Backend::Sum::Sign::Plus, z2);
    auto z3Summand = Backend::Sum::Summand(Backend::Sum::Sign::Minus, z3);

    std::shared_ptr<Backend::Expression> c1 = std::make_shared<Backend::Constant>(3.0+2.0i);
    std::shared_ptr<Backend::Expression> c2 = std::make_shared<Backend::Constant>(3.0+2.0i);
    std::shared_ptr<Backend::Expression> c3 = std::make_shared<Backend::Constant>(4.0+1.0i);

    auto c1Summand = Backend::Sum::Summand(Backend::Sum::Sign::Minus, c1);
    auto c2Summand = Backend::Sum::Summand(Backend::Sum::Sign::Minus, c2);
    auto c3Summand = Backend::Sum::Summand(Backend::Sum::Sign::Minus, c3);

    // Act, Assert
    ASSERT_TRUE(z1Summand == z1Summand);
    ASSERT_TRUE(z1Summand == z2Summand);
    ASSERT_FALSE(z1Summand == z3Summand);

    ASSERT_FALSE(z1Summand != z1Summand);
    ASSERT_FALSE(z1Summand != z2Summand);
    ASSERT_TRUE(z1Summand != z3Summand);

    ASSERT_TRUE(c1Summand == c1Summand);
    ASSERT_TRUE(c1Summand == c2Summand);
    ASSERT_FALSE(c1Summand == c3Summand);

    ASSERT_FALSE(c1Summand != c1Summand);
    ASSERT_FALSE(c1Summand != c2Summand);
    ASSERT_TRUE(c1Summand != c3Summand);

    ASSERT_EQ(z1Summand, z1Summand);
    ASSERT_EQ(z1Summand, z2Summand);
    ASSERT_NE(z1Summand, z3Summand);

    ASSERT_EQ(c1Summand, c1Summand);
    ASSERT_EQ(c1Summand, c2Summand);
    ASSERT_NE(c1Summand, c3Summand);
}

TEST(BackendTest, EqualityShallWorkCorrectlyForSums)
{
    using namespace std::complex_literals;

    // Arrange
    std::shared_ptr<Backend::Expression> z1 = std::make_shared<Backend::BaseZ>();
    std::shared_ptr<Backend::Expression> z2 = std::make_shared<Backend::BaseZ>();
    std::shared_ptr<Backend::Expression> c1 = std::make_shared<Backend::Constant>(3.0+2.0i);

    auto z1Summand = Backend::Sum::Summand(Backend::Sum::Sign::Plus, z1);  // z
    auto z2Summand = Backend::Sum::Summand(Backend::Sum::Sign::Minus, z2); // - z
    auto c1Summand = Backend::Sum::Summand(Backend::Sum::Sign::Plus, c1);  // 3.0+2.0i

    Backend::Sum s2 = Backend::Sum({z1Summand, z1Summand});            // z + z
    Backend::Sum s1 = Backend::Sum({z1Summand, z1Summand});            // z + z
    Backend::Sum s3 = Backend::Sum({z1Summand, z2Summand});            // z - z
    Backend::Sum s4 = Backend::Sum({z1Summand, z1Summand, z1Summand}); // z + z + z
    Backend::Sum s5 = Backend::Sum({z2Summand, z1Summand});            // - z + z
    Backend::Sum s6 = Backend::Sum({c1Summand, z1Summand, z2Summand}); // 3.0+2.0i + z - z
    Backend::Sum s7 = Backend::Sum({z2Summand, c1Summand, z1Summand}); // - z + 3.0+2.0i + z

    // Act, Assert
    ASSERT_TRUE(s1 == s2);
    ASSERT_FALSE(s1 == s3);
    ASSERT_FALSE(s1 == s4);
    ASSERT_TRUE(s3 == s5);
    ASSERT_TRUE(s6 == s7);

    ASSERT_FALSE(s1 != s2);
    ASSERT_TRUE(s1 != s3);
    ASSERT_TRUE(s1 != s4);
    ASSERT_FALSE(s3 != s5);
    ASSERT_FALSE(s6 != s7);

    ASSERT_EQ(s1, s2);
    ASSERT_NE(s1, s3);
    ASSERT_NE(s1, s4);
    ASSERT_EQ(s3, s5);
    ASSERT_EQ(s6, s7);
}

// while we do not consider ( a + b ) + c == a + ( b + c ) to be true (associativity)
// we should have commutativity even when recursive comparison is needed, so
// ( a + b ) + c == c + ( b + a ) should be true
TEST(BackendTest, EqualityShallWorkCorrectlyForRecursiveSums)
{
    using namespace std::complex_literals;

    // Arrange
    std::shared_ptr<Backend::Expression> z1 = std::make_shared<Backend::BaseZ>();
    std::shared_ptr<Backend::Expression> z2 = std::make_shared<Backend::BaseZ>();
    std::shared_ptr<Backend::Expression> c = std::make_shared<Backend::Constant>(3.0+2.0i);

    auto z1Summand = Backend::Sum::Summand(Backend::Sum::Sign::Plus, z1);  // z
    auto z2Summand = Backend::Sum::Summand(Backend::Sum::Sign::Minus, z2); // -z
    auto cSummand = Backend::Sum::Summand(Backend::Sum::Sign::Minus, c);   // -(3.0+2.0i)

    std::shared_ptr<Backend::Expression> s1 = std::make_shared<Backend::Sum>(std::vector<Backend::Sum::Summand>({z1Summand, z1Summand})); // z + z
    std::shared_ptr<Backend::Expression> s2 = std::make_shared<Backend::Sum>(std::vector<Backend::Sum::Summand>({z1Summand, z2Summand})); // z - z

    auto s1Summand = Backend::Sum::Summand(Backend::Sum::Sign::Plus, s1);  // z + z
    auto s2Summand = Backend::Sum::Summand(Backend::Sum::Sign::Minus, s2); // -(z - z)

    std::shared_ptr<Backend::Expression> s3 = std::make_shared<Backend::Sum>(std::vector<Backend::Sum::Summand>({cSummand, s1Summand, s2Summand})); // - (3.0+2.0i) + ( z + z ) - ( z - z )
    std::shared_ptr<Backend::Expression> s4 = std::make_shared<Backend::Sum>(std::vector<Backend::Sum::Summand>({s2Summand, s1Summand, cSummand})); // - ( z - z ) + ( z + z ) - (3.0+2.0i)

    ASSERT_TRUE(*s3 == *s4);
    ASSERT_FALSE(*s3 != *s4);

    ASSERT_EQ(*s3, *s4);
}

TEST(BackendTest, EqualityShallWorkCorrectlyForFactors)
{
    using namespace std::complex_literals;

    // Arrange
    std::shared_ptr<Backend::Expression> z1 = std::make_shared<Backend::BaseZ>();
    std::shared_ptr<Backend::Expression> z2 = std::make_shared<Backend::BaseZ>();
    std::shared_ptr<Backend::Expression> z3 = std::make_shared<Backend::BaseZ>();

    auto z1Factor = Backend::Product::Factor(Backend::Product::Exponent::Positive, z1);
    auto z2Factor = Backend::Product::Factor(Backend::Product::Exponent::Positive, z2);
    auto z3Factor = Backend::Product::Factor(Backend::Product::Exponent::Negative, z3);

    std::shared_ptr<Backend::Expression> c1 = std::make_shared<Backend::Constant>(3.0);
    std::shared_ptr<Backend::Expression> c2 = std::make_shared<Backend::Constant>(3.0);
    std::shared_ptr<Backend::Expression> c3 = std::make_shared<Backend::Constant>(4.0);

    auto c1Factor = Backend::Product::Factor(Backend::Product::Exponent::Positive, c1);
    auto c2Factor = Backend::Product::Factor(Backend::Product::Exponent::Positive, c2);
    auto c3Factor = Backend::Product::Factor(Backend::Product::Exponent::Negative, c3);

    // Act, Assert
    ASSERT_TRUE(z1Factor == z1Factor);
    ASSERT_TRUE(z1Factor == z2Factor);
    ASSERT_FALSE(z1Factor == z3Factor);

    ASSERT_FALSE(z1Factor != z1Factor);
    ASSERT_FALSE(z1Factor != z2Factor);
    ASSERT_TRUE(z1Factor != z3Factor);

    ASSERT_TRUE(c1Factor == c1Factor);
    ASSERT_TRUE(c1Factor == c2Factor);
    ASSERT_FALSE(c1Factor == c3Factor);

    ASSERT_FALSE(c1Factor != c1Factor);
    ASSERT_FALSE(c1Factor != c2Factor);
    ASSERT_TRUE(c1Factor != c3Factor);

    ASSERT_EQ(z1Factor, z1Factor);
    ASSERT_EQ(z1Factor, z2Factor);
    ASSERT_NE(z1Factor, z3Factor);

    ASSERT_EQ(c1Factor, c1Factor);
    ASSERT_EQ(c1Factor, c2Factor);
    ASSERT_NE(c1Factor, c3Factor);
}

TEST(BackendTest, EqualityShallWorkCorrectlyForProducts)
{
    using namespace std::complex_literals;

    // Arrange
    std::shared_ptr<Backend::Expression> z1 = std::make_shared<Backend::BaseZ>();
    std::shared_ptr<Backend::Expression> z2 = std::make_shared<Backend::BaseZ>();
    std::shared_ptr<Backend::Expression> c1 = std::make_shared<Backend::Constant>(3.0+2.0i);

    auto z1Factor = Backend::Product::Factor(Backend::Product::Exponent::Positive, z1);
    auto z2Factor = Backend::Product::Factor(Backend::Product::Exponent::Negative, z2);
    auto c1Factor = Backend::Product::Factor(Backend::Product::Exponent::Positive, c1);

    Backend::Product p1 = Backend::Product({z1Factor, z1Factor});           // z * z
    Backend::Product p2 = Backend::Product({z1Factor, z1Factor});           // z * z
    Backend::Product p3 = Backend::Product({z1Factor, z2Factor});           // z / z
    Backend::Product p4 = Backend::Product({z1Factor, z1Factor, z1Factor}); // z * z * z
    Backend::Product p5 = Backend::Product({z2Factor, z1Factor});           // 1 / z * z
    Backend::Product p6 = Backend::Product({c1Factor, z1Factor, z2Factor}); // 3.0+2.0i * z / z
    Backend::Product p7 = Backend::Product({z2Factor, c1Factor, z1Factor}); // 1 / z * 3.0+2.0i * z

    // Act, Assert
    ASSERT_TRUE(p1 == p2);
    ASSERT_FALSE(p1 == p3);
    ASSERT_FALSE(p1 == p4);
    ASSERT_TRUE(p3 == p5);
    ASSERT_TRUE(p6 == p7);

    ASSERT_FALSE(p1 != p2);
    ASSERT_TRUE(p1 != p3);
    ASSERT_TRUE(p1 != p4);
    ASSERT_FALSE(p3 != p5);
    ASSERT_FALSE(p6 != p7);

    ASSERT_EQ(p1, p2);
    ASSERT_NE(p1, p3);
    ASSERT_NE(p1, p4);
    ASSERT_EQ(p3, p5);
    ASSERT_EQ(p6, p7);
}

// while we do not consider ( a * b ) * c == a * ( b * c ) to be true (associativity)
// we should have commutativity even when recursive comparison is needed, so
// ( a * b ) * c == c * ( b * a ) should be true
TEST(BackendTest, EqualityShallWorkCorrectlyForRecursiveProducts)
{
    using namespace std::complex_literals;

    // Arrange
    std::shared_ptr<Backend::Expression> z1 = std::make_shared<Backend::BaseZ>();
    std::shared_ptr<Backend::Expression> z2 = std::make_shared<Backend::BaseZ>();
    std::shared_ptr<Backend::Expression> c = std::make_shared<Backend::Constant>(3.0+2.0i);

    auto z1Factor = Backend::Product::Factor(Backend::Product::Exponent::Positive, z1); // z
    auto z2Factor = Backend::Product::Factor(Backend::Product::Exponent::Negative, z2); // 1 / z
    auto cFactor = Backend::Product::Factor(Backend::Product::Exponent::Negative, c);   // 1 / 3.0+2.0i

    std::shared_ptr<Backend::Expression> p1 = std::make_shared<Backend::Product>(std::vector<Backend::Product::Factor>({z1Factor, z1Factor})); // z * z
    std::shared_ptr<Backend::Expression> p2 = std::make_shared<Backend::Product>(std::vector<Backend::Product::Factor>({z1Factor, z2Factor})); // z / z

    auto s1Factor = Backend::Product::Factor(Backend::Product::Exponent::Positive, p1); // 1 * ( z * z )
    auto s2Factor = Backend::Product::Factor(Backend::Product::Exponent::Negative, p2); // 1 / ( z * z )

    std::shared_ptr<Backend::Expression> p3 = std::make_shared<Backend::Product>(std::vector<Backend::Product::Factor>({cFactor, s1Factor, s2Factor})); // 1 / 3.0+2.0i * ( z * z ) / ( z / z )
    std::shared_ptr<Backend::Expression> p4 = std::make_shared<Backend::Product>(std::vector<Backend::Product::Factor>({s2Factor, s1Factor, cFactor})); // 1 / ( z / z ) * ( z * z ) / 3.0+2.0i

    ASSERT_TRUE(*p3 == *p4);
    ASSERT_FALSE(*p3 != *p4);

    ASSERT_EQ(*p3, *p4);
}

#endif // TST_EQUALITY_H
