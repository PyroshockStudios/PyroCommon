#pragma once
#include <SWCommon/Types.hpp>
#include <eastl/utility.h>
#include "HashCombine.hpp"

namespace PyroshockStudios {
    inline namespace Util {
        inline void HashCombine(usize& seed) {
        }

        template <typename T, typename... Rest>
        inline void HashCombine(usize& seed, const T& v, Rest... rest) {
            eastl::hash<T> hasher;
            seed ^= hasher(v) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
            HashCombine(seed, rest...); // now eventually hits base case
        }

    }
}
