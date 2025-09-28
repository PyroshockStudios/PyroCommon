#pragma once
#include "IStreamReader.hpp"
#include "IStreamWriter.hpp"

#include <SWCommon/Core.hpp>

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

}