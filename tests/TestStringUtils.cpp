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

#include <PyroCommon/Util/String.hpp>
#include <gtest/gtest.h>

using namespace PyroshockStudios;

TEST(TestStringUtils, Split_BasicSplit) {
    eastl::string cmd = "-arg=1 -foo=bar";
    auto args = StringSplit(cmd, ' ', '"');

    ASSERT_EQ(args.size(), 2u);
    EXPECT_STREQ(args[0].c_str(), "-arg=1");
    EXPECT_STREQ(args[1].c_str(), "-foo=bar");
}

TEST(TestStringUtils, Split_QuotedValue) {
    eastl::string cmd = R"(-arg="Hello World" -foo=bar)";
    auto args = StringSplit(cmd, ' ', '"');

    ASSERT_EQ(args.size(), 2u);
    EXPECT_STREQ(args[0].c_str(), R"(-arg="Hello World")");
    EXPECT_STREQ(args[1].c_str(), "-foo=bar");
}

TEST(TestStringUtils, Split_EscapedQuoteInside) {
    eastl::string cmd = R"(-arg=\"Hello\" -foo=bar)";
    auto args = StringSplit(cmd, ' ', '"');

    ASSERT_EQ(args.size(), 2u);
    EXPECT_STREQ(args[0].c_str(), R"(-arg="Hello")");
    EXPECT_STREQ(args[1].c_str(), "-foo=bar");
}

TEST(TestStringUtils, Split_MultipleSpacesBetweenArgs) {
    eastl::string cmd = "-arg=1    -foo=bar";
    auto args = StringSplit(cmd, ' ', '"');

    ASSERT_EQ(args.size(), 2u);
    EXPECT_STREQ(args[0].c_str(), "-arg=1");
    EXPECT_STREQ(args[1].c_str(), "-foo=bar");
}

TEST(TestStringUtils, Split_EmptyString) {
    eastl::string cmd = "";
    auto args = StringSplit(cmd, ' ', '"');
    EXPECT_TRUE(args.empty());
}

TEST(TestStringUtils, Split_OnlySpaces) {
    eastl::string cmd = "     ";
    auto args = StringSplit(cmd, ' ', '"');
    EXPECT_TRUE(args.empty());
}

TEST(TestStringUtils, Split_QuoteUnclosed) {
    eastl::string cmd = R"(-arg="Hello World -foo=bar)";
    auto args = StringSplit(cmd, ' ', '"');

    ASSERT_EQ(args.size(), 1u);
    EXPECT_STREQ(args[0].c_str(), cmd.c_str());
}

TEST(TestStringUtils, Split_RemoveEscape) {
    eastl::string cmd = R"(-arg=Hello -foo=\\"bar foo")";
    auto args = StringSplit(cmd, ' ', '"');

    ASSERT_EQ(args.size(), 2u);
    EXPECT_STREQ(args[0].c_str(), "-arg=Hello");
    EXPECT_STREQ(args[1].c_str(), R"(-foo=\"bar foo")");
}