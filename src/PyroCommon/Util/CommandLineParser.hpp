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
#include <EASTL/unordered_map.h>
#include <EASTL/vector.h>

#include <PyroCommon/Core.hpp>

namespace PyroshockStudios {
    inline namespace Util {
        class CommandLineParser {
        public:
            CommandLineParser(i32 argc, char* argv[], i32 firstIndex = 1) {
                ParseArguments(argc, argv, firstIndex);
            }
            CommandLineParser(const eastl::vector<eastl::string>& keyValues) {
                ParseArguments(keyValues);
            }

            // Returns the value or a default if not found
            eastl::string GetOption(const eastl::string& key, const eastl::string& defaultValue = "") const;

            i64 GetIntOption(const eastl::string& key, i64 defaultValue = 0) const;

            // Parses boolean values from flags like -tracy=true
            bool GetBoolOption(const eastl::string& key, bool defaultValue = false) const;

            eastl::vector<eastl::string> GetArrayOption(const eastl::string& key, const eastl::vector<eastl::string>& defaultValue = {}) const;

            // Checks if a key was provided
            bool HasOption(const eastl::string& key) const;

        private:
            eastl::unordered_map<eastl::string, eastl::string> mOptions;

            inline bool ParseArg(eastl::string& arg);
            void ParseArguments(const eastl::vector<eastl::string>& keyValues);
            void ParseArguments(i32 argc, char* argv[], i32 firstIndex);
        };
    } // namespace Util
} // namespace PyroshockStudios
