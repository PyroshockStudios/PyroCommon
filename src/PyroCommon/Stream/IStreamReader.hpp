#pragma once
#include <SWCommon/Core.hpp>
namespace PyroshockStudios {
    struct IStreamReader {
        IStreamReader() = default;
        virtual ~IStreamReader() = default;

        // Read and advance
        virtual void Read(void* out, usize size) = 0;

        // Read and advance
        virtual void ReadToEnd(void* out) = 0;

        // Gets size of the stream size. Note: for IO streams such as file streams, this can cause a block!
        PYRO_NODISCARD virtual usize StreamSize() = 0;

        virtual bool IsEndOfStream() = 0;
    };
}