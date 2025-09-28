#pragma once

#include "ISerializable.hpp"
#include <SWCommon/Core.hpp>
#include <SWCommon/Stream/IStreamReader.hpp>
#include <SWCommon/Stream/IStreamWriter.hpp>

#include <EASTL/internal/copy_help.h>
#include <stdexcept>

namespace PyroshockStudios {
    template <typename T>
    concept SerializableConcept =
        eastl::is_base_of_v<ISerializable, T>;

    template <typename T>
    concept TriviallySerializableConcept =
        eastl::is_trivially_copyable_v<eastl::remove_cvref_t<T>> &&
        !eastl::is_function_v<eastl::remove_cvref_t<T>> &&
        !eastl::is_pointer_v<eastl::remove_cvref_t<T>> &&
        !SerializableConcept<T> &&
        !CollectionConcept<T> &&
        !MapConcept<T> &&
        !OptionalConcept<T>;

    template <typename T>
    concept PureCollectionConcept =
        CollectionConcept<T> &&
        !SerializableConcept<T> &&
        !MapConcept<T> &&
        !OptionalConcept<T>;

    class BinarySerializer : DeleteCopy {
    public:
        BinarySerializer(IStreamReader* reader, IStreamWriter* writer);
        ~BinarySerializer() = default;

        using SizeType = u64;

        template <TriviallySerializableConcept T>
        BinarySerializer& operator<<(const T& dataIn) {
            this->Write(reinterpret_cast<const void*>(&dataIn), sizeof(dataIn));
            return *this;
        }
        template <SerializableConcept T>
        BinarySerializer& operator<<(const T& serialIn) {
            serialIn.Serialize(*this);
            return *this;
        }
        template <PureCollectionConcept T>
        BinarySerializer& operator<<(const T& collectionIn) {
            using Elem = eastl::remove_reference_t<decltype(*collectionIn.data())>;
            *this << static_cast<SizeType>(collectionIn.size());
            if (collectionIn.size() > 0) {
                if constexpr (ContiguousCollectionConcept<T> && TriviallySerializableConcept<Elem>) {
                    Write(reinterpret_cast<const void*>(collectionIn.data()), collectionIn.size() * sizeof(Elem));
                } else {
                    for (const Elem& i : collectionIn) {
                        *this << i;
                    }
                }
            }
            return *this;
        }
        template <MapConcept T>
        BinarySerializer& operator<<(const T& mapIn) {
            using Key = typename T::key_type;
            using Value = typename T::mapped_type;

            *this << static_cast<SizeType>(mapIn.size());

            for (const auto& [key, value] : mapIn) {
                *this << key;
                *this << value;
            }
            return *this;
        }
        template <OptionalConcept T>
        BinarySerializer& operator<<(const T& optionalIn) {
            using Elem = eastl::remove_reference_t<decltype(*optionalIn)>;
            *this << static_cast<u8>(optionalIn.has_value());
            if (optionalIn.has_value()) {
                *this << *optionalIn;
            }
            return *this;
        }

        template <TriviallySerializableConcept T>
        BinarySerializer& operator>>(T& dataOut) {
            this->Read(reinterpret_cast<void*>(&dataOut), sizeof(T));
            return *this;
        }
        template <SerializableConcept T>
        BinarySerializer& operator>>(T& serialOut) {
            serialOut.Deserialize(*this);
            return *this;
        }
        template <PureCollectionConcept T>
        BinarySerializer& operator>>(T& collectionOut) {
            using Elem = eastl::remove_reference_t<decltype(*collectionOut.data())>;
            SizeType count = 0;
            *this >> count;
            // 2) Contiguous + resizable (vector-like): resize, then bulk/loop
            if constexpr (ContiguousCollectionConcept<T> && ResizableConcept<T>) {
                collectionOut.resize(static_cast<size_t>(count));
                if (count == 0)
                    return *this;

                if constexpr (TriviallySerializableConcept<Elem>) {
                    Read(reinterpret_cast<void*>(collectionOut.data()), static_cast<size_t>(count) * sizeof(Elem));
                } else {
                    for (auto& e : collectionOut) {
                        *this >> e;
                    }
                }
                return *this;
            }
            // 3) Contiguous + fixed-size (array-like): sizes must match (or choose a policy)
            else if constexpr (ContiguousCollectionConcept<T> && !ResizableConcept<T>) {
                const size_t N = collectionOut.size();

                if (static_cast<size_t>(count) != N) {
                    throw std::runtime_error("Mismatched array size in deserialization");
                }

                const size_t toRead = count;
                if (toRead == 0)
                    return *this;

                if constexpr (TriviallySerializableConcept<Elem>) {
                    Read(reinterpret_cast<void*>(collectionOut.data()), toRead * sizeof(Elem));
                } else {
                    // Read element-by-element for the portion that fits
                    for (usize i = 0; i < toRead; ++i) {
                        *this >> collectionOut[i];
                    }
                }
                return *this;
            }
            // 4) Non-contiguous but push-backable (list/deque, etc.)
            else if constexpr (EmplaceBackableConcept<T> && ClearableConcept<T>) {
                collectionOut.clear();
                collectionOut.reserve(static_cast<size_t>(count)); // if the type has reserve; harmless if not
                for (size_t i = 0; i < static_cast<size_t>(count); ++i) {
                    if constexpr (TriviallySerializableConcept<Elem>) {
                        Elem e{};
                        Read(reinterpret_cast<void*>(&e), sizeof(Elem));
                        collectionOut.emplace_back(eastl::move(e));
                    } else {
                        Elem e{};
                        *this >> e;
                        collectionOut.emplace_back(eastl::move(e));
                    }
                }
                return *this;
            } else {
                static_assert(false, "Deserialisation does not work for this container!");
            }
        }
        template <MapConcept T>
        BinarySerializer& operator>>(T& mapOut) {
            using Key = typename T::key_type;
            using Value = typename T::mapped_type;

            SizeType count = 0;
            *this >> count;

            mapOut.clear();
            for (SizeType i = 0; i < count; ++i) {
                Key key{};
                Value value{};
                *this >> key;
                *this >> value;
                mapOut.emplace(eastl::move(key), eastl::move(value));
            }
            return *this;
        }
        template <OptionalConcept T>
        BinarySerializer& operator>>(T& optionalOut) {
            u8 containsValue = 0;
            *this >> containsValue;
            if (containsValue == 0) {
                optionalOut = T{};
            } else {
                using Elem = eastl::remove_reference_t<decltype(*optionalOut)>;
                Elem containee;
                *this >> containee;
                optionalOut = T{ containee };
            }
            return *this;
        }

        // writes to BinarySerializer buffer
        PYRO_COMMON_API void Write(const void* bytes, usize size);

        // reads from the front of BinarySerializer and advances
        // This may throw an exception if the end of the buffer has been reached!
        PYRO_COMMON_API void Read(void* dataOut, usize size);

    private:
        IStreamReader* mReader = nullptr;
        IStreamWriter* mWriter = nullptr;
    };
}