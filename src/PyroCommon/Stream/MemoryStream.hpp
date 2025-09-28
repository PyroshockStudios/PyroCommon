#pragma once
#include "IStreamReader.hpp"
#include "IStreamWriter.hpp"
#include <SWCommon/Core.hpp>

#include <EASTL/span.h>
#include <EASTL/vector.h>
#include <stdexcept>
#include <string.h>

namespace PyroshockStudios {
    class MemoryStream : public IStreamReader, public IStreamWriter {
    public:
        MemoryStream() = default;
        ~MemoryStream() = default;

        void Write(const void* bytes, usize size) override {
            const u8* src = static_cast<const u8*>(bytes);
            mBuffer.insert(mBuffer.end(), src, src + size);
            mPosition += size;
        }

        void Read(void* out, usize size) override {
            if (mPosition + size > mBuffer.size()) {
                throw std::runtime_error("Buffer underflow");
            }
            memcpy(out, mBuffer.data() + mPosition, size);
            mPosition += size;
        }

        void ReadToEnd(void* out) override {
            usize remaining = StreamSize() - mPosition;
            Read(out, remaining);
        }

        usize StreamSize() override {
            return mBuffer.size();
        }

        bool IsEndOfStream() override {
            return mPosition >= mBuffer.size();
        }

        void Seek(usize position) {
            if (position > mBuffer.size()) {
                throw std::runtime_error("Buffer overflow");
            }
            mPosition = position;
        }

        eastl::span<const u8> Span() const { return { mBuffer.data(), mBuffer.size() }; }

    private:
        eastl::vector<u8> mBuffer;
        usize mPosition = 0;
    };
}