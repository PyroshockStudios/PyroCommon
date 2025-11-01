cmake_minimum_required(VERSION 3.28)
project(PyroCommon)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")

foreach(OUTPUTCONFIG Debug Release RelWithDebInfo MinSizeRel)
    set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_${OUTPUTCONFIG} "${CMAKE_BINARY_DIR}/lib")
    set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_${OUTPUTCONFIG} "${CMAKE_BINARY_DIR}/bin")
    set(CMAKE_EXECUTABLE_OUTPUT_DIRECTORY_${OUTPUTCONFIG} "${CMAKE_BINARY_DIR}/bin")
endforeach()

if (MSVC)
add_compile_options(
 /Zc:preprocessor   # libassert preprocessor issues
 /MP                # multithreading 
)

add_compile_options()
endif()

include(Config.cmake)

# ==== Project Config ====

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

if (PYRO_COMMON_BUILD_TESTS)
enable_testing()
endif()

add_subdirectory(vendor)

add_subdirectory(src)

if (PYRO_COMMON_BUILD_TESTS)
add_subdirectory(tests)
endif()