find_program(CLANG_TIDY_COMMAND NAMES clang-tidy clang-tidy-10)
if(NOT CLANG_TIDY_COMMAND)
    message(FATAL_ERROR "clang-tidy command is not found")
endif()

option(enable_clang_tidy "Run clang-tidy during the build" OFF)
message(STATUS "enable_clang_tidy=${enable_clang_tidy}")

function(add_clang_tidy target_name)
    if(NOT ${enable_clang_tidy})
        return()
    endif()

    set_property(
        TARGET ${target_name}
        PROPERTY CXX_CLANG_TIDY "${CLANG_TIDY_COMMAND}" "--quiet" "--extra-arg=-Wno-unknown-warning-option")
endfunction()
