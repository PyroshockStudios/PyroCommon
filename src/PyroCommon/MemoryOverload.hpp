#include <SWCommon/Core.hpp>

#ifdef PYRO_IMPLEMENT_NEW_OPERATOR

void* PYRO_CDECL operator new[](PyroshockStudios::usize size, const char* name, int flags, PyroshockStudios::u32 debugFlags, const char* file, int line) {
	return new PyroshockStudios::u8[size];
}

void* PYRO_CDECL operator new[](PyroshockStudios::usize  size, PyroshockStudios::usize , PyroshockStudios::usize , char const*, int,  PyroshockStudios::u32, char const*, int) {
    return new PyroshockStudios::u8[size];
}

#endif