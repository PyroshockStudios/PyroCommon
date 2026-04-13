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

#include <PyroCommon/Core.hpp>
#include <gtest/gtest.h>

using namespace PyroshockStudios;

struct TestFlagProperties {
    using Data = uint32_t;
};

using MyFlags = Flags<TestFlagProperties>;

TEST(TestFlags, BitwiseOr) {
    MyFlags f1{1}; // 0001
    MyFlags f2{2}; // 0010

    MyFlags result = f1 | f2;
    EXPECT_EQ(result.data, 3); // 0011

    f1 |= f2;
    EXPECT_EQ(f1.data, 3);
}

TEST(TestFlags, BitwiseAnd) {
    MyFlags f1{3}; // 0011
    MyFlags f2{2}; // 0010

    MyFlags result = f1 & f2;
    EXPECT_EQ(result.data, 2); // 0010

    f1 &= f2;
    EXPECT_EQ(f1.data, 2);
}

TEST(TestFlags, BitwiseXor) {
    MyFlags f1{3}; // 0011
    MyFlags f2{5}; // 0101

    MyFlags result = f1 ^ f2;
    EXPECT_EQ(result.data, 6); // 0110

    f1 ^= f2;
    EXPECT_EQ(f1.data, 6);
}

TEST(TestFlags, BitwiseNot) {
    MyFlags f1{1};
    MyFlags result = ~f1;
    EXPECT_EQ(result.data, ~1u);
}

TEST(TestFlags, Comparison) {
    MyFlags f1{5};
    MyFlags f2{5};
    MyFlags f3{4};

    EXPECT_TRUE(f1 == f2);
    EXPECT_FALSE(f1 == f3);
    EXPECT_TRUE(f3 < f1);
}

TEST(TestFlags, Contains) {
    MyFlags f1{5};
    MyFlags f2{1};

    EXPECT_TRUE(f1 & f2);
}
TEST(TestFlags, NoContains) {
    MyFlags f1{5};
    MyFlags f2{2};

    EXPECT_FALSE(f1 & f2);
}