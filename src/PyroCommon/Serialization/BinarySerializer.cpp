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

#include "BinarySerializer.hpp"
#include <libassert/assert.hpp>

namespace PyroshockStudios {
    PYRO_COMMON_API BinarySerializer::BinarySerializer(IStreamReader* reader, IStreamWriter* writer) : mReader(reader), mWriter(writer) {}
    PYRO_COMMON_API void BinarySerializer::Write(const void* bytes, usize size) {
        ASSERT(mWriter != nullptr, "Stream Writer has not been set!");
        mWriter->Write(bytes, size);
    }
    PYRO_COMMON_API void BinarySerializer::Read(void* out, usize size) {
        ASSERT(mReader != nullptr, "Stream Reader has not been set!");
        mReader->Read(out, size);
    }
} // namespace PyroshockStudios
