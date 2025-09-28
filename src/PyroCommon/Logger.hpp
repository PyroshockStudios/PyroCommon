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
#include <EASTL/string.h>
#include <cstdio>
namespace PyroshockStudios {
    class Logger {
    public:
        template <typename... Args>
        static void Trace(const eastl::string& str, Args... args) { printf("[TRACE] %s\n", str.c_str()); }
        template <typename... Args>
        static void Warn(const eastl::string& str, Args... args) { printf("[WARN] %s\n", str.c_str()); }
        template <typename... Args>
        static void Info(const eastl::string& str, Args... args) { printf("[INFO] %s\n", str.c_str()); }
        template <typename... Args>
        static void Error(const eastl::string& str, Args... args) { printf("[ERROR] %s\n", str.c_str()); }
        template <typename... Args>
        static void Fatal(const eastl::string& str, Args... args) {
            printf("[FATAL] %s\n", str.c_str());
            abort();
        }
    };
} // namespace PyroshockStudios