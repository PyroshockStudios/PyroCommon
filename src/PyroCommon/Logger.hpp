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
        PYRO_FORCEINLINE static void LogFmt(LogSeverity severity, ILogStream* stream, eastl::string_view str, Args&&... args) {
            if (!stream || severity < stream->MinSeverity())
                return;
            auto result = fmt::format(fmt::runtime(str), std::forward<Args>(args)...);
            stream->Log(severity, result.c_str());
        }

        template <typename... Args>
        PYRO_FORCEINLINE static void Verbose(ILogStream* stream, eastl::string_view str, Args&&... args) {
#ifndef PYRO_COMMON_DISABLE_VERBOSE_LOGGING
            LogFmt(LogSeverity::Verbose, stream, str, std::forward<Args>(args)...);
#endif
        }
        template <typename... Args>
        PYRO_FORCEINLINE static void Debug(ILogStream* stream, eastl::string_view str, Args&&... args) {
            LogFmt(LogSeverity::Debug, stream, str, std::forward<Args>(args)...);
        }
        template <typename... Args>
        PYRO_FORCEINLINE static void Trace(ILogStream* stream, eastl::string_view str, Args&&... args) {
            LogFmt(LogSeverity::Trace, stream, str, std::forward<Args>(args)...);
        }
        template <typename... Args>
        PYRO_FORCEINLINE static void Info(ILogStream* stream, eastl::string_view str, Args&&... args) {
            LogFmt(LogSeverity::Info, stream, str, std::forward<Args>(args)...);
        }
        template <typename... Args>
        PYRO_FORCEINLINE static void Warn(ILogStream* stream, eastl::string_view str, Args&&... args) {
            LogFmt(LogSeverity::Warn, stream, str, std::forward<Args>(args)...);
        }
        template <typename... Args>
        PYRO_FORCEINLINE static void Error(ILogStream* stream, eastl::string_view str, Args&&... args) {
            LogFmt(LogSeverity::Error, stream, str, std::forward<Args>(args)...);
        }
        template <typename... Args>
        PYRO_FORCEINLINE static void Fatal(ILogStream* stream, eastl::string_view str, Args&&... args) {
            LogFmt(LogSeverity::Fatal, stream, str, std::forward<Args>(args)...);
        }
    };
} // namespace PyroshockStudios