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
#include <PyroCommon/Core.hpp>

#include <fstream>

namespace PyroshockStudios {
    class FileStream : public IStreamWriter, public IStreamReader {
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

        FileStream(const eastl::string& path, Encoding encoding, Mode mode) {
            std::ios_base::openmode flags = {};
            switch (encoding) {
            case Encoding::Binary:
                flags |= std::ios::binary;
                break;
            case Encoding::Text:
                break;
            default:
                throw std::runtime_error("Invalid config");
            }
            switch (mode) {
            case Mode::ReadOnly:
                flags |= std::ios::in;
                break;
            case Mode::WriteOnly:
                flags |= std::ios::out;
                break;
            case Mode::ReadWrite:
                flags |= std::ios::out | std::ios::in;
                break;
            default:
                throw std::runtime_error("Invalid config");
            }
            mStream.open(path.c_str(), flags);
            if (!mStream)
                throw std::runtime_error("Failed to open file: " + std::string(path.c_str()));
        }

        ~FileStream() {
            Flush();
            Close();
        }

        void Write(const void* bytes, size_t size) override {
            if (!mStream.write(reinterpret_cast<const char*>(bytes), size))
                throw std::runtime_error("File write failed");
            mSeekPos += size;
        }

        void Read(void* out, size_t size) override {
            if (IsEndOfStream())
                throw std::runtime_error("End of file reached");
            if (!mStream.read(reinterpret_cast<char*>(out), size))
                throw std::runtime_error("File read failed");
            mSeekPos += size;
        }


        void ReadToEnd(void* out) override {
            usize remaining = StreamSize() - mSeekPos;
            Read(out, remaining);
        }

        usize StreamSize() override {
            auto currentPos = mStream.tellg();
            mStream.seekg(0, std::ios::end);
            auto endPos = mStream.tellg();
            mStream.seekg(currentPos);
            if (!mStream) {
                throw std::runtime_error("Failed to get stream size!");
            }
            return static_cast<usize>(endPos);
        }

        bool IsEndOfStream() override {
            return mStream.eof();
        }

        void Flush() {
            mStream.flush();
        }
        void Close() {
            mStream.close();
        }

    private:
        std::fstream mStream;
        usize mSeekPos = 0;
    };

} // namespace PyroshockStudios