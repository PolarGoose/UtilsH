#pragma once
#include <type_traits>

namespace utils_h {

template <typename T>
struct is_copyable : std::bool_constant<std::is_copy_constructible_v<T> && std::is_copy_assignable_v<T>> {
};

template <typename T>
struct is_movable : std::bool_constant<std::is_move_constructible_v<T> && std::is_move_assignable_v<T>> {
};

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wpedantic"
template <typename T>
struct is_string_literal
    : std::is_same<T, std::add_lvalue_reference_t<const char[std::extent_v<std::remove_reference_t<T>>]>> // NOLINT
{
};
#pragma GCC diagnostic pop

}
