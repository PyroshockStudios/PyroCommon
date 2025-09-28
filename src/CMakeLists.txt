set(SH_SRC ${CMAKE_CURRENT_SOURCE_DIR}/PyroCommon)
file(GLOB_RECURSE ENDF6_SRC
      "${SH_SRC}/*.hpp"
      "${SH_SRC}/*.cpp")

add_library(PyroCommon SHARED ${ENDF6_SRC})

foreach(_source IN ITEMS ${ENDF6_SRC})
    get_filename_component(_source_path "${_source}" PATH)
    string(REPLACE "${SH_SRC}" "" _group_path "${_source_path}")
    string(REPLACE "/" "\\" _group_path "${_group_path}")
    source_group("${_group_path}" FILES "${_source}")
endforeach()

target_link_libraries(PyroCommon PUBLIC EAStdC EASTL libassert::assert)

target_include_directories(PyroCommon
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/..>
        $<INSTALL_INTERFACE:include>
)

install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
    DESTINATION include/PyroCommon
    FILES_MATCHING PATTERN "*.hpp"
)

target_compile_features(PyroCommon PRIVATE cxx_std_23)

# windows cope
set_target_properties(PyroCommon PROPERTIES WINDOWS_EXPORT_ALL_SYMBOLS YES)
set_target_properties(PyroCommon PROPERTIES MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>DLL")

set(SOURCE_FOLDER ${CMAKE_SOURCE_DIR}/PyroCommon)

file(GLOB_RECURSE LIBRARY_SOURCES
  "${SOURCE_FOLDER}/*.hpp"
  "${SOURCE_FOLDER}/*.cpp"
)

foreach(src_file IN LISTS LIBRARY_SOURCES)
    get_filename_component(dir_path "${src_file}" PATH)
    file(RELATIVE_PATH group_path "${SOURCE_FOLDER}" "${dir_path}")
    string(REPLACE "/" "\\" group_path "${group_path}")
    source_group("${group_path}" FILES "${src_file}")
endforeach()