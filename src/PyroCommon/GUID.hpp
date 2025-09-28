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

#pragma once
#include <EASTL/array.h>
#include <EASTL/functional.h> // for eastl::hash
#include <EASTL/string.h>
#include <PyroCommon/Core.hpp>

namespace PyroshockStudios {
    class GUID {
    public:
        using Container = eastl::array<u8, 16>;

        // automatically generates a random GUID
        PYRO_COMMON_API GUID() noexcept;
        constexpr explicit GUID(const Container& b) noexcept : mBytes(b) {}
        ~GUID() = default;

        GUID(const GUID&) = default;
        GUID& operator=(const GUID&) = default;
        GUID(GUID&&) = default;
        GUID& operator=(GUID&&) = default;

        PYRO_COMMON_API PYRO_NODISCARD eastl::string ToString() const noexcept;

        PYRO_COMMON_API PYRO_NODISCARD static GUID FromString(const char* str);
        PYRO_NODISCARD PYRO_FORCEINLINE static GUID FromString(const eastl::string& str) {
            return FromString(str.c_str());
        }

        PYRO_NODISCARD PYRO_FORCEINLINE static GUID Invalid() noexcept {
            return GUID(Container{});
        }

        PYRO_NODISCARD PYRO_FORCEINLINE bool operator==(const GUID& other) const noexcept {
            return mBytes == other.mBytes;
        }
        PYRO_NODISCARD PYRO_FORCEINLINE bool operator!=(const GUID& other) const noexcept {
            return !(*this == other);
        }

        PYRO_NODISCARD PYRO_FORCEINLINE operator bool() const noexcept {
            return Valid();
        }

        // Returns false if the bytes are empty
        PYRO_NODISCARD PYRO_FORCEINLINE bool Valid() const noexcept {
            // must not be empty
            return mBytes != Container{};
        }

    private:
        Container mBytes;

        friend struct eastl::hash<GUID>;
    };

    namespace internal {
        constexpr u8 guid_hexCharToByte(char c) {
            if (c >= '0' && c <= '9')
                return static_cast<u8>(c - '0');
            if (c >= 'a' && c <= 'f')
                return static_cast<u8>(c - 'a' + 10);
            if (c >= 'A' && c <= 'F')
                return static_cast<u8>(c - 'A' + 10);
            throw "Invalid hex character";
        }

        // Utility: parse 2 hex characters into a byte
        constexpr u8 guid_parseByte(char high, char low) {
            return static_cast<u8>((guid_hexCharToByte(high) << 4) | guid_hexCharToByte(low));
        }


        template <usize kCount>
        struct template_str_buffer {
            using char_type = char;

            consteval template_str_buffer(const char_type (&str)[kCount]) noexcept {
                for (size_t i = 0; i < kCount; ++i) {
                    data[i] = str[i];
                }
            }

            char_type data[kCount];
            constexpr static usize count = kCount - sizeof(char_type);
        };


        template <template_str_buffer kStrBuf>
        consteval bool check_dashes(const eastl::array<i32, 4>& dashes) {
            return ((kStrBuf.data[dashes[0]] == '-') &&
                    (kStrBuf.data[dashes[1]] == '-') &&
                    (kStrBuf.data[dashes[2]] == '-') &&
                    (kStrBuf.data[dashes[3]] == '-'));
        }
    } // namespace internal
} // namespace PyroshockStudios

template <PyroshockStudios::internal::template_str_buffer kStrBuf>
consteval PyroshockStudios::GUID operator""_guid() {
    using namespace PyroshockStudios;
    using namespace PyroshockStudios::Types;
    using namespace PyroshockStudios::internal;
    static_assert(kStrBuf.count == 36, "GUID must be 36 characters long");

    constexpr eastl::array<i32, 4> dashes = { 8, 13, 18, 23 };

    static_assert(check_dashes<kStrBuf>(dashes),
        "GUID must have dashes at positions 8, 13, 18, and 23");

    PyroshockStudios::GUID::Container bytes{};
    i32 i = 0, bi = 0;
    while (i < 36) {
        if (kStrBuf.data[i] == '-') {
            ++i;
            continue;
        }
        bytes[bi++] = guid_parseByte(kStrBuf.data[i], kStrBuf.data[i + 1]);
        i += 2;
    }

    return PyroshockStudios::GUID{ bytes };
}


namespace eastl {
    using namespace PyroshockStudios;
    using namespace PyroshockStudios::Types;

    template <>
    struct hash<PyroshockStudios::GUID> {
        PYRO_COMMON_API usize operator()(const PyroshockStudios::GUID& k) const noexcept {
            usize hash = 0;
            for (usize i = 0; i < 16; i += sizeof(usize)) {
                usize chunk = 0;
                for (usize j = 0; j < sizeof(usize) && (i + j) < 16; ++j) {
                    chunk |= static_cast<usize>(k.mBytes[i + j]) << (j * 8);
                }
                hash ^= eastl::hash<usize>{}(chunk) + 0x9e3779b9 + (hash << 6) + (hash >> 2);
            }

            return hash;
        }
    };
} // namespace eastl
