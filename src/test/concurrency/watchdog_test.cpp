#include "utils_h/concurrency/observable_atomic_variable.h"
#include "utils_h/concurrency/watchdog.h"
#include <gtest/gtest.h>

using namespace utils_h;
using namespace concurrency;
using namespace std::chrono_literals;
using namespace testing;

class watchdog_test : public Test {
  protected:
    watchdog_test() : _watchdog{std::in_place, _watchdog_timeout, [&] { _is_watchdog_timed_out.set(true); }}
    {
    }

    void reset_watchdog_for_some_time()
    {
        for (auto i = 0; i < 5; i++) {
            std::this_thread::sleep_for(_watchdog_timeout / 2);
            _watchdog->Reset();
        }
    }

    void wait_and_check_that_watchdog_triggered()
    {
        return _is_watchdog_timed_out.wait_for_value(true, _watchdog_timeout * 2);
    }

    void wait_and_check_that_watchdog_did_not_trigger()
    {
        ASSERT_THROW(wait_and_check_that_watchdog_triggered(), timed_out_exception);
    }

    const std::chrono::milliseconds _watchdog_timeout{300ms};
    observable_atomic_variable<bool> _is_watchdog_timed_out{false};
    std::optional<watchdog> _watchdog;
};

TEST_F(watchdog_test, Watchdog_starts_when_it_is_constructed_and_timeouts_If_not_resetted)
{
    wait_and_check_that_watchdog_triggered();
}

TEST_F(watchdog_test, Watchdog_can_be_reseted_and_fires_timed_out_event_when_stopped_being_reseted)
{
    reset_watchdog_for_some_time();

    ASSERT_FALSE(_is_watchdog_timed_out.get());
    wait_and_check_that_watchdog_triggered();
}

TEST_F(watchdog_test, Watchdog_is_stopped_on_destruction)
{
    _watchdog.reset();
    wait_and_check_that_watchdog_did_not_trigger();
}
