#pragma once
#include "utils_h/concurrency/one_shot_timer.h"
#include "utils_h/non_copyable_and_non_movable.h"
#include <functional>
#include <optional>

namespace utils_h { namespace concurrency {

class watchdog final : non_copyable_and_non_movable {
  public:
    watchdog(std::chrono::steady_clock::duration timeout, std::function<void()> on_triggered)
        : _on_triggered{std::move(on_triggered)}, _timeout{timeout}, _timer{std::in_place, _timeout, _on_triggered}
    {
    }

    void Reset()
    {
        _timer.emplace(_timeout, _on_triggered);
    }

  private:
    const std::function<void()> _on_triggered;
    const std::chrono::steady_clock::duration _timeout;
    std::optional<one_shot_timer> _timer;
};

}}
