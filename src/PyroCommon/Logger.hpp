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
#include <EASTL/internal/move_help.h>
#include <EASTL/string.h>
#include <PyroCommon/Core.hpp>
#include <PyroCommon/LoggerInterface.hpp>
#include <fmt/format.h>

namespace PyroshockStudios {
    class Logger {
    public:
        template <typename... Args>
        static void LogFmt(LogSeverity severity, ILogStream* stream, fmt::format_string<Args...> format_str, Args&&... args) {
            if (!stream || severity < stream->MinSeverity())
                return;
            auto result = fmt::format(format_str, std::forward<Args>(args)...);
            stream->Log(severity, result.c_str());
        }

        template <typename... Args>
        PYRO_FORCEINLINE static void Verbose(ILogStream* stream, fmt::format_string<Args...> format_str, Args&&... args) {
#ifndef PYRO_COMMON_DISABLE_VERBOSE_LOGGING
            LogFmt(LogSeverity::Verbose, stream, eastl::move(format_str), std::forward<Args>(args)...);
#endif
        }
        template <typename... Args>
        PYRO_FORCEINLINE static void Debug(ILogStream* stream, fmt::format_string<Args...> format_str, Args&&... args) {
            LogFmt(LogSeverity::Debug, stream, eastl::move(format_str), std::forward<Args>(args)...);
        }
        template <typename... Args>
        PYRO_FORCEINLINE static void Trace(ILogStream* stream, fmt::format_string<Args...> format_str, Args&&... args) {
            LogFmt(LogSeverity::Trace, stream, eastl::move(format_str), std::forward<Args>(args)...);
        }
        template <typename... Args>
        PYRO_FORCEINLINE static void Info(ILogStream* stream, fmt::format_string<Args...> format_str, Args&&... args) {
            LogFmt(LogSeverity::Info, stream, eastl::move(format_str), std::forward<Args>(args)...);
        }
        template <typename... Args>
        PYRO_FORCEINLINE static void Warn(ILogStream* stream, fmt::format_string<Args...> format_str, Args&&... args) {
            LogFmt(LogSeverity::Warn, stream, eastl::move(format_str), std::forward<Args>(args)...);
        }
        template <typename... Args>
        PYRO_FORCEINLINE static void Error(ILogStream* stream, fmt::format_string<Args...> format_str, Args&&... args) {
            LogFmt(LogSeverity::Error, stream, eastl::move(format_str), std::forward<Args>(args)...);
        }
        template <typename... Args>
        PYRO_FORCEINLINE static void Fatal(ILogStream* stream, fmt::format_string<Args...> format_str, Args&&... args) {
            LogFmt(LogSeverity::Fatal, stream, eastl::move(format_str), std::forward<Args>(args)...);
        }
    };
} // namespace PyroshockStudios