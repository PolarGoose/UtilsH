#include "utils_h/logging/assert_with_message.h"
#include <gtest/gtest.h>

TEST(assert_macro, compiles)
{
    UTILS_H_ASSERT_MSG(true, "some message");
    UTILS_H_ASSERT_MSG(2 == 2, "some message");
}
