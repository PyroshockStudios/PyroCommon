#pragma once
#include <SWCommon/Core.hpp>
namespace PyroshockStudios {
    class BinarySerializer;
    struct PYRO_NO_VTABLE ISerializable {
        ISerializable() = default;

        virtual void Serialize(BinarySerializer& serializer) const = 0;
        virtual void Deserialize(BinarySerializer& serializer) = 0;
    };
}