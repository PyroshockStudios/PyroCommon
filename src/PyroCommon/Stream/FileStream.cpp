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

#include "FileStream.hpp"

// --- Platform Specific Headers ---
#if defined(PYRO_PLATFORM_FAMILY_WINDOWS)
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#elif defined(PYRO_PLATFORM_FAMILY_UNIX)
#include <fcntl.h>
#include <sys/stat.h>
#include <unistd.h>
#else
#error "Missing Filestream.cpp implementation for this platform!"
#endif

namespace PyroshockStudios {

    // Helper to initialize private members that might not be in the header snippet provided
    // Assuming mHandle/mFd and mSeekPos exist in the class private section based on previous context.

    FileStream::FileStream(const eastl::string& path, Encoding encoding, Mode mode)
        : mSeekPos(0) {
#if defined(PYRO_PLATFORM_FAMILY_WINDOWS)
        mHandle = INVALID_HANDLE_VALUE; // Ensure default state

        DWORD access = 0;
        DWORD share = FILE_SHARE_READ;
        DWORD creation = OPEN_EXISTING;

        switch (mode) {
        case Mode::ReadOnly:
            access = GENERIC_READ;
            break;
        case Mode::WriteOnly:
            access = GENERIC_WRITE;
            creation = CREATE_ALWAYS;
            break;
        case Mode::ReadWrite:
            access = GENERIC_READ | GENERIC_WRITE;
            creation = OPEN_ALWAYS;
            break;
        }

        // FILE_FLAG_OVERLAPPED allows for async operations, even if we are blocking here
        mHandle = CreateFileA(path.c_str(), access, share, nullptr, creation,
            FILE_FLAG_OVERLAPPED | FILE_ATTRIBUTE_NORMAL, nullptr);

#elif defined(PYRO_PLATFORM_FAMILY_UNIX)
        mFd = -1; // Ensure default state

        int flags = 0;
        switch (mode) {
        case Mode::ReadOnly:
            flags = O_RDONLY;
            break;
        case Mode::WriteOnly:
            flags = O_WRONLY | O_CREAT | O_TRUNC;
            break;
        case Mode::ReadWrite:
            flags = O_RDWR | O_CREAT;
            break;
        }

        // 0644 permissions
        mFd = open(path.c_str(), flags, S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);
#endif
    }

    FileStream::~FileStream() {
#if defined(PYRO_PLATFORM_FAMILY_WINDOWS)
        if (mHandle != INVALID_HANDLE_VALUE) {
            CloseHandle(mHandle);
            mHandle = INVALID_HANDLE_VALUE;
        }
#elif defined(PYRO_PLATFORM_FAMILY_UNIX)
        if (mFd != -1) {
            close(mFd);
            mFd = -1;
        }
#endif
    }

    // ---------------------------------------------------------
    // IStreamBase Implementation
    // ---------------------------------------------------------

    bool FileStream::Seek(isize offset, StreamOrigin origin) {
#if defined(PYRO_PLATFORM_FAMILY_WINDOWS)
        if (mHandle == INVALID_HANDLE_VALUE)
            return false;
#elif defined(PYRO_PLATFORM_FAMILY_UNIX)
        if (mFd == -1)
            return false;
#endif

        usize newPos = 0;
        usize len = Length();

        switch (origin) {
        case StreamOrigin::Start:
            if (offset < 0)
                return false;
            newPos = static_cast<usize>(offset);
            break;
        case StreamOrigin::End:
            if (offset > 0)
                return false; // Usually can't seek past end with +offset from End
            // offset is negative here, so we subtract its absolute value
            if (static_cast<usize>(-offset) > len)
                return false;
            newPos = len + offset;
            break;
        case StreamOrigin::Current:
            if (offset < 0) {
                if (static_cast<usize>(-offset) > mSeekPos)
                    return false;
                newPos = mSeekPos + offset;
            } else {
                newPos = mSeekPos + offset;
            }
            break;
        }

        // Since we use Overlapped IO (Win32) or pread (POSIX),
        // we mainly track the logical pointer in mSeekPos.
        mSeekPos = newPos;
        return true;
    }

    usize FileStream::Tell() {
        return mSeekPos;
    }

    usize FileStream::Length() {
#if defined(PYRO_PLATFORM_FAMILY_WINDOWS)
        if (mHandle == INVALID_HANDLE_VALUE)
            return 0;
        LARGE_INTEGER fileSize;
        if (!GetFileSizeEx(mHandle, &fileSize)) {
            return 0;
        }
        return static_cast<usize>(fileSize.QuadPart);
#elif defined(PYRO_PLATFORM_FAMILY_UNIX)
        if (mFd == -1)
            return 0;
        struct stat st;
        if (fstat(mFd, &st) < 0) {
            return 0;
        }
        return static_cast<usize>(st.st_size);
#endif
    }

