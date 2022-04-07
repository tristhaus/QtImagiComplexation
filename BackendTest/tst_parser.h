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

#ifndef TST_PARSER_H
#define TST_PARSER_H

#include <gmock/gmock-matchers.h>
#include <gtest/gtest.h>

#include "SubsetGenerator.h"

#include "../Backend/basez.h"
#include "../Backend/constant.h"
#include "../Backend/parser.h"
#include "../Backend/product.h"
#include "../Backend/sum.h"

TEST(BackendTest, SimpleCasesShouldParseCorrectly)
{
    // Arrange
    Backend::Parser parser(false);
    std::string z1 = "Z";
    std::string z2 = "z";
    std::string y = "Y";
    std::string number1 = " +030.500 ";
    std::string number2 = " -014.500 ";
    std::string invalidNumber = "030.50.0";
    std::string bracedZ = "(Z)";
    std::string doubleBracedZ = "((z))";

    // Act
    auto exprZ1 = parser.Parse(z1);
    auto exprZ2 = parser.Parse(z2);
    auto exprInvalid = parser.Parse(y);
    auto exprNumber1 = parser.Parse(number1);
    auto exprNumber2 = parser.Parse(number2);
    auto exprInvalidNumber = parser.Parse(invalidNumber);
    auto exprBracedZ = parser.Parse(bracedZ);
    auto exprDoubleBracedZ = parser.Parse(doubleBracedZ);

    // Assert
    Backend::BaseZ referenceZ;
    Backend::Constant referenceConstant1(30.5);
    Backend::Constant referenceConstant2(-14.5);

    ASSERT_TRUE(exprZ1);
    EXPECT_EQ(referenceZ, *exprZ1);
    ASSERT_TRUE(exprZ2);
    EXPECT_EQ(referenceZ, *exprZ2);
    EXPECT_FALSE(exprInvalid);
    ASSERT_TRUE(exprNumber1);
    EXPECT_EQ(referenceConstant1, *exprNumber1);
    ASSERT_TRUE(exprNumber2);
    EXPECT_EQ(referenceConstant2, *exprNumber2);
    EXPECT_FALSE(exprInvalidNumber);
    ASSERT_TRUE(exprBracedZ);
    EXPECT_EQ(referenceZ, *exprBracedZ);
    ASSERT_TRUE(exprDoubleBracedZ);
    EXPECT_EQ(referenceZ, *exprDoubleBracedZ);
}

TEST(BackendTest, ParserShallParseRealNumber)
{
    // Arrange
    Backend::Parser parser(false);
    Backend::Constant referenceConstant1(-0.4);
    Backend::Constant referenceConstant2(0.6);

    // Act
    auto expression1 = parser.Parse(std::string(u8"-0.4"));
    auto expression2 = parser.Parse(std::string(u8"0.6"));

    // Assert
    EXPECT_EQ(referenceConstant1, *expression1);
    EXPECT_EQ(referenceConstant2, *expression2);
}

TEST(BackendTest, ParserShallParseImaginaryNumber)
{
    using namespace std::complex_literals;

    // Arrange
    Backend::Parser parser(false);
    Backend::Constant referenceConstant1(-0.4i);
    Backend::Constant referenceConstant2(0.6i);

    // Act
    auto expression1 = parser.Parse(std::string(u8"-0.4i"));
    auto expression2 = parser.Parse(std::string(u8"0.6I"));

    // Assert
    EXPECT_EQ(referenceConstant1, *expression1);
    EXPECT_EQ(referenceConstant2, *expression2);
}

TEST(BackendTest, ParserShallParseSum)
{
    using namespace std::complex_literals;

    // Arrange
    Backend::Parser parser(false);
    auto constant = std::make_shared<Backend::Constant>(0.4i);
    auto z = std::make_shared<Backend::BaseZ>();
    auto expected1 = std::make_shared<Backend::Sum>(std::vector<Backend::Sum::Summand>({Backend::Sum::Summand(Backend::Sum::Sign::Plus, z), Backend::Sum::Summand(Backend::Sum::Sign::Plus, constant)}));
    auto expected2 = std::make_shared<Backend::Sum>(std::vector<Backend::Sum::Summand>({Backend::Sum::Summand(Backend::Sum::Sign::Plus, z), Backend::Sum::Summand(Backend::Sum::Sign::Minus, constant)}));

    // Act
    auto expression1 = parser.Parse(std::string(u8"0.4i+Z"));
    auto expression2 = parser.Parse(std::string(u8"z-0.4i"));

    // Assert
    EXPECT_EQ(*expected1, *expression1);
    EXPECT_EQ(*expected2, *expression2);
}

