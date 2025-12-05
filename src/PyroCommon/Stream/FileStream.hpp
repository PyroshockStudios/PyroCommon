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

#include <EASTL/string.h>

#ifdef PYRO_PLATFORM_FAMILY_WINDOWS
typedef void* HANDLE;
#endif
namespace PyroshockStudios {
    class FileStream : public IStreamWriter, public IStreamReader, DeleteCopy, DeleteMove {
    public:
        enum struct Mode {
            // To read from an existing file
            ReadOnly,
            // To write/create a file
            WriteOnly,
            // To read/write from an existing file. Note, the file MUST exist!
            ReadWrite
        };
        enum struct Encoding {
            Binary,
            Text
        };

        FileStream(const eastl::string& path, Encoding encoding, Mode mode);

        ~FileStream();

        PYRO_NODISCARD bool Seek(isize offset, StreamOrigin origin) override;
        PYRO_NODISCARD usize Length() override;
        PYRO_NODISCARD usize Tell() override;

        PYRO_NODISCARD bool Resize(usize bytes) override;
        PYRO_NODISCARD usize Write(const void* bytes, usize size) override;

        PYRO_NODISCARD usize Read(void* out, usize size) override;

    private:
        usize mSeekPos = 0;

#ifdef PYRO_PLATFORM_FAMILY_WINDOWS
        HANDLE mHandle = {};
#elifdef PYRO_PLATFORM_FAMILY_UNIX
        int mHandle = {};
#else
#endif
    };
} // namespace PyroshockStudios