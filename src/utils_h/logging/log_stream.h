#pragma once
#include "utils_h/non_copyable_and_non_movable.h"
#include <functional>
#include <sstream>

namespace utils_h { namespace logging {

class log_stream final : non_copyable_and_non_movable {
    using log_function = std::function<void(const std::string &)>;

  public:
    log_stream(log_function log) : _log_function{std::move(log)}
    {
    }

    std::ostringstream &get_stream()
    {
        return _stream;
    }

    ~log_stream()
    {
        _log_function(_stream.str());
    }

  private:
    std::ostringstream _stream;
    log_function _log_function;
};

}}
