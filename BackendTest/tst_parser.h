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
    friend std::ostream& operator<<(std::ostream& wos, const TestConstantParsing& obj)
    {
        return wos << u8"input: " << obj.input;
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

#endif // TST_PARSER_H
