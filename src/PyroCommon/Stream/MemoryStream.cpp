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
#include "MemoryStream.hpp"

#include <libassert/assert.hpp>
#include <string.h>

namespace PyroshockStudios {
    bool MemoryStream::Resize(usize bytes) {
        mBuffer.resize(mBuffer.size() + bytes);
        return true;
    }
    usize MemoryStream::Write(const void* bytes, usize size) {
        const u8* src = static_cast<const u8*>(bytes);
        mBuffer.insert(mBuffer.begin() + mPosition, src, src + size);
        mPosition += size;
        return size;
    }

    usize MemoryStream::Read(void* out, usize size) {
        usize readSize = std::min(mBuffer.size() - mPosition, size);
        memcpy(out, mBuffer.data() + mPosition, readSize);
        mPosition += readSize;
        return readSize;
    }


    bool MemoryStream::Seek(isize offset, StreamOrigin origin) {
        switch (origin) {
        case StreamOrigin::Start:
            ASSERT(offset >= 0, "Start offset must be positive!");
            if (offset < 0)
                return false;
            if (offset >= mBuffer.size()) {
                return false;
            }
            mPosition = offset;
            return true;
        case StreamOrigin::End:
            ASSERT(offset >= 0, "End offset must be positive!");
            if (offset < 0)
                return false;
            if ((mBuffer.size() + offset) < 0) {
                return false;
            }
            mPosition = mBuffer.size() - offset;
            return true;
        case StreamOrigin::Current:
            if ((mPosition + offset) < 0 || (mPosition + offset) >= mBuffer.size()) {
                return false;
            }
            mPosition += offset;
            return true;
        default:
            return false;
        }
    }

    usize MemoryStream::Length() {
        return mBuffer.size();
    }

    usize MemoryStream::Tell(){
        return mPosition;
    }

    eastl::span<const u8> MemoryStream::Span() const { return { mBuffer.data(), mBuffer.size() }; }

} // namespace PyroshockStudios