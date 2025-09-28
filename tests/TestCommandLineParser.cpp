#include <SWCommon/CommandLineParser.hpp>
#include <gtest/gtest.h>

using namespace PyroshockStudios;
using namespace PyroshockStudios::Types;

// Helper to simulate command-line input
class CommandLineParserTest : public ::testing::Test {
protected:
    static void BuildArgv(const std::vector<eastl::string>& args, i32& argc, char**& argv) {
        argc = static_cast<i32>(args.size());
        argvStorage.clear();
        argvPointers.clear();

        for (const auto& arg : args) {
            argvStorage.push_back(eastl::string(arg));
        }

        for (auto& str : argvStorage) {
            argvPointers.push_back(const_cast<char*>(str.c_str()));
        }

        argv = argvPointers.data();
    }

    static std::vector<eastl::string> argvStorage;
    static std::vector<char*> argvPointers;
};

std::vector<eastl::string> CommandLineParserTest::argvStorage;
std::vector<char*> CommandLineParserTest::argvPointers;

// ---- Tests ---- //

TEST_F(CommandLineParserTest, ParsesSimpleKeyValue) {
    int argc;
    char** argv;
    BuildArgv({ "App.exe", "-rhi=vk13" }, argc, argv);
    CommandLineParser parser(argc, argv);

    EXPECT_EQ(parser.GetOption("rhi"), "vk13");
}

TEST_F(CommandLineParserTest, HandlesQuotedValue) {
    int argc;
    char** argv;
    BuildArgv({ "App.exe", "-rhi-options=\"opt1 opt2\"" }, argc, argv);
    CommandLineParser parser(argc, argv);

    EXPECT_EQ(parser.GetOption("rhi-options"), "opt1 opt2");
}

TEST_F(CommandLineParserTest, HandlesFlagWithoutValue) {
    int argc;
    char** argv;
    BuildArgv({ "App.exe", "-debug" }, argc, argv);
    CommandLineParser parser(argc, argv);

    EXPECT_TRUE(parser.HasOption("debug"));
    EXPECT_EQ(parser.GetOption("debug"), "");
}

TEST_F(CommandLineParserTest, ReturnsDefaultValueIfMissing) {
    int argc;
    char** argv;
    BuildArgv({ "App.exe" }, argc, argv);
    CommandLineParser parser(argc, argv);

    EXPECT_EQ(parser.GetOption("rhi", "defaultRHI"), "defaultRHI");
}

TEST_F(CommandLineParserTest, ParsesBooleanTrueVariants) {
    std::vector<eastl::string> trueVariants = { "true", "1", "yes", "on" };

    for (const auto& val : trueVariants) {
        int argc;
        char** argv;
        BuildArgv({ "App.exe", ("-tracy=" + val).c_str() }, argc, argv);
        CommandLineParser parser(argc, argv);
        EXPECT_TRUE(parser.GetBoolOption("tracy")) << "Failed on input: " << val.c_str();
    }
}


TEST_F(CommandLineParserTest, ParsesBooleanFalseDefault) {
    int argc;
    char** argv;
    BuildArgv({ "App.exe" }, argc, argv);
    CommandLineParser parser(argc, argv);

    EXPECT_FALSE(parser.GetBoolOption("tracy")); // no tracy flag, should be false
}

TEST_F(CommandLineParserTest, ParsesBooleanFalseExplicit) {
    int argc;
    char** argv;
    BuildArgv({ "App.exe", "-tracy=false" }, argc, argv);
    CommandLineParser parser(argc, argv);

    EXPECT_FALSE(parser.GetBoolOption("tracy", true)); // should override true default
}

TEST_F(CommandLineParserTest, IgnoresArgumentsWithoutDash) {
    int argc;
    char** argv;
    BuildArgv({ "App.exe", "rhi=vk13" }, argc, argv); // Missing leading '-'
    CommandLineParser parser(argc, argv);

    EXPECT_FALSE(parser.HasOption("rhi"));
    EXPECT_EQ(parser.GetOption("rhi"), "");
}

TEST_F(CommandLineParserTest, HandlesEmptyArguments) {
    int argc;
    char** argv;
    BuildArgv({ "App.exe", "-" }, argc, argv); // Just a dash
    CommandLineParser parser(argc, argv);

    EXPECT_FALSE(parser.HasOption(""));
}

TEST_F(CommandLineParserTest, HandlesKeyWithNoEqualSign) {
    int argc;
    char** argv;
    BuildArgv({ "App.exe", "-justakey" }, argc, argv);
    CommandLineParser parser(argc, argv);

    EXPECT_TRUE(parser.HasOption("justakey"));
    EXPECT_EQ(parser.GetOption("justakey"), "");
}

TEST_F(CommandLineParserTest, HandlesEmptyValueAfterEqualSign) {
    int argc;
    char** argv;
    BuildArgv({ "App.exe", "-rhi=" }, argc, argv); // rhi= (empty value)
    CommandLineParser parser(argc, argv);

    EXPECT_TRUE(parser.HasOption("rhi"));
    EXPECT_EQ(parser.GetOption("rhi"), "");
}

TEST_F(CommandLineParserTest, HandlesOnlyEqualSign) {
    int argc;
    char** argv;
    BuildArgv({ "App.exe", "-=" }, argc, argv); // malformed: "-="
    CommandLineParser parser(argc, argv);

    EXPECT_FALSE(parser.HasOption(""));
    EXPECT_EQ(parser.GetOption(""), "");
}

TEST_F(CommandLineParserTest, HandlesUnbalancedQuotes) {
    int argc;
    char** argv;
    BuildArgv({ "App.exe", "-name=\"Unfinished" }, argc, argv); // Unbalanced quote
    CommandLineParser parser(argc, argv);

    EXPECT_TRUE(parser.HasOption("name"));
    EXPECT_EQ(parser.GetOption("name"), "\"Unfinished"); // Still preserves raw value
}

