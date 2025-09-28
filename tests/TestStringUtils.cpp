#include <SWCommon/Util/String.hpp>
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