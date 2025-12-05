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
#include <PyroCommon/Core.hpp>
namespace PyroshockStudios {
    enum struct StreamOrigin : i32 {
        Start,
        End,
        Current
    };
    struct IStreamBase {
        IStreamBase() = default;
        virtual ~IStreamBase() = default;

        /// Seeks to a point in a previously opened file.
        /// @param offset The number of bytes to seek.
        /// @param origin Seek origin
        /// @return True if the operation completed successfully, false otherwise.
        PYRO_NODISCARD virtual bool Seek(isize offset, StreamOrigin origin) = 0;

        /// Returns the current position of the file pointer.
        /// @param file The handle of the file to be queried.
        /// @return The number of bytes from the origin of the file.
        PYRO_NODISCARD virtual size_t Tell() = 0;

        /// Returns the length of the file.
        /// The default implementation uses Seek & Tell.
        /// @param file The handle of the file to be queried.
        /// @return The length of the file in bytes.
        PYRO_NODISCARD virtual usize Length() = 0;
    };
} // namespace PyroshockStudios