TEST_F(CommandLineParserTest, HandlesMultipleEquals) {
    int argc;
    char** argv;
    BuildArgv({ "App.exe", "-shader=path=to/shader" }, argc, argv); // multiple '='
    CommandLineParser parser(argc, argv);

    EXPECT_TRUE(parser.HasOption("shader"));
    EXPECT_EQ(parser.GetOption("shader"), "path=to/shader");
}

TEST_F(CommandLineParserTest, HandlesArrayBasicSplit) {
    int argc;
    char** argv;
    BuildArgv({ "App.exe", "-array=\"one two three\"" }, argc, argv);
    CommandLineParser parser(argc, argv);

    auto result = parser.GetArrayOption("array");

    ASSERT_EQ(result.size(), 3u);
    EXPECT_EQ(result[0], "one");
    EXPECT_EQ(result[1], "two");
    EXPECT_EQ(result[2], "three");
}

TEST_F(CommandLineParserTest, HandlesArraySingleQuotedToken) {
    int argc;
    char** argv;
    BuildArgv({ "App.exe", "-array=\"one 'two words' three\"" }, argc, argv);
    CommandLineParser parser(argc, argv);

    auto result = parser.GetArrayOption("array");

    ASSERT_EQ(result.size(), 3u);
    EXPECT_EQ(result[0], "one");
    EXPECT_EQ(result[1], "two words");
    EXPECT_EQ(result[2], "three");
}

TEST_F(CommandLineParserTest, HandlesArrayMultipleQuotedTokens) {
    int argc;
    char** argv;
    BuildArgv({ "App.exe", "-array=\"'first one' mid 'last part'\"" }, argc, argv);
    CommandLineParser parser(argc, argv);

    auto result = parser.GetArrayOption("array");

    ASSERT_EQ(result.size(), 3u);
    EXPECT_EQ(result[0], "first one");
    EXPECT_EQ(result[1], "mid");
    EXPECT_EQ(result[2], "last part");
}

TEST_F(CommandLineParserTest, HandlesArrayUnclosedApostrophe) {
    int argc;
    char** argv;
    BuildArgv({ "App.exe", "-array=\"start 'unfinished token\"" }, argc, argv);
    CommandLineParser parser(argc, argv);

    auto result = parser.GetArrayOption("array");

    ASSERT_EQ(result.size(), 2u);
    EXPECT_EQ(result[0], "start");
    EXPECT_EQ(result[1], "unfinished token"); // still includes the rest
}

TEST_F(CommandLineParserTest, ArraySkipsExtraSpacesOutsideTokens) {
    int argc;
    char** argv;
    BuildArgv({ "App.exe", "-array=\"  one   'two  words'   three  \"" }, argc, argv);
    CommandLineParser parser(argc, argv);

    auto result = parser.GetArrayOption("array");

    ASSERT_EQ(result.size(), 3u);
    EXPECT_EQ(result[0], "one");
    EXPECT_EQ(result[1], "two  words");
    EXPECT_EQ(result[2], "three");
}

TEST_F(CommandLineParserTest, HandlesArrayOnlyQuotedToken) {
    int argc;
    char** argv;
    BuildArgv({ "App.exe", "-array=\"'just one token'\"" }, argc, argv);
    CommandLineParser parser(argc, argv);

    auto result = parser.GetArrayOption("array");

    ASSERT_EQ(result.size(), 1u);
    EXPECT_EQ(result[0], "just one token");
}

TEST_F(CommandLineParserTest, HandlesArrayEmptyInput) {
    int argc;
    char** argv;
    BuildArgv({ "App.exe", "-array=\"\"" }, argc, argv);
    CommandLineParser parser(argc, argv);

    auto result = parser.GetArrayOption("array");
    ASSERT_TRUE(result.empty());
}
TEST_F(CommandLineParserTest, ParsesIntegerCorrectly) {
    int argc;
    char** argv;
    BuildArgv({ "App.exe", "-threads=8" }, argc, argv);
    CommandLineParser parser(argc, argv);

    EXPECT_EQ(parser.GetIntOption("threads", 4), 8);
}

TEST_F(CommandLineParserTest, FallsBackToDefaultOnInvalidInt) {
    int argc;
    char** argv;
    BuildArgv({ "App.exe", "-threads=fast" }, argc, argv);
    CommandLineParser parser(argc, argv);

    EXPECT_EQ(parser.GetIntOption("threads", 4), 4);
}

TEST_F(CommandLineParserTest, UsesDefaultIntIfMissing) {
    int argc;
    char** argv;
    BuildArgv({ "App.exe" }, argc, argv);
    CommandLineParser parser(argc, argv);

    EXPECT_EQ(parser.GetIntOption("threads", 2), 2);
}
TEST_F(CommandLineParserTest, ParsesNegativeIntCorrectly) {
    int argc;
    char** argv;
    BuildArgv({ "App.exe", "-threads=-1", "-speed=3" }, argc, argv);
    CommandLineParser parser(argc, argv);

    EXPECT_EQ(parser.GetIntOption("threads"), -1);
    EXPECT_EQ(parser.GetIntOption("speed"), 3);
}
TEST_F(CommandLineParserTest, ParsesLongLongIntegerCorrectly) {
    int argc;
    char** argv;
    BuildArgv({ "App.exe", "-threads=999999999999999" }, argc, argv);
    CommandLineParser parser(argc, argv);

    EXPECT_EQ(parser.GetIntOption("threads"), static_cast<i64>(999999999999999));
}