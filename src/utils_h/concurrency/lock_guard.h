#pragma once
#include "utils_h/preprocessor.h"
#include <mutex>

#define UTILS_H_LOCK_GUARD(mutex) std::lock_guard<std::decay_t<decltype(mutex)>> UTILS_H_UNIQUE_NAME(mutex)
