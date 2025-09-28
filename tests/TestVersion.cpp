// MIT License
//
// Copyright (c) 2025 Pyroshock Studios
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.

#include <PyroCommon/Version.hpp>
#include <gtest/gtest.h>

using namespace PyroshockStudios;

TEST(TestVersion, ConstructAndAccessFields) {
    Version v(1, 2, 300, 20250726);

    EXPECT_EQ(v.Major(), 1);
    EXPECT_EQ(v.Minor(), 2);
    EXPECT_EQ(v.Revision(), 300);
    EXPECT_EQ(v.BuildDate(), 20250726);
}

TEST(TestVersion, CopyConstructor) {
    Version v1 = Version(1, 2, 300, 20250726);
    Version v2 = v1;
    EXPECT_EQ(v1, v2);
}

TEST(TestVersion, MoveConstructor) {
    Version v1 = Version(1, 2, 300, 20250726);
    Version expected = v1; // assuming copy constructor works

    Version v2 = eastl::move(v1);

    EXPECT_EQ(expected, v2);
}


TEST(TestVersion, ToStringOutput) {
    Version v(3, 5, 789, 20250101);
    EXPECT_EQ(v.ToString(), "3.5.789 (Build 20250101)");
}

TEST(TestVersion, EqualityComparison) {
    Version v1(1, 0, 0, 20240101);
    Version v2(1, 0, 0, 20240101);
    Version v3(1, 0, 0, 20240102);

    EXPECT_TRUE(v1 == v2);
    EXPECT_FALSE(v1 != v2);
    EXPECT_FALSE(v1 == v3);
    EXPECT_TRUE(v1 != v3);
}

TEST(TestVersion, RelationalComparison) {
    Version older(1, 2, 100, 20240101);
    Version newer(1, 2, 100, 20250101);

    EXPECT_LT(older, newer);
    EXPECT_LE(older, newer);
    EXPECT_GT(newer, older);
    EXPECT_GE(newer, older);
}

TEST(TestVersion, FullComparisonHierarchy) {
    Version v1(1, 0, 0, 20230101);
    Version v2(1, 1, 0, 20230101);
    Version v3(1, 1, 1, 20230101);
    Version v4(1, 1, 1, 20240101);

    EXPECT_LT(v1, v2);
    EXPECT_LT(v2, v3);
    EXPECT_LT(v3, v4);
}