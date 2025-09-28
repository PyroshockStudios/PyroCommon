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

#include "GUID.hpp"
#include <EASTL/string.h>
#include <iomanip>
#include <random>
#include <sstream>

namespace PyroshockStudios {
    PYRO_COMMON_API GUID::GUID() noexcept {
        static std::random_device rd;
        static std::mt19937_64 gen(rd());
        static std::uniform_int_distribution<u32> dist;

        for (usize i = 0; i < 16; i += 4) {
            u32 randVal = dist(gen);
            mBytes[i] = static_cast<u8>(randVal >> 24);
            mBytes[i + 1] = static_cast<u8>((randVal >> 16) & 0xFF);
            mBytes[i + 2] = static_cast<u8>((randVal >> 8) & 0xFF);
            mBytes[i + 3] = static_cast<u8>(randVal & 0xFF);
        }

        // Set version to 4 --- UUID version 4 (random)
        mBytes[6] = (mBytes[6] & 0x0F) | 0x40;

        // Set variant to 10xx (RFC 4122 variant)
        mBytes[8] = (mBytes[8] & 0x3F) | 0x80;
    }

    PYRO_COMMON_API eastl::string GUID::ToString() const noexcept {
        std::ostringstream oss;
        oss << std::hex << std::setfill('0');
        for (u8 i = 0; i < mBytes.size(); ++i) {
            oss << std::setw(2) << static_cast<i32>(mBytes[i]);
            if (i == 3 || i == 5 || i == 7 || i == 9) {
                oss << '-';
            }
        }
        return eastl::string(oss.str().c_str());
    }

    PYRO_COMMON_API GUID GUID::FromString(const char* str) {
        std::istringstream iss(str);
        std::string part;
        GUID guid;
        usize byteIdx = 0;

        while (std::getline(iss, part, '-')) {
            for (usize i = 0; i < part.length(); i += 2) {
                std::string byteStr = part.substr(i, 2);
                guid.mBytes[byteIdx++] = static_cast<u8>(std::stoi(byteStr, nullptr, 16));
            }
        }
        return guid;
    }

} // namespace PyroshockStudios