#pragma once
#include <SWCommon/Core.hpp>

#include <EASTL/string.h>

namespace PyroshockStudios {
    struct PYRO_COMMON_API Version {
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

        constexpr inline u32 Major() const noexcept { return static_cast<u32>((mData >> 56) & 0xFF); }
        constexpr inline u32 Minor() const noexcept { return static_cast<u32>((mData >> 48) & 0xFF); }
        constexpr inline u32 Revision() const noexcept { return static_cast<u32>((mData >> 32) & 0xFFFF); }
        constexpr inline u32 BuildDate() const noexcept { return static_cast<u32>(mData & 0xFFFFFFFF); }
        // Returns the version without the build date.
        constexpr inline u32 PureVersion() const noexcept { return static_cast<u32>((mData >> 32) & 0xFFFFFFFF); }

        eastl::string ToString() const;

        // Comparison operators (compare raw Data)
        constexpr bool operator==(const Version& other) const noexcept { return mData == other.mData; }
        constexpr bool operator!=(const Version& other) const noexcept { return mData != other.mData; }
        constexpr bool operator<(const Version& other) const noexcept{ return mData < other.mData; }
        constexpr bool operator>(const Version& other) const noexcept{ return mData > other.mData; }
        constexpr bool operator<=(const Version& other) const noexcept{ return mData <= other.mData; }
        constexpr bool operator>=(const Version& other) const noexcept{ return mData >= other.mData; }

        static Version CurrentVersion() ;

    private:
        u64 mData;
    };
}