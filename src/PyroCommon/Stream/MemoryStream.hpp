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
#include "IStreamReader.hpp"
#include "IStreamWriter.hpp"
#include <PyroCommon/Core.hpp>

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
} // namespace PyroshockStudios