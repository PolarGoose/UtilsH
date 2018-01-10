cmake_minimum_required (VERSION 3.17)
project(UtilsHLibrary LANGUAGES CXX)

include(tools/cmake/compiler_options.cmake)
include(tools/cmake/clang_format.cmake)
include(tools/cmake/clang_tidy.cmake)
include(tools/cmake/conan.cmake)
include(tools/cmake/tests_runner.cmake)

conan_cmake_run(
    REQUIRES
        boost/1.72.0
        gtest/1.10.0
    BASIC_SETUP
        CMAKE_TARGETS
)

add_subdirectory(src/test)
