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
#include <EASTL/type_traits.h>
#include <EASTL/internal/move_help.h>

namespace PyroshockStudios {
    template <typename From, typename To>
    concept ConvertibleTo =
        requires(From f) {
            static_cast<To>(f);
        };

    template <typename T>
    concept StandardLayoutConcept =
        eastl::is_trivially_copyable_v<eastl::remove_cvref_t<T>> &&
        !eastl::is_function_v<eastl::remove_cvref_t<T>> &&
        !eastl::is_pointer_v<eastl::remove_cvref_t<T>>;

    template <typename T>
    concept CopyableConcept =
        eastl::is_copy_assignable_v<eastl::remove_cvref_t<T>> &&
        eastl::is_copy_constructible_v<eastl::remove_cvref_t<T>>;

    template <typename T>
    concept MoveableConcept =
        eastl::is_move_assignable_v<eastl::remove_cvref_t<T>> &&
        eastl::is_move_constructible_v<eastl::remove_cvref_t<T>>;

    template <typename T>
    concept CopyMoveableConcept =
        CopyableConcept<T> && MoveableConcept<T>;

    template <typename T>
    concept IterableConcept =
        requires(const T& t) {
            t.begin();
            t.end();
        };

    template <typename T>
    concept CollectionConcept =
        IterableConcept<T> &&
        requires(const T& t) {
            t.size();
        };

    template <typename C>
    concept ContiguousCollectionConcept =
        requires(C& c) {
            { c.data() } -> ConvertibleTo<typename eastl::add_pointer_t<
                eastl::remove_reference_t<decltype(*c.data())>>>;
            { c.size() } -> ConvertibleTo<size_t>;
        };

    template <typename T>
    concept MapConcept =
        requires(const T& t) {
            typename T::key_type;
            typename T::mapped_type;
            t.begin();
            t.end();
            t.size();
            // Must be iterable over pairs
            { *t.begin() } -> ConvertibleTo<const typename T::value_type&>;
        };

    template <typename C>
    concept ResizableConcept = requires(C& c) {
        { c.resize(size_t{}) };
    };
    template <typename C>
    concept ClearableConcept = requires(C& c) {
        { c.clear() };
    };

    template <typename C>
    concept PushBackableConcept = requires(C& c, eastl::remove_cvref_t<decltype(*c.begin())> v) {
        c.push_back(v);
    };

    template <typename C, typename... Args>
    concept EmplaceBackableConcept = requires(C& c, Args&&... args) {
        c.emplace_back(eastl::forward<Args>(args)...);
    };

    template <typename T>
    concept OptionalConcept =
        requires(const T& t) {
            t.has_value();
            *t;
        };

} // namespace PyroshockStudios