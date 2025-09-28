#include "Version.hpp"
#include <cstdio>

using namespace PyroshockStudios;
using namespace PyroshockStudios::Types;
constexpr static u32 BUILD_VERSION = {
    // YYYY
    u32(__DATE__[7] - 0x30) * 10000000 + u32(__DATE__[8] - 0x30) * 1000000 + u32(__DATE__[9] - 0x30) * 100000 + u32(__DATE__[10] - 0x30) * 10000 +

    // MM
    (__DATE__[0] == 'O' || __DATE__[0] == 'N' || __DATE__[0] == 'D') * 1000 + // if greater/equal to 10
    ((__DATE__[0] == 'J')      ? ((__DATE__[1] == 'a') ? 100 :                // Jan, Jun or Jul
                                    ((__DATE__[2] == 'n') ? 600 : 700))
        : (__DATE__[0] == 'F') ? 200
                               : // Feb
            (__DATE__[0] == 'M') ? (__DATE__[2] == 'r') ? 300 : 500
                                 : // Mar or May
            (__DATE__[0] == 'A') ? (__DATE__[1] == 'p') ? 400 : 800
                                 : // Apr or Aug
            (__DATE__[0] == 'S') ? 900
                                 : // Sep
            // october adds nothing
            (__DATE__[0] == 'N') ? 100
                                 : // Nov
            (__DATE__[0] == 'D') ? 200
                                 : // Dec
            0) +

    // DD
    (__DATE__[4] == ' ' ? 0 : uint32_t(__DATE__[4] - 0x30)) * 10 + uint32_t(__DATE__[5] - 0x30)
};

static constexpr Version gEngineVersion = { 1, 0, 0, BUILD_VERSION };

namespace PyroshockStudios {
    eastl::string Version::ToString() const {
        char buffer[64];
        snprintf(buffer, sizeof(buffer), "%u.%u.%u (Build %u)",
            Major(), Minor(), Revision(), BuildDate());
        return { buffer };
    }

    Version Version::CurrentVersion() {
        return gEngineVersion;
    }
}