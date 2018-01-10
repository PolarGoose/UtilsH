find_program(CLANG_FORMAT_COMMAND NAMES clang-format clang-format-10)
if(NOT CLANG_FORMAT_COMMAND)
    message(FATAL_ERROR "clang-format command is not found")
endif()

add_custom_target(format_code_using_clang_format ALL
    VERBATIM COMMAND
        /bin/sh -c "${CLANG_FORMAT_COMMAND} -style=file -i $(find '${PROJECT_SOURCE_DIR}/src' -name '*.c' -o -name '*.cpp' -o -name '*.h' -o -name '*.hpp')"
    WORKING_DIRECTORY
        ${PROJECT_SOURCE_DIR}
    COMMENT
        "Formating C++ files with clang-format")

function(add_clang_format target_name)
    add_dependencies(${target_name} format_code_using_clang_format)
endfunction()
