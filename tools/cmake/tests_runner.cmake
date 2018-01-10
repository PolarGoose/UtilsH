include(GoogleTest)
enable_testing()
add_custom_target(run_tests COMMAND
    ${CMAKE_CTEST_COMMAND} -C $<CONFIGURATION> --parallel 8 --no-compress-output --output-on-failure)

function(add_to_tests_runner target_name)
    gtest_discover_tests(${target_name})
    add_dependencies(run_tests ${target_name})
endfunction()
