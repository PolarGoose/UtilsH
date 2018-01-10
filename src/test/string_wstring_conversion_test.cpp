#include "utils_h/string_wstring_conversion.h"
#include <gtest/gtest.h>

using namespace utils_h;

TEST(string_wstring_conversion, std_string_to_std_wstring_conversion_works)
{
    ASSERT_EQ(to_wstring(std::string("Test string")), L"Test string");
}

TEST(string_wstring_conversion, std_wstring_to_std_string_conversion_works)
{
    ASSERT_EQ(to_string(std::wstring(L"Test string")), "Test string");
}

TEST(string_wstring_conversion, const_char_ptr_to_std_wstring_conversion_works)
{
    ASSERT_EQ(to_wstring("Test string"), L"Test string");
}

TEST(string_wstring_conversion, const_wchar_t_ptr_to_std_string_conversion_works)
{
    ASSERT_EQ(to_string(L"Test string"), "Test string");
}