TEST(BackendTest, ParserShallParseProduct)
{
    using namespace std::complex_literals;

    // Arrange
    Backend::Parser parser(false);
    auto constant = std::make_shared<Backend::Constant>(-0.4i);
    auto z = std::make_shared<Backend::BaseZ>();
    auto expected1 = std::make_shared<Backend::Product>(std::vector<Backend::Product::Factor>({Backend::Product::Factor(Backend::Product::Exponent::Positive, z), Backend::Product::Factor(Backend::Product::Exponent::Positive, constant)}));
    auto expected2 = std::make_shared<Backend::Product>(std::vector<Backend::Product::Factor>({Backend::Product::Factor(Backend::Product::Exponent::Positive, z), Backend::Product::Factor(Backend::Product::Exponent::Negative, constant)}));

    // Act
    auto expression1 = parser.Parse(std::string(u8"-0.4i*Z"));
    auto expression2 = parser.Parse(std::string(u8"z/(-0.4i)"));

    // Assert
    EXPECT_EQ(*expected1, *expression1);
    EXPECT_EQ(*expected2, *expression2);
}

TEST(BackendTest, ParserShallParseComposite)
{
    using namespace std::complex_literals;

    // Arrange
    Backend::Parser parser(false);
    auto constant = std::make_shared<Backend::Constant>(0.4i);
    auto z = std::make_shared<Backend::BaseZ>();

    auto sum1 = std::make_shared<Backend::Sum>(std::vector<Backend::Sum::Summand>({Backend::Sum::Summand(Backend::Sum::Sign::Plus, z), Backend::Sum::Summand(Backend::Sum::Sign::Plus, constant)}));
    auto product1 = std::make_shared<Backend::Product>(std::vector<Backend::Product::Factor>({Backend::Product::Factor(Backend::Product::Exponent::Positive, z), Backend::Product::Factor(Backend::Product::Exponent::Positive, sum1)}));

    auto product2 = std::make_shared<Backend::Product>(std::vector<Backend::Product::Factor>({Backend::Product::Factor(Backend::Product::Exponent::Positive, z), Backend::Product::Factor(Backend::Product::Exponent::Positive, z)}));
    auto sum2 = std::make_shared<Backend::Sum>(std::vector<Backend::Sum::Summand>({Backend::Sum::Summand(Backend::Sum::Sign::Plus, product2), Backend::Sum::Summand(Backend::Sum::Sign::Minus, constant)}));

    // Act
    auto expression1 = parser.Parse(std::string(u8"Z*(0.4i+Z)"));
    auto expression2 = parser.Parse(std::string(u8"Z*z-0.4i"));

    // Assert
    EXPECT_EQ(*product1, *expression1);
    EXPECT_EQ(*sum2, *expression2);
}

struct TestConstantParsing
{
    std::string input;
    bool optimize;
    std::shared_ptr<Backend::Expression> expected;
    friend std::ostream& operator<<(std::ostream& os, const TestConstantParsing& obj)
    {
        return os << u8"input: " << obj.input;
    }
};

class ConstantParsingTest : public testing::TestWithParam<TestConstantParsing>
{
};

using namespace std::complex_literals;

