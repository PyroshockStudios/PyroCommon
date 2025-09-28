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
#include <PyroCommon/Allocator.hpp>
#include <PyroCommon/Concepts.hpp>
#include <PyroCommon/Platform.hpp>
#include <PyroCommon/Traits.hpp>
#include <PyroCommon/Types.hpp>
#include <compare>

#ifdef _MSC_VER

#ifdef PYRO_COMMON_SHARED_LIBRARY
#ifdef PyroCommon_EXPORTS
#define PYRO_COMMON_API __declspec(dllexport)
#else
#define PYRO_COMMON_API __declspec(dllimport)
#endif
#else
#define PYRO_COMMON_API
#endif

#define PYRO_EXPORT extern "C" __declspec(dllexport)
#define PYRO_NO_VTABLE __declspec(novtable)
#define PYRO_CDECL __cdecl
#define PYRO_FORCEINLINE __forceinline

#else
#define PYRO_COMMON_API

#define PYRO_EXPORT extern "C"
#define PYRO_NO_VTABLE
// doesnt seem to be necessary, gcc and clang both generate warnings that this attribute is ignored
// #define PYRO_CDECL __attribute__((__cdecl__))
#define PYRO_CDECL
#define PYRO_FORCEINLINE __attribute__((always_inline)) inline

#endif

#define PYRO_NODISCARD [[nodiscard]]
#define PYRO_MAX_SIZE (~(static_cast<usize>(0)))
#define PYRO_MAX_FLOAT (3.402823466e+38F)
#define PYRO_MAX_DOUBLE (1.7976931348623158e+308)
#define PYRO_ARRAY_SIZE(p) ((sizeof(p)) / (sizeof(*p)))
#define PYRO_VERIFY_ALIGNMENT(data, alignment) (((data) % (alignment)) == 0)
#define PYRO_ALIGN(value, alignment) ((value + alignment - 1) & ~(alignment - 1))

namespace PyroshockStudios {
    template <typename Properties>
    struct Flags final {
        typename Properties::Data data;
        PYRO_FORCEINLINE constexpr Flags& operator|=(const Flags& other) noexcept {
            data |= other.data;
            return *this;
        }

        PYRO_FORCEINLINE constexpr Flags& operator&=(const Flags& other) noexcept {
            data &= other.data;
            return *this;
        }

        PYRO_FORCEINLINE constexpr Flags& operator^=(const Flags& other) noexcept {
            data ^= other.data;
            return *this;
        }

        PYRO_NODISCARD PYRO_FORCEINLINE constexpr Flags operator~() const noexcept {
            return { ~data };
        }

        PYRO_NODISCARD PYRO_FORCEINLINE constexpr Flags operator|(const Flags& other) const noexcept {
            return { data | other.data };
        }

        PYRO_NODISCARD PYRO_FORCEINLINE constexpr Flags operator&(const Flags& other) const noexcept {
            return { data & other.data };
        }

        PYRO_NODISCARD PYRO_FORCEINLINE constexpr Flags operator^(const Flags& other) const noexcept {
            return { data ^ other.data };
        }

        PYRO_NODISCARD PYRO_FORCEINLINE constexpr auto operator<=>(const Flags& other) const = default;

        PYRO_FORCEINLINE constexpr operator bool() const noexcept {
            return data != 0;
        }
    };
    template <typename... Ts>
    struct Union;

    template <typename T, typename... Ts>
    struct Union<T, Ts...> {
        template <typename U>
        PYRO_FORCEINLINE U& Get() { return *reinterpret_cast<U*>(this); }
        template <typename U>
        PYRO_FORCEINLINE const U& Get() const { return *reinterpret_cast<const U*>(this); }
        PYRO_FORCEINLINE void* Data() { return reinterpret_cast<void*>(this); }
        PYRO_FORCEINLINE const void* Data() const { return reinterpret_cast<const void*>(this); }

        Union() = default;
        ~Union() = default;

        template <typename U>
        Union& operator=(const U& v) {
            next = v;
            return *this;
        }
        Union& operator=(const T& v) {
            value = v;
            return *this;
        }
        PYRO_NODISCARD PYRO_FORCEINLINE bool operator==(const Union& other) const {
            return value == other.value && next == other.next;
        }
        PYRO_NODISCARD PYRO_FORCEINLINE bool operator!=(const Union& other) const {
            return !(*this == other);
        }

    protected:
        union {
            T value;
            Union<Ts...> next;
        };
    };

    template <typename T>
    struct Union<T> {
        template <typename U>
        PYRO_FORCEINLINE U& Get() { return *reinterpret_cast<U*>(this); }
        template <typename U>
        PYRO_FORCEINLINE const U& Get() const { return *reinterpret_cast<U*>(this); }
        PYRO_FORCEINLINE void* Data() { return reinterpret_cast<void*>(this); }
        PYRO_FORCEINLINE const void* Data() const { return reinterpret_cast<void*>(this); }

        Union() = default;
        ~Union() = default;

        Union& operator=(const T& v) {
            value = v;
            return *this;
        }

        PYRO_NODISCARD PYRO_FORCEINLINE bool operator==(const Union& other) const {
            return value == other.value;
        }
        PYRO_NODISCARD PYRO_FORCEINLINE bool operator!=(const Union& other) const {
            return !(*this == other);
        }

    protected:
        union {
            T value;
        };
    };
    template <typename>
    struct FunctionPtr_;

    // partial specialization for function types
    template <typename Ret, typename... Args>
    struct FunctionPtr_<Ret(Args...)> {
        using Type = Ret (*)(Args...);
    };

    // convenient alias
    template <typename Signature>
    using FunctionPtr = typename FunctionPtr_<Signature>::Type;
} // namespace PyroshockStudios