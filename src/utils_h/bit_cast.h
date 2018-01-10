#pragma once
#include <memory.h>
#include <type_traits>

namespace utils_h {

template <typename Dest, typename Source> static Dest bit_cast(const Source &source)
{
    static_assert(sizeof(Dest) == sizeof(Source), "Source and Dest sizes should be the same");
    static_assert(std::is_trivially_copyable_v<Source>, "Source should be trivially copyable");
    static_assert(std::is_trivially_copyable_v<Dest>, "Dest should be trivially copyable");

    Dest dest;
    memcpy(&dest, &source, sizeof(dest));
    return dest;
}

} // namespace UtilsH
