message(STATUS "Compiler: ${CMAKE_CXX_COMPILER} version ${CMAKE_CXX_COMPILER_VERSION}")

add_library(CompilerOptions INTERFACE)

# https://cmake.org/cmake/help/latest/prop_gbl/CMAKE_CXX_KNOWN_FEATURES.html#prop_gbl:CMAKE_CXX_KNOWN_FEATURES
target_compile_features(CompilerOptions INTERFACE cxx_std_17)

target_compile_options(CompilerOptions INTERFACE
    "$<$<CONFIG:DEBUG>:-O0;>")
target_compile_options(CompilerOptions INTERFACE
    -Wall
    -Wextra
    -Werror
    -Wshadow
    -Wnon-virtual-dtor
    -Wold-style-cast
    -Wunused
    -Woverloaded-virtual
    -Wconversion
    -Wnull-dereference
    -Wformat=2
    -Wmisleading-indentation
    -Wduplicated-cond
    -Wduplicated-branches
    -Wlogical-op
    -Wcast-align
    -Wsign-compare
    -Wunused-function
    -Wuseless-cast
    -Wdouble-promotion
    -Wpedantic
)

function(add_compiler_options targetName)
    target_link_libraries(${targetName} PRIVATE CompilerOptions)
endfunction()
