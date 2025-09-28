cmake_minimum_required(VERSION 3.14)
set(SH_SRC "${CMAKE_SOURCE_DIR}/test/TestsEditor")
file(GLOB_RECURSE ENDF6_SRC
      "${SH_SRC}/*.hpp"
      "${SH_SRC}/*.cpp")
      
add_executable("TestsEditor" ${ENDF6_SRC})

foreach(_source IN ITEMS ${ENDF6_SRC})
    get_filename_component(_source_path "${_source}" PATH)
    string(REPLACE "${SH_SRC}" "" _group_path "${_source_path}")
    string(REPLACE "/" "\\" _group_path "${_group_path}")
    source_group("${_group_path}" FILES "${_source}")
endforeach()

set_target_properties("TestsEditor" PROPERTIES
  POSITION_INDEPENDENT_CODE False
  INTERPROCEDURAL_OPTIMIZATION False
)

set_target_properties(TestsEditor PROPERTIES MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>DLL")

gtest_discover_tests(TestsEditor)

target_link_libraries(TestsEditor
  SWEditor
  GTest::gmock
  GTest::gtest_main
  GTest::gmock_main
  )

setup_executable(TestsEditor TRUE TRUE)