INSTANTIATE_TEST_SUITE_P(BackendTest, ConstantParsingTest, // clazy:exclude=non-pod-global-static //NOLINT (cert-err58-cpp, cppcoreguidelines-avoid-non-const-global-variables, fuchsia-statically-constructed-objects)
    testing::Values(
    // indifferent to optimization
    TestConstantParsing{u8"2.0", false, std::make_shared<Backend::Constant>(2.0)},
    TestConstantParsing{u8"2", false, std::make_shared<Backend::Constant>(2.0)},
    TestConstantParsing{u8"+2.0", false, std::make_shared<Backend::Constant>(2.0)},
    TestConstantParsing{u8"+2", false, std::make_shared<Backend::Constant>(2.0)},
    TestConstantParsing{u8"-2.0", false, std::make_shared<Backend::Constant>(-2.0)},
    TestConstantParsing{u8"-2", false, std::make_shared<Backend::Constant>(-2.0)},
    TestConstantParsing{u8"3.0i", false, std::make_shared<Backend::Constant>(3.0i)},
    TestConstantParsing{u8"3i", false, std::make_shared<Backend::Constant>(3.0i)},
    TestConstantParsing{u8"+3.0i", false, std::make_shared<Backend::Constant>(3.0i)},
    TestConstantParsing{u8"+3i", false, std::make_shared<Backend::Constant>(3.0i)},
    TestConstantParsing{u8"-3.0i", false, std::make_shared<Backend::Constant>(-3.0i)},
    TestConstantParsing{u8"-3i", false, std::make_shared<Backend::Constant>(-3.0i)},
    // non-optimized
    TestConstantParsing{u8"3.0*i", false, std::make_shared<Backend::Product>(std::vector<Backend::Product::Factor>({
                            Backend::Product::Factor(Backend::Product::Exponent::Positive, std::make_shared<Backend::Constant>(3.0)),
                            Backend::Product::Factor(Backend::Product::Exponent::Positive, std::make_shared<Backend::Constant>(1.0i))}))},
    TestConstantParsing{u8"3*i", false, std::make_shared<Backend::Product>(std::vector<Backend::Product::Factor>({
                            Backend::Product::Factor(Backend::Product::Exponent::Positive, std::make_shared<Backend::Constant>(3.0)),
                            Backend::Product::Factor(Backend::Product::Exponent::Positive, std::make_shared<Backend::Constant>(1.0i))}))},
    TestConstantParsing{u8"+3.0*i", false, std::make_shared<Backend::Product>(std::vector<Backend::Product::Factor>({
                            Backend::Product::Factor(Backend::Product::Exponent::Positive, std::make_shared<Backend::Constant>(3.0)),
                            Backend::Product::Factor(Backend::Product::Exponent::Positive, std::make_shared<Backend::Constant>(1.0i))}))},
    TestConstantParsing{u8"+3*i", false, std::make_shared<Backend::Product>(std::vector<Backend::Product::Factor>({
                            Backend::Product::Factor(Backend::Product::Exponent::Positive, std::make_shared<Backend::Constant>(3.0)),
                            Backend::Product::Factor(Backend::Product::Exponent::Positive, std::make_shared<Backend::Constant>(1.0i))}))},
    TestConstantParsing{u8"-3.0*i", false, std::make_shared<Backend::Product>(std::vector<Backend::Product::Factor>({
                            Backend::Product::Factor(Backend::Product::Exponent::Positive, std::make_shared<Backend::Constant>(-3.0)),
                            Backend::Product::Factor(Backend::Product::Exponent::Positive, std::make_shared<Backend::Constant>(1.0i))}))},
    TestConstantParsing{u8"-3*i", false, std::make_shared<Backend::Product>(std::vector<Backend::Product::Factor>({
                            Backend::Product::Factor(Backend::Product::Exponent::Positive, std::make_shared<Backend::Constant>(-3.0)),
                            Backend::Product::Factor(Backend::Product::Exponent::Positive, std::make_shared<Backend::Constant>(1.0i))}))},
    TestConstantParsing{u8"2.0+3.0i", false, std::make_shared<Backend::Sum>(std::vector<Backend::Sum::Summand>({
                            Backend::Sum::Summand(Backend::Sum::Sign::Plus, std::make_shared<Backend::Constant>(2.0)),
                            Backend::Sum::Summand(Backend::Sum::Sign::Plus, std::make_shared<Backend::Constant>(3.0i))}))},
    TestConstantParsing{u8"2+3.0i", false, std::make_shared<Backend::Sum>(std::vector<Backend::Sum::Summand>({
                            Backend::Sum::Summand(Backend::Sum::Sign::Plus, std::make_shared<Backend::Constant>(2.0)),
                            Backend::Sum::Summand(Backend::Sum::Sign::Plus, std::make_shared<Backend::Constant>(3.0i))}))},
    TestConstantParsing{u8"2.0+3i", false, std::make_shared<Backend::Sum>(std::vector<Backend::Sum::Summand>({
                            Backend::Sum::Summand(Backend::Sum::Sign::Plus, std::make_shared<Backend::Constant>(2.0)),
                            Backend::Sum::Summand(Backend::Sum::Sign::Plus, std::make_shared<Backend::Constant>(3.0i))}))},
    TestConstantParsing{u8"2+3i", false, std::make_shared<Backend::Sum>(std::vector<Backend::Sum::Summand>({
                            Backend::Sum::Summand(Backend::Sum::Sign::Plus, std::make_shared<Backend::Constant>(2.0)),
                            Backend::Sum::Summand(Backend::Sum::Sign::Plus, std::make_shared<Backend::Constant>(3.0i))}))},
    TestConstantParsing{u8"+2.0+3.0i", false, std::make_shared<Backend::Sum>(std::vector<Backend::Sum::Summand>({
                            Backend::Sum::Summand(Backend::Sum::Sign::Plus, std::make_shared<Backend::Constant>(2.0)),
                            Backend::Sum::Summand(Backend::Sum::Sign::Plus, std::make_shared<Backend::Constant>(3.0i))}))},
    TestConstantParsing{u8"+2+3.0i", false, std::make_shared<Backend::Sum>(std::vector<Backend::Sum::Summand>({
                            Backend::Sum::Summand(Backend::Sum::Sign::Plus, std::make_shared<Backend::Constant>(2.0)),
                            Backend::Sum::Summand(Backend::Sum::Sign::Plus, std::make_shared<Backend::Constant>(3.0i))}))},
    TestConstantParsing{u8"+2.0+3i", false, std::make_shared<Backend::Sum>(std::vector<Backend::Sum::Summand>({
                            Backend::Sum::Summand(Backend::Sum::Sign::Plus, std::make_shared<Backend::Constant>(2.0)),
                            Backend::Sum::Summand(Backend::Sum::Sign::Plus, std::make_shared<Backend::Constant>(3.0i))}))},
    TestConstantParsing{u8"+2+3i", false, std::make_shared<Backend::Sum>(std::vector<Backend::Sum::Summand>({
                            Backend::Sum::Summand(Backend::Sum::Sign::Plus, std::make_shared<Backend::Constant>(2.0)),
                            Backend::Sum::Summand(Backend::Sum::Sign::Plus, std::make_shared<Backend::Constant>(3.0i))}))},
    TestConstantParsing{u8"-2.0+3.0i", false, std::make_shared<Backend::Sum>(std::vector<Backend::Sum::Summand>({
                            Backend::Sum::Summand(Backend::Sum::Sign::Plus, std::make_shared<Backend::Constant>(-2.0)),
                            Backend::Sum::Summand(Backend::Sum::Sign::Plus, std::make_shared<Backend::Constant>(3.0i))}))},
    TestConstantParsing{u8"-2+3.0i", false, std::make_shared<Backend::Sum>(std::vector<Backend::Sum::Summand>({
                            Backend::Sum::Summand(Backend::Sum::Sign::Plus, std::make_shared<Backend::Constant>(-2.0)),
                            Backend::Sum::Summand(Backend::Sum::Sign::Plus, std::make_shared<Backend::Constant>(3.0i))}))},
    TestConstantParsing{u8"-2.0+3i", false, std::make_shared<Backend::Sum>(std::vector<Backend::Sum::Summand>({
                            Backend::Sum::Summand(Backend::Sum::Sign::Plus, std::make_shared<Backend::Constant>(-2.0)),
                            Backend::Sum::Summand(Backend::Sum::Sign::Plus, std::make_shared<Backend::Constant>(3.0i))}))},
    TestConstantParsing{u8"-2+3i", false, std::make_shared<Backend::Sum>(std::vector<Backend::Sum::Summand>({
                            Backend::Sum::Summand(Backend::Sum::Sign::Plus, std::make_shared<Backend::Constant>(-2.0)),
                            Backend::Sum::Summand(Backend::Sum::Sign::Plus, std::make_shared<Backend::Constant>(3.0i))}))},
    TestConstantParsing{u8"2.0-3.0i", false, std::make_shared<Backend::Sum>(std::vector<Backend::Sum::Summand>({
                            Backend::Sum::Summand(Backend::Sum::Sign::Plus, std::make_shared<Backend::Constant>(2.0)),
                            Backend::Sum::Summand(Backend::Sum::Sign::Minus, std::make_shared<Backend::Constant>(3.0i))}))},
    TestConstantParsing{u8"2-3.0i", false, std::make_shared<Backend::Sum>(std::vector<Backend::Sum::Summand>({
                            Backend::Sum::Summand(Backend::Sum::Sign::Plus, std::make_shared<Backend::Constant>(2.0)),
                            Backend::Sum::Summand(Backend::Sum::Sign::Minus, std::make_shared<Backend::Constant>(3.0i))}))},
    TestConstantParsing{u8"2.0-3i", false, std::make_shared<Backend::Sum>(std::vector<Backend::Sum::Summand>({
                            Backend::Sum::Summand(Backend::Sum::Sign::Plus, std::make_shared<Backend::Constant>(2.0)),
                            Backend::Sum::Summand(Backend::Sum::Sign::Minus, std::make_shared<Backend::Constant>(3.0i))}))},
    TestConstantParsing{u8"2-3i", false, std::make_shared<Backend::Sum>(std::vector<Backend::Sum::Summand>({
                            Backend::Sum::Summand(Backend::Sum::Sign::Plus, std::make_shared<Backend::Constant>(2.0)),
                            Backend::Sum::Summand(Backend::Sum::Sign::Minus, std::make_shared<Backend::Constant>(3.0i))}))},
    TestConstantParsing{u8"+2.0-3.0i", false, std::make_shared<Backend::Sum>(std::vector<Backend::Sum::Summand>({
                            Backend::Sum::Summand(Backend::Sum::Sign::Plus, std::make_shared<Backend::Constant>(2.0)),
                            Backend::Sum::Summand(Backend::Sum::Sign::Minus, std::make_shared<Backend::Constant>(3.0i))}))},
    TestConstantParsing{u8"+2-3.0i", false, std::make_shared<Backend::Sum>(std::vector<Backend::Sum::Summand>({
                            Backend::Sum::Summand(Backend::Sum::Sign::Plus, std::make_shared<Backend::Constant>(2.0)),
                            Backend::Sum::Summand(Backend::Sum::Sign::Minus, std::make_shared<Backend::Constant>(3.0i))}))},
    TestConstantParsing{u8"+2.0-3i", false, std::make_shared<Backend::Sum>(std::vector<Backend::Sum::Summand>({
                            Backend::Sum::Summand(Backend::Sum::Sign::Plus, std::make_shared<Backend::Constant>(2.0)),
                            Backend::Sum::Summand(Backend::Sum::Sign::Minus, std::make_shared<Backend::Constant>(3.0i))}))},
    TestConstantParsing{u8"+2-3i", false, std::make_shared<Backend::Sum>(std::vector<Backend::Sum::Summand>({
                            Backend::Sum::Summand(Backend::Sum::Sign::Plus, std::make_shared<Backend::Constant>(2.0)),
                            Backend::Sum::Summand(Backend::Sum::Sign::Minus, std::make_shared<Backend::Constant>(3.0i))}))},
    TestConstantParsing{u8"-2.0-3.0i", false, std::make_shared<Backend::Sum>(std::vector<Backend::Sum::Summand>({
                            Backend::Sum::Summand(Backend::Sum::Sign::Plus, std::make_shared<Backend::Constant>(-2.0)),
                            Backend::Sum::Summand(Backend::Sum::Sign::Minus, std::make_shared<Backend::Constant>(3.0i))}))},
    TestConstantParsing{u8"-2-3.0i", false, std::make_shared<Backend::Sum>(std::vector<Backend::Sum::Summand>({
                            Backend::Sum::Summand(Backend::Sum::Sign::Plus, std::make_shared<Backend::Constant>(-2.0)),
                            Backend::Sum::Summand(Backend::Sum::Sign::Minus, std::make_shared<Backend::Constant>(3.0i))}))},
    TestConstantParsing{u8"-2.0-3i", false, std::make_shared<Backend::Sum>(std::vector<Backend::Sum::Summand>({
                            Backend::Sum::Summand(Backend::Sum::Sign::Plus, std::make_shared<Backend::Constant>(-2.0)),
                            Backend::Sum::Summand(Backend::Sum::Sign::Minus, std::make_shared<Backend::Constant>(3.0i))}))},
    TestConstantParsing{u8"-2-3i", false, std::make_shared<Backend::Sum>(std::vector<Backend::Sum::Summand>({
                            Backend::Sum::Summand(Backend::Sum::Sign::Plus, std::make_shared<Backend::Constant>(-2.0)),
                            Backend::Sum::Summand(Backend::Sum::Sign::Minus, std::make_shared<Backend::Constant>(3.0i))}))},
    TestConstantParsing{u8"2.0+i", false, std::make_shared<Backend::Sum>(std::vector<Backend::Sum::Summand>({
                            Backend::Sum::Summand(Backend::Sum::Sign::Plus, std::make_shared<Backend::Constant>(2.0)),
                            Backend::Sum::Summand(Backend::Sum::Sign::Plus, std::make_shared<Backend::Constant>(1.0i))}))},
    TestConstantParsing{u8"2+i", false, std::make_shared<Backend::Sum>(std::vector<Backend::Sum::Summand>({
                            Backend::Sum::Summand(Backend::Sum::Sign::Plus, std::make_shared<Backend::Constant>(2.0)),
                            Backend::Sum::Summand(Backend::Sum::Sign::Plus, std::make_shared<Backend::Constant>(1.0i))}))},
    TestConstantParsing{u8"2.0-i", false, std::make_shared<Backend::Sum>(std::vector<Backend::Sum::Summand>({
                            Backend::Sum::Summand(Backend::Sum::Sign::Plus, std::make_shared<Backend::Constant>(2.0)),
                            Backend::Sum::Summand(Backend::Sum::Sign::Minus, std::make_shared<Backend::Constant>(1.0i))}))},
    TestConstantParsing{u8"2-i", false, std::make_shared<Backend::Sum>(std::vector<Backend::Sum::Summand>({
                            Backend::Sum::Summand(Backend::Sum::Sign::Plus, std::make_shared<Backend::Constant>(2.0)),
                            Backend::Sum::Summand(Backend::Sum::Sign::Minus, std::make_shared<Backend::Constant>(1.0i))}))},
    TestConstantParsing{u8"+2.0+i", false, std::make_shared<Backend::Sum>(std::vector<Backend::Sum::Summand>({
                            Backend::Sum::Summand(Backend::Sum::Sign::Plus, std::make_shared<Backend::Constant>(2.0)),
                            Backend::Sum::Summand(Backend::Sum::Sign::Plus, std::make_shared<Backend::Constant>(1.0i))}))},
    TestConstantParsing{u8"+2+i", false, std::make_shared<Backend::Sum>(std::vector<Backend::Sum::Summand>({
                            Backend::Sum::Summand(Backend::Sum::Sign::Plus, std::make_shared<Backend::Constant>(2.0)),
                            Backend::Sum::Summand(Backend::Sum::Sign::Plus, std::make_shared<Backend::Constant>(1.0i))}))},
    TestConstantParsing{u8"+2.0-i", false, std::make_shared<Backend::Sum>(std::vector<Backend::Sum::Summand>({
                            Backend::Sum::Summand(Backend::Sum::Sign::Plus, std::make_shared<Backend::Constant>(2.0)),
                            Backend::Sum::Summand(Backend::Sum::Sign::Minus, std::make_shared<Backend::Constant>(1.0i))}))},
    TestConstantParsing{u8"+2-i", false, std::make_shared<Backend::Sum>(std::vector<Backend::Sum::Summand>({
                            Backend::Sum::Summand(Backend::Sum::Sign::Plus, std::make_shared<Backend::Constant>(2.0)),
                            Backend::Sum::Summand(Backend::Sum::Sign::Minus, std::make_shared<Backend::Constant>(1.0i))}))},
    TestConstantParsing{u8"-2.0+i", false, std::make_shared<Backend::Sum>(std::vector<Backend::Sum::Summand>({
                            Backend::Sum::Summand(Backend::Sum::Sign::Plus, std::make_shared<Backend::Constant>(-2.0)),
                            Backend::Sum::Summand(Backend::Sum::Sign::Plus, std::make_shared<Backend::Constant>(1.0i))}))},
    TestConstantParsing{u8"-2+i", false, std::make_shared<Backend::Sum>(std::vector<Backend::Sum::Summand>({
                            Backend::Sum::Summand(Backend::Sum::Sign::Plus, std::make_shared<Backend::Constant>(-2.0)),
                            Backend::Sum::Summand(Backend::Sum::Sign::Plus, std::make_shared<Backend::Constant>(1.0i))}))},
    TestConstantParsing{u8"-2.0-i", false, std::make_shared<Backend::Sum>(std::vector<Backend::Sum::Summand>({
                            Backend::Sum::Summand(Backend::Sum::Sign::Plus, std::make_shared<Backend::Constant>(-2.0)),
                            Backend::Sum::Summand(Backend::Sum::Sign::Minus, std::make_shared<Backend::Constant>(1.0i))}))},
    TestConstantParsing{u8"-2-i", false, std::make_shared<Backend::Sum>(std::vector<Backend::Sum::Summand>({
                            Backend::Sum::Summand(Backend::Sum::Sign::Plus, std::make_shared<Backend::Constant>(-2.0)),
                            Backend::Sum::Summand(Backend::Sum::Sign::Minus, std::make_shared<Backend::Constant>(1.0i))}))},
    TestConstantParsing{u8"3.0+2.0*i", false, std::make_shared<Backend::Sum>(std::vector<Backend::Sum::Summand>({
                            Backend::Sum::Summand(Backend::Sum::Sign::Plus, std::make_shared<Backend::Constant>(3.0)),
                            Backend::Sum::Summand(Backend::Sum::Sign::Plus, std::make_shared<Backend::Product>(std::vector<Backend::Product::Factor>({Backend::Product::Factor(Backend::Product::Exponent::Positive, std::make_shared<Backend::Constant>(2.0)), Backend::Product::Factor(Backend::Product::Exponent::Positive, std::make_shared<Backend::Constant>(1.0i))})))}))},
    TestConstantParsing{u8"3+2*i", false, std::make_shared<Backend::Sum>(std::vector<Backend::Sum::Summand>({
                            Backend::Sum::Summand(Backend::Sum::Sign::Plus, std::make_shared<Backend::Constant>(3.0)),
                            Backend::Sum::Summand(Backend::Sum::Sign::Plus, std::make_shared<Backend::Product>(std::vector<Backend::Product::Factor>({Backend::Product::Factor(Backend::Product::Exponent::Positive, std::make_shared<Backend::Constant>(2.0)), Backend::Product::Factor(Backend::Product::Exponent::Positive, std::make_shared<Backend::Constant>(1.0i))})))}))},
    TestConstantParsing{u8"+3.0+2.0*i", false, std::make_shared<Backend::Sum>(std::vector<Backend::Sum::Summand>({
                            Backend::Sum::Summand(Backend::Sum::Sign::Plus, std::make_shared<Backend::Constant>(3.0)),
                            Backend::Sum::Summand(Backend::Sum::Sign::Plus, std::make_shared<Backend::Product>(std::vector<Backend::Product::Factor>({Backend::Product::Factor(Backend::Product::Exponent::Positive, std::make_shared<Backend::Constant>(2.0)), Backend::Product::Factor(Backend::Product::Exponent::Positive, std::make_shared<Backend::Constant>(1.0i))})))}))},
    TestConstantParsing{u8"+3+2*i", false, std::make_shared<Backend::Sum>(std::vector<Backend::Sum::Summand>({
                            Backend::Sum::Summand(Backend::Sum::Sign::Plus, std::make_shared<Backend::Constant>(3.0)),
                            Backend::Sum::Summand(Backend::Sum::Sign::Plus, std::make_shared<Backend::Product>(std::vector<Backend::Product::Factor>({Backend::Product::Factor(Backend::Product::Exponent::Positive, std::make_shared<Backend::Constant>(2.0)), Backend::Product::Factor(Backend::Product::Exponent::Positive, std::make_shared<Backend::Constant>(1.0i))})))}))},
    TestConstantParsing{u8"3.0-2.0*i", false, std::make_shared<Backend::Sum>(std::vector<Backend::Sum::Summand>({
                            Backend::Sum::Summand(Backend::Sum::Sign::Plus, std::make_shared<Backend::Constant>(3.0)),
                            Backend::Sum::Summand(Backend::Sum::Sign::Minus, std::make_shared<Backend::Product>(std::vector<Backend::Product::Factor>({Backend::Product::Factor(Backend::Product::Exponent::Positive, std::make_shared<Backend::Constant>(2.0)), Backend::Product::Factor(Backend::Product::Exponent::Positive, std::make_shared<Backend::Constant>(1.0i))})))}))},
    TestConstantParsing{u8"3-2*i", false, std::make_shared<Backend::Sum>(std::vector<Backend::Sum::Summand>({
                            Backend::Sum::Summand(Backend::Sum::Sign::Plus, std::make_shared<Backend::Constant>(3.0)),
                            Backend::Sum::Summand(Backend::Sum::Sign::Minus, std::make_shared<Backend::Product>(std::vector<Backend::Product::Factor>({Backend::Product::Factor(Backend::Product::Exponent::Positive, std::make_shared<Backend::Constant>(2.0)), Backend::Product::Factor(Backend::Product::Exponent::Positive, std::make_shared<Backend::Constant>(1.0i))})))}))},
    TestConstantParsing{u8"+3.0-2.0*i", false, std::make_shared<Backend::Sum>(std::vector<Backend::Sum::Summand>({
                            Backend::Sum::Summand(Backend::Sum::Sign::Plus, std::make_shared<Backend::Constant>(3.0)),
                            Backend::Sum::Summand(Backend::Sum::Sign::Minus, std::make_shared<Backend::Product>(std::vector<Backend::Product::Factor>({Backend::Product::Factor(Backend::Product::Exponent::Positive, std::make_shared<Backend::Constant>(2.0)), Backend::Product::Factor(Backend::Product::Exponent::Positive, std::make_shared<Backend::Constant>(1.0i))})))}))},
    TestConstantParsing{u8"+3-2*i", false, std::make_shared<Backend::Sum>(std::vector<Backend::Sum::Summand>({
                            Backend::Sum::Summand(Backend::Sum::Sign::Plus, std::make_shared<Backend::Constant>(3.0)),
                            Backend::Sum::Summand(Backend::Sum::Sign::Minus, std::make_shared<Backend::Product>(std::vector<Backend::Product::Factor>({Backend::Product::Factor(Backend::Product::Exponent::Positive, std::make_shared<Backend::Constant>(2.0)), Backend::Product::Factor(Backend::Product::Exponent::Positive, std::make_shared<Backend::Constant>(1.0i))})))}))},
    TestConstantParsing{u8"-3.0+2.0*i", false, std::make_shared<Backend::Sum>(std::vector<Backend::Sum::Summand>({
                            Backend::Sum::Summand(Backend::Sum::Sign::Plus, std::make_shared<Backend::Constant>(-3.0)),
                            Backend::Sum::Summand(Backend::Sum::Sign::Plus, std::make_shared<Backend::Product>(std::vector<Backend::Product::Factor>({Backend::Product::Factor(Backend::Product::Exponent::Positive, std::make_shared<Backend::Constant>(2.0)), Backend::Product::Factor(Backend::Product::Exponent::Positive, std::make_shared<Backend::Constant>(1.0i))})))}))},
    TestConstantParsing{u8"-3+2*i", false, std::make_shared<Backend::Sum>(std::vector<Backend::Sum::Summand>({
                            Backend::Sum::Summand(Backend::Sum::Sign::Plus, std::make_shared<Backend::Constant>(-3.0)),
                            Backend::Sum::Summand(Backend::Sum::Sign::Plus, std::make_shared<Backend::Product>(std::vector<Backend::Product::Factor>({Backend::Product::Factor(Backend::Product::Exponent::Positive, std::make_shared<Backend::Constant>(2.0)), Backend::Product::Factor(Backend::Product::Exponent::Positive, std::make_shared<Backend::Constant>(1.0i))})))}))},
    TestConstantParsing{u8"-3.0-2.0*i", false, std::make_shared<Backend::Sum>(std::vector<Backend::Sum::Summand>({
                            Backend::Sum::Summand(Backend::Sum::Sign::Plus, std::make_shared<Backend::Constant>(-3.0)),
                            Backend::Sum::Summand(Backend::Sum::Sign::Minus, std::make_shared<Backend::Product>(std::vector<Backend::Product::Factor>({Backend::Product::Factor(Backend::Product::Exponent::Positive, std::make_shared<Backend::Constant>(2.0)), Backend::Product::Factor(Backend::Product::Exponent::Positive, std::make_shared<Backend::Constant>(1.0i))})))}))},
    TestConstantParsing{u8"-3-2*i", false, std::make_shared<Backend::Sum>(std::vector<Backend::Sum::Summand>({
                            Backend::Sum::Summand(Backend::Sum::Sign::Plus, std::make_shared<Backend::Constant>(-3.0)),
                            Backend::Sum::Summand(Backend::Sum::Sign::Minus, std::make_shared<Backend::Product>(std::vector<Backend::Product::Factor>({Backend::Product::Factor(Backend::Product::Exponent::Positive, std::make_shared<Backend::Constant>(2.0)), Backend::Product::Factor(Backend::Product::Exponent::Positive, std::make_shared<Backend::Constant>(1.0i))})))}))},
    TestConstantParsing{u8"ii", false, nullptr}
));

