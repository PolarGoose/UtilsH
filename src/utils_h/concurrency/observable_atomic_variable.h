#pragma once
#include "utils_h/concurrency/interruptible_condition_variable.h"
#include "utils_h/non_copyable_and_non_movable.h"
#include <functional>

namespace utils_h { namespace concurrency {

template <typename T> class observable_atomic_variable final : non_copyable_and_non_movable {
  public:
    observable_atomic_variable(
        T value, std::function<void(const T &)> on_value_changed_callback = [](const T & /* val */) {})
        : _on_value_changed_callback{std::move(on_value_changed_callback)}, _value{std::move(value)}
    {
    }

    auto get()
    {
        std::lock_guard lock{_mutex};
        return _value;
    }

    void set(T value)
    {
        std::lock_guard lock{_mutex};
        if (_value != value) {
            _value = std::move(value);
            _condVar.notify_all();
            _on_value_changed_callback(_value);
        }
    }

    void wait_for_value(const T &value, const std::optional<std::chrono::steady_clock::duration> timeout = {})
    {
        std::unique_lock lock{_mutex};

        _condVar.wait_for(
            lock, [&] { return _value == value; }, timeout);
    }

    void interrupt()
    {
        std::unique_lock lock{_mutex};
        _condVar.interrupt(lock);
    }

  private:
    mutable std::mutex _mutex;
    interruptible_condition_variable _condVar;
    std::function<void(T &)> _on_value_changed_callback;
    T _value;
};

}}
