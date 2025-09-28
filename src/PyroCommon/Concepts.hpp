#pragma once
#include <EASTL/type_traits.h>

namespace PyroshockStudios {
    template <typename From, typename To>
    concept ConvertibleTo =
        requires {
            static_cast<To>(eastl::declval<From>());
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
        requires {
            CopyableConcept<T> && MoveableConcept<T>;
        };

    template <typename T>
    concept IterableConcept =
        requires(const T& t) {
            t.begin();
            t.end();
        };

    template <typename T>
    concept CollectionConcept =
        requires(const T& t) {
            requires IterableConcept<T>;
            t.size();
        };

    template <typename C>
    concept ContiguousCollectionConcept = requires(C& c) {
        { c.data() } -> ConvertibleTo<typename eastl::add_pointer_t<eastl::remove_reference_t<decltype(*c.data())>>>;
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

    template <typename C>
    concept EmplaceBackableConcept = requires(C& c, eastl::remove_cvref_t<decltype(*c.begin())> v) {
        c.push_back(v);
    };

    template <typename T>
    concept OptionalConcept =
        requires(const T& t) {
            t.has_value();
            *t;
        };

}