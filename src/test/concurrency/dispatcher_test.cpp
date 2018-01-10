#include "utils_h/concurrency/dispatcher.h"
#include <gtest/gtest.h>

using namespace utils_h;
using namespace testing;

class dispatcher_test : public Test {
  protected:
    void wait_while_all_tasks_are_executed()
    {
        _dispatcher.dispatch_and_wait([] {});
    }

    void unhandled_exception_handler()
    {
        _unhandled_exception_handler_was_called = true;
    }

    concurrency::dispatcher _dispatcher{[&](auto & /* exception */) { unhandled_exception_handler(); }};
    bool _unhandled_exception_handler_was_called{false};
};

TEST_F(dispatcher_test, Dispatch_and_wait_function_works_with_functions_which_return_void)
{
    _dispatcher.dispatch_and_wait([&] {});
}

TEST_F(dispatcher_test, Dispatch_and_wait_function_works_with_functions_which_return_value)
{
    ASSERT_EQ(_dispatcher.dispatch_and_wait([] { return std::string{"string"}; }), "string");
    ASSERT_EQ(_dispatcher.dispatch_and_wait([] { return 1; }), 1);
}

TEST_F(dispatcher_test, If_an_exception_occurs_during_executing_a_task_An_unhandled_exception_handler_is_called)
{
    _dispatcher.dispatch([] { throw std::runtime_error("error"); });
    wait_while_all_tasks_are_executed();

    ASSERT_TRUE(_unhandled_exception_handler_was_called);
}

TEST_F(dispatcher_test, Exception_during_dispatch_and_wait_function_is_propagated_to_the_caller)
{
    ASSERT_THROW(_dispatcher.dispatch_and_wait([] { throw std::runtime_error("error"); }), std::runtime_error);
}

TEST_F(dispatcher_test, is_dispatcher_thread_function_returns_true_if_it_is_called_inside_a_dispatcher_thread)
{
    ASSERT_FALSE(_dispatcher.is_dispatcher_thread());
    ASSERT_TRUE(_dispatcher.dispatch_and_wait([&] { return _dispatcher.is_dispatcher_thread(); }));
}

TEST_F(dispatcher_test, Tasks_are_executed_in_the_order_they_were_dispatched)
{
    std::vector<size_t> results;

    for (size_t i = 0; i < 100; i++) {
        _dispatcher.dispatch([i, &results] { results.push_back(i); });
    }

    wait_while_all_tasks_are_executed();
    ASSERT_EQ(results.size(), 100);

    for (size_t i = 0; i < 100; i++) {
        ASSERT_EQ(results[i], i);
    }
}
