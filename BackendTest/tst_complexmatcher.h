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

#ifndef TST_COMPLEXMATCHER_H
#define TST_COMPLEXMATCHER_H

#include <gmock/gmock-matchers.h>
#include <gtest/gtest.h>

#include "ComplexMatcher.h"

TEST(BackendTest, ComplexMatcherShallWorkAsExpected)
{
    using namespace std::complex_literals;

    // Assert
    ASSERT_THAT(2.1+4.3i, COMPLEX_NEAR(2.1+4.3i));
    ASSERT_THAT(2.1+4.3i, testing::Not(COMPLEX_NEAR(2.1+5.3i)));
    ASSERT_THAT(2.1+4.3i, testing::Not(COMPLEX_NEAR(5.1+4.2i)));
}

#endif // TST_COMPLEXMATCHER_H
