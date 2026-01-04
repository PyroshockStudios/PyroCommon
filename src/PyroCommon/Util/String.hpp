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
#include <EASTL/vector.h>
#include <PyroCommon/Types.hpp>

namespace PyroshockStudios {
    inline namespace Util {
        static eastl::vector<eastl::string> StringSplit(const eastl::string& input, char delimiter) {
            eastl::vector<eastl::string> result;
            usize start = 0;
            usize pos = 0;

            while ((pos = input.find(delimiter, start)) != eastl::string::npos) {
                result.push_back(input.substr(start, pos - start));
                start = pos + 1;
            }

            // Add the last segment
            if (start <= input.size()) {
                result.push_back(input.substr(start));
            }

            return result;
        }
        static eastl::vector<eastl::string> StringSplit(const eastl::string& input, char delimiter, char quote, char escapeMarker = '\\') {
            eastl::vector<eastl::string> tokens;
            eastl::string current;
            bool inQuotes = false;
            char quoteChar = '\0';

            for (size_t i = 0; i < input.size(); ++i) {
                char c = input[i];

                // Handle backslash escapes
                if (c == escapeMarker) {
                    if (i + 1 < input.size()) {
                        char nxt = input[i + 1];

                        // If we're in quotes: allow escaping the current quote or a backslash
                        if (inQuotes && (nxt == quoteChar || nxt == escapeMarker)) {
                            current.push_back(nxt); // add the escaped char, drop the backslash
                            ++i;
                            continue;
                        }

                        // If we're not in quotes: allow escaping quotes so they remain literal,
                        // without toggling quoted mode. Also allow escaping backslash.
                        if (!inQuotes && (nxt == quote || nxt == escapeMarker)) {
                            current.push_back(nxt);
                            ++i;
                            continue;
                        }
                    }

                    // Otherwise keep the backslash as-is
                    current.push_back(c);
                    continue;
                }

                // Handle quote chars
                if (c == quote) {
                    if (!inQuotes) {
                        inQuotes = true;
                        quoteChar = c;
                        current.push_back(c); // keep opening quote
                    } else if (c == quoteChar) {
                        inQuotes = false;
                        quoteChar = '\0';
                        current.push_back(c); // keep closing quote
                    } else {
                        current.push_back(c); // different quote inside quoted string
                    }
                    continue;
                }

                // Split on delimiter only when not inside quotes
                if (c == delimiter && !inQuotes) {
                    if (!current.empty()) {
                        tokens.push_back(current);
                        current.clear();
                    }
                    continue;
                }

                // Regular character
                current.push_back(c);
            }

            if (!current.empty()) {
                tokens.push_back(current);
            }

            return tokens;
        }
    } // namespace Util
} // namespace PyroshockStudios
