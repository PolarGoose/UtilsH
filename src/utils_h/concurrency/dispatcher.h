#pragma once
#include "utils_h/concurrency/waiting_queue.h"
#include "utils_h/non_copyable_and_non_movable.h"
#include <functional>
#include <future>

namespace utils_h { namespace concurrency {

class dispatcher final : non_copyable_and_non_movable {
    using Task = std::function<void()>;

  public:
    explicit dispatcher(std::function<void(const std::exception_ptr &)> unhandled_exception_handler)
        : _unhandled_exception_handler{std::move(unhandled_exception_handler)}, _task_processing_thread{
                                                                                    &dispatcher::tasks_processing, this}
    {
    }

    [[nodiscard]] bool is_dispatcher_thread() const
    {
        return std::this_thread::get_id() == _task_processing_thread.get_id();
    }

    void dispatch(Task task)
    {
        _tasks_queue.blocking_push(std::move(task));
    }

    template <typename T> auto dispatch_and_wait(T task) -> decltype(task())
    {
        std::promise<decltype(task())> taskResult;
        auto taskResultFuture{taskResult.get_future()};

        _tasks_queue.blocking_push([task = std::move(task), &taskResult] {
            try {
                if constexpr (std::is_void_v<decltype(task())>) {
                    task();
                    taskResult.set_value();
                }
                else {
                    taskResult.set_value(task());
                }
            }
            catch (...) {
                taskResult.set_exception(std::current_exception());
            }
        });

        return taskResultFuture.get();
    }

    ~dispatcher()
    {
        _tasks_queue.interrupt();
        _task_processing_thread.join();
    }

  private:
    void tasks_processing()
    {
        try {
            while (true) {
                execute(_tasks_queue.blocking_pop());
            }
        }
        catch (const interrupted_exception &) {
            /* ignore */
        }
    }

    void execute(Task &&task) const
    {
        try {
            task();
        }
        catch (...) {
            _unhandled_exception_handler(std::current_exception());
        }
    }

    const std::function<void(const std::exception_ptr &)> _unhandled_exception_handler;
    waiting_queue<Task> _tasks_queue;
    std::thread _task_processing_thread;
};

}} // namespace UtilsH
