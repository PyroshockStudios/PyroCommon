#include <PyroCommon/Logger.hpp>
#include <gmock/gmock.h>
#include <gtest/gtest.h>

using namespace PyroshockStudios;
using namespace PyroshockStudios::Types;
using ::testing::_;
using ::testing::Return;
using ::testing::StrEq;

class MockLogStream : public ILogStream {
public:
    MOCK_METHOD(void, Log, (LogSeverity severity, const char* message), (override));
    MOCK_METHOD(LogSeverity, MinSeverity, (), (const, override));
    MOCK_METHOD(const char*, Name, (), (const, override));
};

TEST(TestLogger, LogsMessageAboveMinSeverity) {
    MockLogStream stream;
    EXPECT_CALL(stream, MinSeverity())
        .WillRepeatedly(Return(LogSeverity::Info));

    // Expect Info to be logged
    EXPECT_CALL(stream, Log(LogSeverity::Info, StrEq("Test message 123")));

    Logger::Info(&stream, "Test message {}", 123);
}

TEST(TestLogger, DoesNotLogBelowMinSeverity) {
    MockLogStream stream;
    EXPECT_CALL(stream, MinSeverity())
        .WillRepeatedly(Return(LogSeverity::Warn));

    // Should not call Log since Info < Warn
    EXPECT_CALL(stream, Log(_, _)).Times(0);

    Logger::Info(&stream, "Should not log");
}

TEST(TestLogger, FormatsMessageCorrectly) {
    MockLogStream stream;
    EXPECT_CALL(stream, MinSeverity())
        .WillRepeatedly(Return(LogSeverity::Debug));

    EXPECT_CALL(stream, Log(LogSeverity::Debug, StrEq("Value=42")));

    Logger::Debug(&stream, "Value={}", 42);
}

TEST(TestLogger, HandlesNullStreamGracefully) {
    // Should not crash if stream is nullptr
    EXPECT_NO_THROW(Logger::Info(nullptr, "Hello"));
}

TEST(TestLogger, AllSeveritiesCallLogCorrectly) {
    MockLogStream stream;
    EXPECT_CALL(stream, MinSeverity())
        .WillRepeatedly(Return(LogSeverity::Verbose));

    EXPECT_CALL(stream, Log(LogSeverity::Verbose, StrEq("v")));
    EXPECT_CALL(stream, Log(LogSeverity::Debug, StrEq("d")));
    EXPECT_CALL(stream, Log(LogSeverity::Trace, StrEq("t")));
    EXPECT_CALL(stream, Log(LogSeverity::Info, StrEq("i")));
    EXPECT_CALL(stream, Log(LogSeverity::Warn, StrEq("w")));
    EXPECT_CALL(stream, Log(LogSeverity::Error, StrEq("e")));
    EXPECT_CALL(stream, Log(LogSeverity::Fatal, StrEq("f")));

    Logger::Verbose(&stream, "v");
    Logger::Debug(&stream, "d");
    Logger::Trace(&stream, "t");
    Logger::Info(&stream, "i");
    Logger::Warn(&stream, "w");
    Logger::Error(&stream, "e");
    Logger::Fatal(&stream, "f");
}
