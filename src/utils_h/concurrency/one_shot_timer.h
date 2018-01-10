#pragma once

#include "utils_h/concurrency/observable_atomic_variable.h"
#include "utils_h/non_copyable_and_non_movable.h"
#include <functional>
#include <thread>

namespace utils_h { namespace concurrency {

class one_shot_timer final : non_copyable_and_non_movable {
  public:
    one_shot_timer(const std::chrono::steady_clock::duration time, std::function<void()> callback)
        : _time{time}, _callback{std::move(callback)}, _thread{&one_shot_timer::thread, this}
    {
    }

    ~one_shot_timer()
    {
        _stopRequested.set(true);
        _thread.join();
    }

  private:
    void thread()
    {
        wait_while_time_passes();
        if (!_stopRequested.get()) {
            _callback();
        }
    }

    void wait_while_time_passes()
    {
        try {
            _stopRequested.wait_for_value(true, _time);
        }
        catch (const timed_out_exception &) {
            /* ignore */
        }
    }

    const std::chrono::steady_clock::duration _time;
    const std::function<void()> _callback;
    observable_atomic_variable<bool> _stopRequested{false};
    std::thread _thread;
};

}}