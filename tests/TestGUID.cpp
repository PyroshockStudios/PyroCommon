#include <gtest/gtest.h>
#include <SWCommon/GUID.hpp>
#include <EASTL/hash_set.h>
#include <EASTL/hash_map.h>

using namespace PyroshockStudios;

TEST(TestGUID, RandomGeneration) {
    GUID guid1 = GUID();
    GUID guid2 = GUID();
    // this has an absurdly low chance at collision!!
    EXPECT_NE(guid1, guid2);
}

TEST(TestGUID, CopyConstructor) {
    GUID guid1 = GUID();
    GUID guid2 = guid1;
    EXPECT_EQ(guid1, guid2);
}

TEST(TestGUID, MoveConstructor) {
    GUID guid1 = GUID();
    GUID expected = guid1; // assuming copy constructor works

    GUID guid2 = eastl::move(guid1);

    EXPECT_EQ(expected, guid2);
}

TEST(TestGUID, ToString) {
    GUID exampleGuid = GUID({ 0xc0, 0xff, 0xee, 0xba, 0xbe, 0xde, 0xad, 0xbe, 0xef, 0x01, 0x23, 0x45, 0x67, 0x89, 0x10, 0xcc });
    // xxxxxxxx-xxxx-xxxx-xxxx-xxxxxxxxxxxx
    const char* expectedString = "c0ffeeba-bede-adbe-ef01-2345678910cc";

    EXPECT_STREQ(exampleGuid.ToString().c_str(), expectedString);
}
TEST(TestGUID, FromString) {
    // xxxxxxxx-xxxx-xxxx-xxxx-xxxxxxxxxxxx
    const char* exampleString = "c0ffeeba-bede-adbe-ef01-2345678910cc";
    GUID expectedGuid = GUID({ 0xc0, 0xff, 0xee, 0xba, 0xbe, 0xde, 0xad, 0xbe, 0xef, 0x01, 0x23, 0x45, 0x67, 0x89, 0x10, 0xcc });

    EXPECT_EQ(GUID::FromString(exampleString), expectedGuid);
}

TEST(TestGUID, UserLiteralOperator) {
    GUID exampleStringGuid = "c0ffeeba-bede-adbe-ef01-2345678910cc"_guid;
    GUID expectedGuid = GUID({ 0xc0, 0xff, 0xee, 0xba, 0xbe, 0xde, 0xad, 0xbe, 0xef, 0x01, 0x23, 0x45, 0x67, 0x89, 0x10, 0xcc });

    EXPECT_EQ(exampleStringGuid, expectedGuid);
}
TEST(TestGUID, InvalidGUID) {
    GUID invalid = GUID::Invalid();
    ASSERT_FALSE(invalid.Valid());
}
TEST(TestGUID, ValidGUID) {
    GUID valid = GUID();
    ASSERT_TRUE(valid.Valid());
}


TEST(TestGUID, HashSetWorks) {
    auto guid1 = "e4d1c35e-2b17-4e0a-b3e1-0d13eeb14f27"_guid;
    auto guid2 = "01234567-89ab-cdef-0123-456789abcdef"_guid;

    eastl::hash_set<GUID> set;
    set.insert(guid1);
    set.insert(guid2);

    EXPECT_EQ(set.count(guid1), 1);
    EXPECT_EQ(set.count(guid2), 1);
}

TEST(TestGUID, HashMapWorks) {
    auto guid1 = "e4d1c35e-2b17-4e0a-b3e1-0d13eeb14f27"_guid;
    auto guid2 = "01234567-89ab-cdef-0123-456789abcdef"_guid;

    eastl::hash_map<GUID, eastl::string> map;
    map[guid1] = "test1";
    map[guid2] = "test2";
    EXPECT_EQ(map[guid1], "test1");
    EXPECT_EQ(map[guid2], "test2");
}
