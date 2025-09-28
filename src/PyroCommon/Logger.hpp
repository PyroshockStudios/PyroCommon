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
}