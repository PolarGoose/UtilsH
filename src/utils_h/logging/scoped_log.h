#pragma once
#include "utils_h/non_copyable_and_non_movable.h"
#include <chrono>
#include <functional>
#include <sstream>

namespace utils_h { namespace logging {

class scoped_log final : non_copyable_and_non_movable {
    using log_function = std::function<void(const std::string &)>;

  public:
    scoped_log(const char *const line_info, log_function log) : _line_info{line_info}, _log_function{std::move(log)}
    {
        _log_function(_line_info + "Enter");
    }

    ~scoped_log()
    {
        std::stringstream stream;
        stream << _line_info << (is_destructor_called_because_of_exception() ? "Exception" : "Exit") << "("
               << time_passed().count() << "ms)";
        _log_function(stream.str());
    }

  private:
    [[nodiscard]] bool is_destructor_called_because_of_exception() const
    {
        return std::uncaught_exceptions() > _uncaught_exceptions_when_created;
    }

    [[nodiscard]] std::chrono::milliseconds time_passed() const
    {
        return std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - _creation_time);
    }

    const std::chrono::steady_clock::time_point _creation_time{std::chrono::steady_clock::now()};
    const int _uncaught_exceptions_when_created{std::uncaught_exceptions()};
    const std::string _line_info;
    log_function _log_function;
};

}}
