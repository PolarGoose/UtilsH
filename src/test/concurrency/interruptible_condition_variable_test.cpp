#include "utils/time_passed_method.h"
#include "utils_h/concurrency/interruptible_condition_variable.h"
#include <chrono>
#include <gtest/gtest.h>
#include <thread>

using namespace utils_h;
using namespace std::chrono_literals;
using namespace testing;

class interruptible_condition_variable_test : public Test, protected time_passed_method {
  protected:
    std::mutex _mutex;
    concurrency::interruptible_condition_variable _cond_var;
};

TEST_F(interruptible_condition_variable_test, Wait_method_throws_an_exception_on_time_out)
{
    std::unique_lock lock{_mutex};

    ASSERT_THROW(_cond_var.wait_for(
                     lock, [] { return false; }, 10ms),
                 concurrency::timed_out_exception);
}

TEST_F(interruptible_condition_variable_test, Wait_method_throws_an_exception_when_interrupted)
{
    auto client_func{[&] {
        try {
            std::unique_lock lock{_mutex};
            _cond_var.wait_for(
                lock, [] { return false; }, 1s);
        }
        catch (const concurrency::interrupted_exception &) {
            return true;
        }
        return false;
    }};

    auto client1_is_interrupted{false};
    auto client2_is_interrupted{false};

    std::thread client1{[&] { client1_is_interrupted = client_func(); }};
    std::thread client2{[&] { client2_is_interrupted = client_func(); }};

    std::this_thread::sleep_for(100ms);
    {
        std::unique_lock lock{_mutex};
        _cond_var.interrupt(lock);
    }
    client1.join();
    client2.join();

    ASSERT_TRUE(client1_is_interrupted);
    ASSERT_TRUE(client2_is_interrupted);
}

TEST_F(interruptible_condition_variable_test, After_interrupted_All_calls_to_Wait_throw_exceptions)
{
    std::unique_lock lock{_mutex};
    _cond_var.interrupt(lock);

    ASSERT_THROW(_cond_var.wait_for(lock, [] { return false; }), concurrency::interrupted_exception);
}

TEST_F(interruptible_condition_variable_test, Wait_method_unblocks_when_condition_becomes_true)
{
    auto value_to_wait_for{false};
    const auto time_to_wait_before_unblock_thread{300ms};

    std::thread client{[&] {
        std::unique_lock lock{_mutex};
        _cond_var.wait_for(
            lock, [&] { return value_to_wait_for; }, 1s);
    }};

    std::this_thread::sleep_for(time_to_wait_before_unblock_thread);
    {
        std::unique_lock lock{_mutex};
        value_to_wait_for = true;
    }
    client.join();
}
