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

#include <PyroCommon/Serialization/BinarySerializer.hpp>
#include <PyroCommon/Stream/MemoryStream.hpp>

#include <EASTL/array.h>
#include <EASTL/map.h>
#include <EASTL/optional.h>
#include <EASTL/string.h>
#include <EASTL/unordered_map.h>
#include <EASTL/vector.h>
#include <gtest/gtest.h>
#include <random>

using namespace PyroshockStudios;

// Example serializable type
struct MyStruct : public ISerializable {
    MyStruct() = default;
    int a{};
    float b{};

    MyStruct(int a, float b) : a(a), b(b) {}

    void Serialize(BinarySerializer& s) const override {
        s << a << b;
    }
    void Deserialize(BinarySerializer& s) override {
        s >> a >> b;
    }
    bool operator==(const MyStruct& other) const {
        return a == other.a && b == other.b;
    }
};

class TestBinarySerializer : public ::testing::Test {
protected:
    MemoryStream mem;
    BinarySerializer serializer{ &mem, &mem };
};

// --- TESTS ---
TEST_F(TestBinarySerializer, PodRoundTrip_Int) {
    int32_t in = 123456;
    serializer << in;
    mem.Seek(0);
    int32_t out = 0;
    serializer >> out;

    EXPECT_EQ(in, out);
}

TEST_F(TestBinarySerializer, VectorRoundTrip_POD) {
    eastl::vector<uint32_t> in = { 1, 2, 3, 4, 5 };
    serializer << in;
    mem.Seek(0);

    eastl::vector<uint32_t> out;
    serializer >> out;

    EXPECT_EQ(in, out);
}

TEST_F(TestBinarySerializer, VectorRoundTrip_Serializable) {
    eastl::vector<MyStruct> in = { { 1, 1.5f }, { 2, 2.5f }, { 3, 3.5f } };
    serializer << in;
    mem.Seek(0);

    eastl::vector<MyStruct> out;
    serializer >> out;

    EXPECT_EQ(in.size(), out.size());
    for (size_t i = 0; i < in.size(); ++i) {
        EXPECT_EQ(in[i], out[i]);
    }
}

TEST_F(TestBinarySerializer, OptionalRoundTrip) {
    eastl::optional<int32_t> in = 999;
    serializer << in;
    mem.Seek(0);

    eastl::optional<int32_t> out;
    serializer >> out;

    ASSERT_TRUE(out.has_value());
    EXPECT_EQ(in.value(), out.value());
}

TEST_F(TestBinarySerializer, OptionalEmptyRoundTrip) {
    eastl::optional<int32_t> in = eastl::nullopt;
    serializer << in;
    mem.Seek(0);

    eastl::optional<int32_t> out = 123; // start with value
    serializer >> out;

    EXPECT_FALSE(out.has_value());
}

TEST_F(TestBinarySerializer, EmptyVector) {
    eastl::vector<int> in;
    serializer << in;
    mem.Seek(0);

    eastl::vector<int> out;
    serializer >> out;

    EXPECT_TRUE(out.empty());
}

TEST_F(TestBinarySerializer, FixedArrayExactSize) {
    eastl::array<int, 3> in = { 1, 2, 3 };
    serializer << in;
    mem.Seek(0);

    eastl::array<int, 3> out{};
    serializer >> out;

    EXPECT_EQ(in, out);
}

TEST_F(TestBinarySerializer, FixedArraySizeMismatchThrows) {
    eastl::array<int, 3> in = { 1, 2, 3 };
    serializer << in;
    mem.Seek(0);

    eastl::array<int, 4> out{}; // mismatch — should throw
    EXPECT_THROW(serializer >> out, std::runtime_error);
}

TEST_F(TestBinarySerializer, OptionalEmpty) {
    eastl::optional<int> in{};
    serializer << in;
    mem.Seek(0);

    eastl::optional<int> out = 5; // start with value
    serializer >> out;

    EXPECT_FALSE(out.has_value());
}

TEST_F(TestBinarySerializer, OptionalWithValue) {
    eastl::optional<int> in = 42;
    serializer << in;

    mem.Seek(0);
    eastl::optional<int> out;
    serializer >> out;

    ASSERT_TRUE(out.has_value());
    EXPECT_EQ(out.value(), 42);
}


TEST_F(TestBinarySerializer, StringEmpty) {
    eastl::string in{};
    serializer << in;
    mem.Seek(0);

    eastl::string out = "bla"; // start with value
    serializer >> out;

    EXPECT_TRUE(out.empty());
}