    // ---------------------------------------------------------
    // IStreamReader Implementation
    // ---------------------------------------------------------

    usize FileStream::Read(void* out, usize size) {
        if (size == 0)
            return 0;

#if defined(PYRO_PLATFORM_FAMILY_WINDOWS)
        if (mHandle == INVALID_HANDLE_VALUE)
            return 0;

        OVERLAPPED ol = {};
        ol.Offset = static_cast<DWORD>(mSeekPos & 0xFFFFFFFF);
        ol.OffsetHigh = static_cast<DWORD>((mSeekPos >> 32) & 0xFFFFFFFF);

        DWORD bytesRead = 0;
        // Try reading
        if (!ReadFile(mHandle, out, static_cast<DWORD>(size), nullptr, &ol)) {
            if (GetLastError() != ERROR_IO_PENDING) {
                // Real failure
                return 0;
            }
            // Async pending, wait for result
            if (!GetOverlappedResult(mHandle, &ol, &bytesRead, TRUE)) {
                return 0;
            }
        } else {
            // Completed immediately, get result (without waiting, since it's done)
            GetOverlappedResult(mHandle, &ol, &bytesRead, FALSE);
        }

        mSeekPos += bytesRead;
        return static_cast<usize>(bytesRead);

#elif defined(PYRO_PLATFORM_FAMILY_UNIX)
        if (mFd == -1)
            return 0;

        // pread reads from specific offset without modifying underlying file descriptor pointer
        ssize_t result = pread(mFd, out, size, mSeekPos);
        if (result < 0) {
            return 0;
        }

        mSeekPos += result;
        return static_cast<usize>(result);
#endif
    }

    // ---------------------------------------------------------
    // IStreamWriter Implementation
    // ---------------------------------------------------------

    usize FileStream::Write(const void* in, usize size) {
        if (size == 0)
            return 0;

#if defined(PYRO_PLATFORM_FAMILY_WINDOWS)
        if (mHandle == INVALID_HANDLE_VALUE)
            return 0;

        OVERLAPPED ol = {};
        ol.Offset = static_cast<DWORD>(mSeekPos & 0xFFFFFFFF);
        ol.OffsetHigh = static_cast<DWORD>((mSeekPos >> 32) & 0xFFFFFFFF);

        DWORD bytesWritten = 0;
        if (!WriteFile(mHandle, in, static_cast<DWORD>(size), nullptr, &ol)) {
            if (GetLastError() != ERROR_IO_PENDING) {
                return 0;
            }
            if (!GetOverlappedResult(mHandle, &ol, &bytesWritten, TRUE)) {
                return 0;
            }
        } else {
            GetOverlappedResult(mHandle, &ol, &bytesWritten, FALSE);
        }

        mSeekPos += bytesWritten;
        return static_cast<usize>(bytesWritten);

#elif defined(PYRO_PLATFORM_FAMILY_UNIX)
        if (mFd == -1)
            return 0;

        ssize_t result = pwrite(mFd, in, size, mSeekPos);
        if (result < 0) {
            return 0;
        }

        mSeekPos += result;
        return static_cast<usize>(result);
#endif
    }

    bool FileStream::Resize(usize bytes) {
#if defined(PYRO_PLATFORM_FAMILY_WINDOWS)
        if (mHandle == INVALID_HANDLE_VALUE)
            return false;

        // On Windows, SetEndOfFile truncates at the CURRENT file pointer.
        // Since we use Overlapped IO, our mSeekPos is logical.
        // We must physically move the pointer to the desired size, truncate,
        // then arguably we don't need to move it back because Overlapped ignores it,
        // but it is good practice to not leave it dangling.

        LARGE_INTEGER li;
        li.QuadPart = static_cast<LONGLONG>(bytes);

        // 1. Move physical pointer to new size
        if (!SetFilePointerEx(mHandle, li, nullptr, FILE_BEGIN)) {
            return false;
        }

        // 2. Set End Of File
        if (!SetEndOfFile(mHandle)) {
            return false;
        }

        return true;

#elif defined(PYRO_PLATFORM_FAMILY_UNIX)
        if (mFd == -1)
            return false;

        if (ftruncate(mFd, static_cast<off_t>(bytes)) != 0) {
            return false;
        }
        return true;
#endif
    }

} // namespace PyroshockStudios