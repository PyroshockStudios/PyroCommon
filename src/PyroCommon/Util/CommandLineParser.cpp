#include "CommandLineParser.hpp"

#include <EASTL/algorithm.h>

#include <SWCommon/Platform.hpp>

#ifdef PYRO_PLATFORM_WINDOWS
#define PYRO_STR_TO_INT64 strtoll
#else
#define PYRO_STR_TO_INT64 strtol
#endif
namespace PyroshockStudios {
    inline namespace Util {
        eastl::string CommandLineParser::GetOption(const eastl::string& key, const eastl::string& defaultValue) const {
            auto it = mOptions.find(key);
            return (it != mOptions.end()) ? it->second : defaultValue;
        }

        i64 CommandLineParser::GetIntOption(const eastl::string& key, i64 defaultValue) const {
            auto it = mOptions.find(key);
            if (it != mOptions.end()) {
                const eastl::string& val = it->second;
                char* endPtr = nullptr;
                i64 result = static_cast<i64>(PYRO_STR_TO_INT64(val.c_str(), &endPtr, 10));
                if (endPtr != val.c_str() && *endPtr == '\0') {
                    return result;
                }
            }
            return defaultValue;
        }

        bool CommandLineParser::GetBoolOption(const eastl::string& key, bool defaultValue) const {
            auto it = mOptions.find(key);
            if (it != mOptions.end()) {
                eastl::string val = it->second;
                eastl::transform(val.begin(), val.end(), val.begin(), [](char c) { return static_cast<char>(tolower(c)); });
                return (val == "1" || val == "true" || val == "yes" || val == "on");
            }
            return defaultValue;
        }

        eastl::vector<eastl::string> CommandLineParser::GetArrayOption(const eastl::string& key, const eastl::vector<eastl::string>& defaultValue) const {
            auto it = mOptions.find(key);
            if (it != mOptions.end()) {
                const auto& input = it->second;
                eastl::vector<eastl::string> tokens;
                eastl::string current;

                bool inApostrophes = false;

                for (usize i = 0; i < input.length(); ++i) {
                    char c = input[i];

                    if (c == '\'') {
                        inApostrophes = !inApostrophes;
                    } else if (c == ' ' && !inApostrophes) {
                        if (!current.empty()) {
                            tokens.push_back(current);
                            current.clear();
                        }
                    } else {
                        current += c;
                    }
                }

                // Always push the last token if non-empty
                if (!current.empty()) {
                    tokens.push_back(current);
                }

                return tokens;
            }
            return defaultValue;
        }

        bool CommandLineParser::HasOption(const eastl::string& key) const {
            return mOptions.find(key) != mOptions.end();
        }


        bool CommandLineParser::ParseArg(eastl::string& arg) {
            if (!arg.empty() && arg[0] == '-') {
                arg.erase(0, 1); // Remove leading '-'
                if (arg.empty())
                    return false; // Skip lone '-'

                size_t equalPos = arg.find('=');
                if (equalPos != eastl::string::npos) {
                    eastl::string key = arg.substr(0, equalPos);
                    eastl::string value = arg.substr(equalPos + 1);

                    if (!value.empty() && value.front() == '"' && value.back() == '"') {
                        value = value.substr(1, value.size() - 2);
                    }

                    if (!key.empty())
                        mOptions[key] = value;
                } else {
                    mOptions[arg] = eastl::string(); // Flag without value
                }
            }
            return true;
        }
        void CommandLineParser::ParseArguments(const eastl::vector<eastl::string>& keyValues) {
            for (i32 i = 0; i < keyValues.size(); ++i) {
                eastl::string arg = keyValues[i];
                if (!ParseArg(arg))
                    continue;
            }
        }
        void CommandLineParser::ParseArguments(i32 argc, char* argv[], i32 firstIndex) {
            for (i32 i = firstIndex; i < argc; ++i) {
                eastl::string arg = argv[i];
                if (!ParseArg(arg))
                    continue;
            }
        }
    }
}