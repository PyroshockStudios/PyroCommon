#pragma once
#include <SWCommon/Core.hpp>
namespace PyroshockStudios {
    struct IStreamWriter {
        IStreamWriter() = default;
        virtual ~IStreamWriter() = default;

        // Write and advance
        virtual void Write(const void* in, usize size) = 0;
    };
}