add_executable(utils_h_test
	bit_cast_test.cpp
	circular_buffer_test.cpp
	circular_buffer_with_sum_of_all_elements_test
	struct_aliasing_disable_test.cpp
	string_wstring_conversion_test.cpp
	non_copyable_test.cpp
	non_copyable_and_non_movable_test.cpp
	is_string_literal_test.cpp
    boost_utils/serial_port_test.cpp
	logging/assert_with_message_test.cpp
	logging/log_stream_test.cpp
	logging/scoped_log_test.cpp
	concurrency/one_shot_timer_test.cpp
	concurrency/waiting_queue_test.cpp
	concurrency/watchdog_test.cpp
	concurrency/dispatcher_test.cpp
	concurrency/interruptible_condition_variable_test.cpp
	concurrency/observable_atomic_variable_test.cpp
	concurrency/lock_guard_test.cpp
)

target_link_libraries(utils_h_test PRIVATE CONAN_PKG::gtest CONAN_PKG::boost)
target_include_directories(utils_h_test PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/..)
add_to_tests_runner(utils_h_test)
add_compiler_options(utils_h_test)
add_clang_format(utils_h_test)
add_clang_tidy(utils_h_test)