TEST_P(ConstantParsingTest, ShallParseConstantsCorrectly) //NOLINT (cert-err58-cpp, cppcoreguidelines-avoid-non-const-global-variables, cppcoreguidelines-owning-memory, fuchsia-statically-constructed-objects, misc-definitions-in-headers)
{
    // Arrange
    TestConstantParsing tcp = GetParam();
    Backend::Parser parser(tcp.optimize);

    // Act
    auto result = parser.Parse(tcp.input);

    // Assert
    if (tcp.expected != nullptr)
    {
        ASSERT_TRUE(result);
        EXPECT_EQ(*(tcp.expected), *result);
    }
    else
    {
        ASSERT_FALSE(result);
    }
}

struct TestFunctionResult
{
    std::string testname;
    std::string text;
    bool expectedParseability;
    friend std::ostream& operator<<(std::ostream& os, const TestFunctionResult& obj)
    {
        return os
                << "testname: " << obj.testname
                << " text: " << obj.text
                << " expectedParseability: " << (obj.expectedParseability ? "true" : "false");
    }
};

class ParseabilityTest : public testing::TestWithParam<TestFunctionResult>
{
};

INSTANTIATE_TEST_SUITE_P(BackendTest, ParseabilityTest, // clazy:exclude=non-pod-global-static
    testing::Values(
    TestFunctionResult{"Z", "Z", true},
    TestFunctionResult{"Y", "Y", false},
    TestFunctionResult{"Number", " +030.500", true},
    TestFunctionResult{"Invalid number", "030.50.0", false},
    TestFunctionResult{"Braced Z", "(Z)", true},
    TestFunctionResult{"Double braced Z", "((Z))", true},
    TestFunctionResult{"Complete sum", "1+z+2", true},
    TestFunctionResult{"Incomplete sum", "1+z+", false},
    TestFunctionResult{"Complete product", "1/z", true},
    TestFunctionResult{"Incomplete product", "1/", false},
//    TestFunctionResult{"Complete power", "1^z", true},
//    TestFunctionResult{"Incomplete power", "1^", false},
    TestFunctionResult{"Malformed", "(z)z", false},
//    TestFunctionResult{"Function no argument", "sin()", false},
    TestFunctionResult{"Number with space", "1 .0", true},
    TestFunctionResult{"Double z 1", "z z", false},
    TestFunctionResult{"Double z 2", "zz", false},
    TestFunctionResult{"Braced double z", "(z z)", false},
    TestFunctionResult{"Open brace", "(", false},
    TestFunctionResult{"Close brace", ")", false},
    TestFunctionResult{"Just braces", "()", false},
    TestFunctionResult{"Missing operator", "4z", false},
    TestFunctionResult{"shadow01", "+1.1", true},
    TestFunctionResult{"shadow02", "-2.2", true},
    TestFunctionResult{"shadow03", "+z", true},
    TestFunctionResult{"shadow04", "-z", true},
    TestFunctionResult{"shadow05", "+2.0+3.0", true},
    TestFunctionResult{"shadow06", "-2.0-3.0", true},
    TestFunctionResult{"shadow07", "(2.0)+(3.0)", true},
    TestFunctionResult{"shadow08", "2.0+3.0-z", true},
    TestFunctionResult{"shadow09", "2.0-(3.0+z)+1.0", true},
    TestFunctionResult{"shadow10", "2.0*3.0", true},
    TestFunctionResult{"shadow11", "2.0/3.0", true},
    TestFunctionResult{"shadow12", "(2.0)*(3.0)", true},
    TestFunctionResult{"shadow13", "2.0*3.0/z", true},
    TestFunctionResult{"shadow14", "2.0/(3.0*z)*1.0", true},
    TestFunctionResult{"shadow15", "2.0*z+1.0", true},
    TestFunctionResult{"shadow16", "-2.0*z+1.0", true},
    TestFunctionResult{"shadow17", "-2.1*(z+3.1)+1.1", true},
    TestFunctionResult{"shadow18", "(z+3.1)*-2.1+1.1", true},
//    TestFunctionResult{"shadow19", "x^2.0", true},
//    TestFunctionResult{"shadow20", "x^(-2.0)", true},
//    TestFunctionResult{"shadow21", "x^-2.0", false},
//    TestFunctionResult{"shadow22", "2.0^x", true},
//    TestFunctionResult{"shadow23", "-(2.0^x)", true},
//    TestFunctionResult{"shadow24", "-((2.0*x)^(x+1.0))", true},
//    TestFunctionResult{"shadow25", "3.0^x^2.0", true},
//    TestFunctionResult{"shadow26", "cos(x+sin(x))", true},
//    TestFunctionResult{"shadow27", "abs(sin(cos(tan(exp(ln(x))))))", true},
//    TestFunctionResult{"shadow28", "cis(x+sin(x))", false},
//    TestFunctionResult{"shadow29", "z(x+sin(x))", false}
    TestFunctionResult{"Empty", "", false}
));

