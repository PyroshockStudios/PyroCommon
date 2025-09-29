# eabase
add_subdirectory(EABase)

# eathread
add_subdirectory(EAThread)

# eaassert
add_subdirectory(EAAssert)

# eastdc
add_subdirectory(EAStdC)

# eastl
add_subdirectory(EASTL)

# snmalloc
set(SNMALLOC_BUILD_TESTING OFF)
add_subdirectory(snmalloc)

# libassert
add_subdirectory(libassert)

# fmt format
add_subdirectory(fmt)
