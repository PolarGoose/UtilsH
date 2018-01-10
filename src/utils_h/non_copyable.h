#pragma once

namespace utils_h {

class non_copyable {
  protected:
    non_copyable() = default;
    ~non_copyable() = default;

    non_copyable(non_copyable &&) = default;
    non_copyable &operator=(non_copyable &&) = default;

  public:
    non_copyable(const non_copyable &) = delete;
    non_copyable &operator=(const non_copyable &) = delete;
};

}