TEST_P(ParseabilityTest, CheckingForParseabilityShouldNotCrashAndYieldCorrectResult)
{
    // Arrange
    Backend::Parser parser(false);
    TestFunctionResult tfr = GetParam();

    // Act
    bool actualResult = {};

    if(tfr.text.empty())
    {
        actualResult = parser.IsParseable(tfr.text);
    }
    else
    {
#ifdef _SKIP_LONG_TEST
        if(tfr.text.length() < 9)
#else // _USE_LONG_TEST
        if(tfr.text.length() < 16)
#endif // _SKIP_LONG_TEST
        {
            // simulate typing - all subsets
            SubsetGenerator generator(tfr.text);
            while(generator.HasNext())
            {
                actualResult = parser.IsParseable(generator.GetNext());
            }
        }
        else
        {
            // simulate typing - linearly
            for(size_t i = 1; i <= tfr.text.length(); ++i)
            {
                auto substr = tfr.text.substr(0, i);
                actualResult = parser.IsParseable(substr);
            }
        }
    }

    // Assert
    EXPECT_EQ(tfr.expectedParseability, actualResult)
            << "testname: \"" << tfr.testname
            << "\" text: \"" << tfr.text
            << "\" expectedParseability: " << (tfr.expectedParseability ? "true" : "false");
}

#endif // TST_PARSER_H
