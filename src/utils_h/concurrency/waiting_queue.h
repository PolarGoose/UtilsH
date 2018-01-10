#pragma once
#include "utils_h/concurrency/interruptible_condition_variable.h"
#include "utils_h/non_copyable_and_non_movable.h"
#include <queue>

namespace utils_h { namespace concurrency {

template <typename T> class waiting_queue final : private non_copyable_and_non_movable {
  public:
    explicit waiting_queue(const size_t max_size = std::numeric_limits<size_t>::max()) : _max_size{max_size}
    {
    }

    auto size() const
    {
        std::lock_guard lock{_mutex};
        return _queue.size();
    }

    auto empty() const
    {
        std::lock_guard lock{_mutex};
        return _queue.empty();
    }

    auto max_size() const
    {
        return _max_size;
    }

    void blocking_push(T val, const std::optional<std::chrono::steady_clock::duration> timeout = {})
    {
        std::unique_lock lock{_mutex};

        _cond_var.wait_for(
            lock, [&] { return _queue.size() < _max_size; }, timeout);

        _queue.emplace(std::move(val));
        _cond_var.notify_one();
    }

    T blocking_pop(const std::optional<std::chrono::steady_clock::duration> timeout = {})
    {
        std::unique_lock lock{_mutex};

        _cond_var.wait_for(
            lock, [&] { return !_queue.empty(); }, timeout);

        T value{std::move(_queue.front())};
        _queue.pop();
        _cond_var.notify_one();
        return value;
    }

    void interrupt()
    {
        std::unique_lock lock{_mutex};
        _cond_var.interrupt(lock);
    }

  private:
    const size_t _max_size;
    mutable std::mutex _mutex;
    interruptible_condition_variable _cond_var;
    std::queue<T> _queue;
};

}}
