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
#include <PyroCommon/Core.hpp>
#include <PyroCommon/LoggerInterface.hpp>
#include <fmt/format.h>

namespace PyroshockStudios {
    class Logger {
    public:
        template <typename... Args>
        PYRO_FORCEINLINE static void LogFmt(LogSeverity severity, const ILogStream* stream, const eastl::string& str, Args... args) {
			if (stream == nullptr || severity < stream->MinSeverity()) return;
            auto result = fmt::format(str, eastl::forward<Args>(args)...);
            stream->Log(severity, result.c_str());
        }

        template <typename... Args>
        PYRO_FORCEINLINE static void Trace(const ILogStream* stream, const eastl::string& str, Args... args) {
            LogFmt(LogSeverity::Trace, stream, str, eastl::forward<Args>(args));
        }
        template <typename... Args>
        PYRO_FORCEINLINE static void Info(const ILogStream* stream, const eastl::string& str, Args... args) {
            LogFmt(LogSeverity::Info, stream, str, eastl::forward<Args>(args));
        }
        template <typename... Args>
        PYRO_FORCEINLINE static void Warn(const ILogStream* stream, const eastl::string& str, Args... args) {
            LogFmt(LogSeverity::Warn, stream, str, eastl::forward<Args>(args));
        }
        template <typename... Args>
        PYRO_FORCEINLINE static void Error(const ILogStream* stream, const eastl::string& str, Args... args) {
            LogFmt(LogSeverity::Error, stream, str, eastl::forward<Args>(args));
        }
        template <typename... Args>
        PYRO_FORCEINLINE static void Fatal(const ILogStream* stream, const eastl::string& str, Args... args) {
            LogFmt(LogSeverity::Fatal, stream, str, eastl::forward<Args>(args));
        }
    };
} // namespace PyroshockStudios