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
#include <PyroCommon/Core.hpp>

#include <EASTL/string.h>

namespace PyroshockStudios {
    constexpr static u32 BUILD_VERSION = {
        // YYYY
        u32(__DATE__[7] - 0x30) * 10000000 + u32(__DATE__[8] - 0x30) * 1000000 + u32(__DATE__[9] - 0x30) * 100000 + u32(__DATE__[10] - 0x30) * 10000 +

        // MM
        (__DATE__[0] == 'O' || __DATE__[0] == 'N' || __DATE__[0] == 'D') * 1000 + // if greater/equal to 10
        ((__DATE__[0] == 'J')      ? ((__DATE__[1] == 'a') ? 100 :                // Jan, Jun or Jul
                                        ((__DATE__[2] == 'n') ? 600 : 700))
            : (__DATE__[0] == 'F') ? 200
                                   : // Feb
                (__DATE__[0] == 'M') ? (__DATE__[2] == 'r') ? 300 : 500
                                     : // Mar or May
                (__DATE__[0] == 'A') ? (__DATE__[1] == 'p') ? 400 : 800
                                     : // Apr or Aug
                (__DATE__[0] == 'S') ? 900
                                     : // Sep
                // october adds nothing
                (__DATE__[0] == 'N') ? 100
                                     : // Nov
                (__DATE__[0] == 'D') ? 200
                                     : // Dec
                0) +

        // DD
        (__DATE__[4] == ' ' ? 0 : uint32_t(__DATE__[4] - 0x30)) * 10 + uint32_t(__DATE__[5] - 0x30)
    };

    struct Version {
        constexpr Version(uint8_t major, uint8_t minor, uint16_t revision, uint32_t buildDate) noexcept
            : mData((static_cast<uint64_t>(major) << 56) |
                    (static_cast<uint64_t>(minor) << 48) |
                    (static_cast<uint64_t>(revision) << 32) |
                    (static_cast<uint64_t>(buildDate))) {
        }
        Version() = default;
        Version(const Version&) = default;
        Version& operator=(const Version&) = default;
        Version(Version&&) = default;
        Version& operator=(Version&&) = default;

        PYRO_NODISCARD PYRO_FORCEINLINE constexpr u32 Major() const noexcept { return static_cast<u32>((mData >> 56) & 0xFF); }
        PYRO_NODISCARD PYRO_FORCEINLINE constexpr u32 Minor() const noexcept { return static_cast<u32>((mData >> 48) & 0xFF); }
        PYRO_NODISCARD PYRO_FORCEINLINE constexpr u32 Revision() const noexcept { return static_cast<u32>((mData >> 32) & 0xFFFF); }
        PYRO_NODISCARD PYRO_FORCEINLINE constexpr u32 BuildDate() const noexcept { return static_cast<u32>(mData & 0xFFFFFFFF); }
        // Returns the version without the build date.
        PYRO_NODISCARD PYRO_FORCEINLINE constexpr u32 PureVersion() const noexcept { return static_cast<u32>((mData >> 32) & 0xFFFFFFFF); }

        PYRO_COMMON_API eastl::string ToString() const;

        // Comparison operators (compare raw Data)
        PYRO_NODISCARD PYRO_FORCEINLINE constexpr bool operator==(const Version& other) const noexcept { return mData == other.mData; }
        PYRO_NODISCARD PYRO_FORCEINLINE constexpr bool operator!=(const Version& other) const noexcept { return mData != other.mData; }
        PYRO_NODISCARD PYRO_FORCEINLINE constexpr bool operator<(const Version& other) const noexcept { return mData < other.mData; }
        PYRO_NODISCARD PYRO_FORCEINLINE constexpr bool operator>(const Version& other) const noexcept { return mData > other.mData; }
        PYRO_NODISCARD PYRO_FORCEINLINE constexpr bool operator<=(const Version& other) const noexcept { return mData <= other.mData; }
        PYRO_NODISCARD PYRO_FORCEINLINE constexpr bool operator>=(const Version& other) const noexcept { return mData >= other.mData; }

    private:
        u64 mData;
    };
} // namespace PyroshockStudios