TEST_F(TestBinarySerializer, StringValid) {
    eastl::string in = "Hello world! 1234";
    serializer << in;
    mem.Seek(0);
    eastl::string out{};
    serializer >> out;

    ASSERT_FALSE(out.empty());
    EXPECT_STREQ(out.c_str(), in.c_str());
}

TEST_F(TestBinarySerializer, LargeVectorBulkWrite) {
    const size_t count = 10000;
    eastl::vector<uint64_t> in(count);
    for (size_t i = 0; i < count; ++i) {
        in[i] = static_cast<uint64_t>(i * 10);
    }

    serializer << in;

    mem.Seek(0);
    eastl::vector<uint64_t> out;
    serializer >> out;

    EXPECT_EQ(in.size(), out.size());
    for (size_t i = 0; i < count; ++i) {
        EXPECT_EQ(in[i], out[i]);
    }
}


TEST_F(TestBinarySerializer, StressTest) {
    eastl::vector<eastl::array<eastl::optional<eastl::string>, 4>> in = {
        eastl::array<eastl::optional<eastl::string>, 4>{ eastl::optional<eastl::string>{ "Hello 11" }, {}, {}, eastl::optional<eastl::string>{ "Hello 14" } },
    };

    serializer << in;
    mem.Seek(0);
    eastl::vector<eastl::array<eastl::optional<eastl::string>, 4>> out{};
    serializer >> out;

    EXPECT_EQ(out.size(), 1);
    EXPECT_TRUE(out[0][0].has_value());
    EXPECT_FALSE(out[0][1].has_value());
    EXPECT_FALSE(out[0][2].has_value());
    EXPECT_TRUE(out[0][3].has_value());

    EXPECT_STREQ(out[0][0].value().c_str(), "Hello 11");
    EXPECT_STREQ(out[0][3].value().c_str(), "Hello 14");
}

TEST_F(TestBinarySerializer, BasicSerialization_Map) {
    eastl::map<int, float> original{
        { 1, 1.5f },
        { 2, -3.25f },
        { 5, 42.0f }
    };

    serializer << original;

    eastl::map<int, float> loaded;
    mem.Seek(0);
    serializer >> loaded;

    EXPECT_EQ(original.size(), loaded.size());
    for (auto& [key, value] : original) {
        EXPECT_EQ(value, loaded.at(key));
    }
}

TEST_F(TestBinarySerializer, BasicSerialization_UnorderedMap) {
    eastl::unordered_map<eastl::string, int> original{
        { "alpha", 10 },
        { "beta", -5 },
        { "gamma", 999 }
    };

    serializer << original;

    eastl::unordered_map<eastl::string, int> loaded;
    mem.Seek(0);
    serializer >> loaded;

    EXPECT_EQ(original.size(), loaded.size());
    for (auto& [key, value] : original) {
        auto it = loaded.find(key);
        ASSERT_NE(it, loaded.end());
        EXPECT_EQ(it->second, value);
    }
}

TEST_F(TestBinarySerializer, EmptyMap) {

    eastl::unordered_map<int, int> original;
    serializer << original;

    eastl::unordered_map<int, int> loaded;
    mem.Seek(0);
    serializer >> loaded;

    EXPECT_TRUE(loaded.empty());
}

TEST_F(TestBinarySerializer, SingleElement_Map) {

    eastl::map<int, eastl::string> original{
        { 42, "The Answer" }
    };

    serializer << original;

    eastl::map<int, eastl::string> loaded;
    mem.Seek(0);
    serializer >> loaded;

    ASSERT_EQ(loaded.size(), 1u);
    EXPECT_EQ(loaded.at(42), "The Answer");
}

TEST_F(TestBinarySerializer, StressTest_UnorderedMap) {

    constexpr size_t kNumEntries = 1000;
    std::mt19937 rng(12345); // Fixed seed for reproducibility
    std::uniform_int_distribution<int> keyDist(1, 5000);
    std::uniform_real_distribution<float> valueDist(-1000.0f, 1000.0f);

    eastl::unordered_map<int, float> original;
    original.reserve(kNumEntries);
    for (size_t i = 0; i < kNumEntries; ++i) {
        int key = keyDist(rng);
        float value = valueDist(rng);
        original[key] = value; // If duplicate key generated, it will overwrite — fine for test
    }

    serializer << original;

    eastl::unordered_map<int, float> loaded;
    mem.Seek(0);
    serializer >> loaded;

    EXPECT_EQ(original.size(), loaded.size());
    for (auto& [key, value] : original) {
        auto it = loaded.find(key);
        ASSERT_NE(it, loaded.end()) << "Missing key: " << key;
        EXPECT_FLOAT_EQ(it->second, value) << "Mismatch for key: " << key;
    }
}