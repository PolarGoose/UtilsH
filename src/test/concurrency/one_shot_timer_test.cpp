#include "utils/time_passed_method.h"
#include "utils_h/concurrency/observable_atomic_variable.h"
#include "utils_h/concurrency/one_shot_timer.h"
#include <gtest/gtest.h>

using namespace utils_h;
using namespace concurrency;
using namespace std::chrono_literals;
using namespace testing;

class one_shot_timer_test : public Test, protected time_passed_method {
  protected:
    void wait_for_timer_callback_is_called()
    {
        _timer_callback_was_called.wait_for_value(true, _time * 2);
    }

    const std::chrono::milliseconds _time{300};
    observable_atomic_variable<bool> _timer_callback_was_called{false};
    std::unique_ptr<one_shot_timer> _timer{
        std::make_unique<one_shot_timer>(_time, [&] { _timer_callback_was_called.set(true); })};
};

TEST_F(one_shot_timer_test, Timer_is_started_when_it_is_constructed)
{
    ASSERT_NO_THROW(wait_for_timer_callback_is_called());
    ASSERT_GE(time_passed(), _time);
}

TEST_F(one_shot_timer_test, Timer_is_stopped_when_it_is_destructed)
{
    _timer.reset();
    ASSERT_THROW(wait_for_timer_callback_is_called(), timed_out_exception);
}
