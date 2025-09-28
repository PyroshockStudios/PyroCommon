#pragma once

#include <EASTL/string.h>
#include <EASTL/unordered_map.h>
#include <EASTL/vector.h>

#include <SWCommon/Core.hpp>

namespace PyroshockStudios {
    class PYRO_COMMON_API CommandLineParser {
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

}
