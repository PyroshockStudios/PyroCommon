#include <gtest/gtest.h>
#include <SWCommon/Version.hpp>

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