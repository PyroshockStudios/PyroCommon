#pragma once
#include <EASTL/internal/move_help.h>
#include <SWCommon/Core.hpp>
#include <atomic>

namespace PyroshockStudios {
    // Base class for intrusive ref-counting
    class RefCounted {
    public:
        void AddRef() const noexcept {
            mReferences.fetch_add(1, std::memory_order_relaxed);
        }

        void Release() const noexcept {
            if (mReferences.fetch_sub(1, std::memory_order_acq_rel) == 1) {
                delete this;
            }
        }

    protected:
        virtual ~RefCounted() = default;

    private:
        mutable std::atomic<u32> mReferences = { 0 };
    };

    template <typename T>
    class SharedRef {
        T* mPtr = nullptr;

    public:
        SharedRef() = default;
        explicit SharedRef(T* ptr) : mPtr(ptr) {
            if (mPtr)
                mPtr->AddRef();
        }

        SharedRef(std::nullptr_t) : mPtr(nullptr) {}

        // Copy constructor
        SharedRef(const SharedRef& other) : mPtr(other.mPtr) {
            if (mPtr)
                mPtr->AddRef();
        }

        // Move constructor
        SharedRef(SharedRef&& other) noexcept : mPtr(other.mPtr) {
            other.mPtr = nullptr;
        }

        // Converting copy constructor: allows SharedRef<Derived> -> SharedRef<Base>
        template <typename U, typename = std::enable_if_t<std::is_convertible_v<U*, T*>>>
        SharedRef(const SharedRef<U>& other) : mPtr(other.mPtr) {
            if (mPtr)
                mPtr->AddRef();
        }

        // Converting move constructor
        template <typename U, typename = std::enable_if_t<std::is_convertible_v<U*, T*>>>
        SharedRef(SharedRef<U>&& other) noexcept : mPtr(other.mPtr) {
            other.mPtr = nullptr;
        }

        ~SharedRef() {
            if (mPtr)
                mPtr->Release();
        }

        // Copy assignment
        SharedRef& operator=(const SharedRef& other) {
            if (this != &other) {
                if (mPtr)
                    mPtr->Release();
                mPtr = other.mPtr;
                if (mPtr)
                    mPtr->AddRef();
            }
            return *this;
        }

        // Move assignment
        SharedRef& operator=(SharedRef&& other) noexcept {
            if (this != &other) {
                if (mPtr)
                    mPtr->Release();
                mPtr = other.mPtr;
                other.mPtr = nullptr;
            }
            return *this;
        }

        // Converting copy assignment
        template <typename U, typename = std::enable_if_t<std::is_convertible_v<U*, T*>>>
        SharedRef& operator=(const SharedRef<U>& other) {
            if (mPtr != other.mPtr) {
                if (mPtr)
                    mPtr->Release();
                mPtr = other.mPtr;
                if (mPtr)
                    mPtr->AddRef();
            }
            return *this;
        }

        // Converting move assignment
        template <typename U, typename = std::enable_if_t<std::is_convertible_v<U*, T*>>>
        SharedRef& operator=(SharedRef<U>&& other) noexcept {
            if (mPtr != other.mPtr) {
                if (mPtr)
                    mPtr->Release();
                mPtr = other.mPtr;
                other.mPtr = nullptr;
            }
            return *this;
        }

        PYRO_NODISCARD PYRO_FORCEINLINE T* operator->() const noexcept { return mPtr; }
        PYRO_NODISCARD PYRO_FORCEINLINE T& operator*() const noexcept { return *mPtr; }
        PYRO_NODISCARD PYRO_FORCEINLINE T* Get() const noexcept { return mPtr; }
        PYRO_NODISCARD PYRO_FORCEINLINE explicit operator bool() const noexcept { return mPtr != nullptr; }

        PYRO_NODISCARD PYRO_FORCEINLINE bool operator==(const SharedRef&) const = default;
        PYRO_NODISCARD PYRO_FORCEINLINE bool operator!=(const SharedRef&) const = default;

        // Factory helper
        template <typename... Args>
        PYRO_NODISCARD PYRO_FORCEINLINE static SharedRef<T> Create(Args&&... args) {
            return SharedRef<T>(new T(eastl::forward<Args>(args)...));
        }

        // Give access to other template instantiations
        template <typename U>
        friend class SharedRef;

        operator T&() {
            return *mPtr;
        }  
        operator const T&() const {
            return *mPtr;
        }  
    };

}