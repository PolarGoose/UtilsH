#pragma once
#include "utils_h/type_traits.h"
#include <cassert>
#include <type_traits>

#define UTILS_H_ASSERT_MSG(expression, message)                                                                        \
    static_assert(utils_h::is_string_literal<decltype(message)>::value, "'message' should be a string literal");       \
    assert((expression) && (message))
