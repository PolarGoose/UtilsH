#pragma once
#include <string>
#include <string_view>

namespace utils_h { namespace priv {

template <typename To, typename From> To Transform(From str)
{
    return {std::begin(str), std::end(str)};
}

}}

namespace utils_h {

inline std::wstring to_wstring(std::string_view str)
{
    return priv::Transform<std::wstring>(str);
}

inline std::string to_string(std::wstring_view wideStr)
{
    return priv::Transform<std::string>(wideStr);
}

}