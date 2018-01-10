#pragma once
#include "utils_h/non_copyable_and_non_movable.h"
#include <chrono>
#include <condition_variable>
#include <functional>
#include <mutex>

namespace utils_h { namespace concurrency {

struct timed_out_exception final : public std::runtime_error {
    explicit timed_out_exception(char const *const msg) : std::runtime_error(msg)
    {
    }
};

struct interrupted_exception final : public std::runtime_error {
    explicit interrupted_exception(char const *const msg) : std::runtime_error(msg)
    {
    }
};

class interruptible_condition_variable : non_copyable_and_non_movable {
  public:
    void wait_for(std::unique_lock<std::mutex> &lock, const std::function<bool()> &predicate,
                  const std::optional<std::chrono::steady_clock::duration> timeout = {})
    {
        if (timeout) {
            m_cond_var.wait_for(lock, *timeout, [&] { return predicate() || _is_interrupt_requested; });
        }
        else {
            m_cond_var.wait(lock, [&] { return predicate() || _is_interrupt_requested; });
        }

        if (_is_interrupt_requested) {
            throw interrupted_exception("interruptible_condition_variable: interrupted");
        }
        if (!predicate()) {
            throw timed_out_exception("interruptible_condition_variable: timed out");
        }
    }

    void notify_one()
    {
        m_cond_var.notify_one();
    }
    void notify_all()
    {
        m_cond_var.notify_all();
    }

    void interrupt(std::unique_lock<std::mutex> & /* lock */)
    {
        _is_interrupt_requested = true;
        m_cond_var.notify_all();
    }

  private:
    std::condition_variable m_cond_var;
    bool _is_interrupt_requested{false};
};

}}
