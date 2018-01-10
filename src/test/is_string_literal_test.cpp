#include "utils_h/type_traits.h"
#include <gtest/gtest.h>
#include <string>

using namespace utils_h;

TEST(is_string_literal, Non_string_literals)
{
    const char *msg1 = "some string";            // NOLINT
    const char msg2[] = "some string";           // NOLINT
    const char msg3[]{'s', 'o', 'm', 'e', '\0'}; // NOLINT
    auto msg4 = "some string";                   // NOLINT
    const std::string msg5 = "some string";

    static_assert(!is_string_literal<decltype(msg1)>::value);
    static_assert(!is_string_literal<decltype(msg2)>::value);
    static_assert(!is_string_literal<decltype(msg3)>::value);
    static_assert(!is_string_literal<decltype(msg4)>::value);
    static_assert(!is_string_literal<decltype(msg5)>::value);
}

TEST(is_string_literal, String_literals)
{
    const auto &msg1 = "some string";

    static_assert(is_string_literal<decltype(msg1)>::value);
    static_assert(is_string_literal<decltype("some string")>::value);
}
