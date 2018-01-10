#pragma once
#include "non_copyable.h"

namespace utils_h {

class non_copyable_and_non_movable : non_copyable {
  protected:
    non_copyable_and_non_movable() = default;
    ~non_copyable_and_non_movable() = default;

  public:
    non_copyable_and_non_movable(non_copyable_and_non_movable &&) = delete;
    non_copyable_and_non_movable &operator=(non_copyable_and_non_movable &&) = delete;
};

}
