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
#include <cstddef>
#include <cstdint>

namespace PyroshockStudios {
    inline namespace Types {
        using u8 = std::uint8_t;
        using u16 = std::uint16_t;
        using u32 = std::uint32_t;
        using u64 = std::uint64_t;
        using usize = std::size_t;
        using b32 = u32;

        enum PyroB32 : b32 {
            PYRO_FALSE32 = b32(0),
            PYRO_TRUE32 = b32(1),
        };

        using i8 = std::int8_t;
        using i16 = std::int16_t;
        using i32 = std::int32_t;
        using i64 = std::int64_t;
        using isize = std::ptrdiff_t;

        using uptr = std::uintptr_t;
        using iptr = std::intptr_t;

        using f32 = float;
        using f64 = double;

        using NativeHandle = uptr;

        struct Point {
            i32 x = {};
            i32 y = {};

            inline bool operator==(const Point& point) const = default;
            inline bool operator!=(const Point& point) const = default;
        };
        struct Size {
            u32 width = {};
            u32 height = {};

            inline bool operator==(const Size& point) const = default;
            inline bool operator!=(const Size& point) const = default;
        };
        struct FPoint {
            f32 x = {};
            f32 y = {};
            inline bool operator==(const FPoint& point) const = default;
            inline bool operator!=(const FPoint& point) const = default;
        };
        struct FSize {
            f32 width = {};
            f32 height = {};
            inline bool operator==(const FSize& point) const = default;
            inline bool operator!=(const FSize& point) const = default;
        };
    } // namespace Types
} // namespace PyroshockStudios