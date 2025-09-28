#pragma once

// ----------------------
// Platform Family
// ----------------------

// #define PYRO_PLATFORM_FAMILY_WINDOWS 0
// #define PYRO_PLATFORM_FAMILY_UNIX 0
// #define PYRO_PLATFORM_FAMILY_CONSOLE 0

// ----------------------
// Specific Platforms
// ----------------------

// #define PYRO_PLATFORM_WINDOWS 0
// #define PYRO_PLATFORM_LINUX 0
// #define PYRO_PLATFORM_MACOS 0
// #define PYRO_PLATFORM_PS5 0
// #define PYRO_PLATFORM_XBOX 0
// #define PYRO_PLATFORM_SWITCH 0

// ----------------------
// Detect Platforms
// ----------------------

#include <cstdint> // For UINTPTR_MAX

#if defined(_WIN32) || defined(_WIN64)
#define PYRO_PLATFORM_FAMILY_WINDOWS 1
#define PYRO_PLATFORM_WINDOWS 1
#endif

#if defined(__linux__)
#define PYRO_PLATFORM_FAMILY_UNIX 1
#define PYRO_PLATFORM_LINUX 1
#endif

#if defined(__APPLE__)
#define PYRO_PLATFORM_FAMILY_UNIX 1
#define PYRO_PLATFORM_MACOS 1
#endif


// Default to 0
#ifndef PYRO_PLATFORM_32_BIT
#if defined(_WIN32) && !defined(_WIN64)
// Windows 32-bit
#define PYRO_PLATFORM_32_BIT 1
#elif defined(__i386__) || defined(__i386) || defined(__X86__) || (defined(__arm__) && !defined(__aarch64__))
// x86 32-bit or ARM 32-bit
#define PYRO_PLATFORM_32_BIT 1
// Generic check based on pointer size
#elif UINTPTR_MAX == UINT32_MAX
#define PYRO_PLATFORM_32_BIT 1
#elif UINTPTR_MAX == UINT64_MAX
#define PYRO_PLATFORM_64_BIT 1
#elif UINTPTR_MAX > UINT64_MAX
#define PYRO_PLATFORM_128_BIT 1
#else
#error Unsupported platform!
#endif
#endif