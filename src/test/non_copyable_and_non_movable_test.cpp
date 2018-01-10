#include "utils_h/non_copyable_and_non_movable.h"
#include "utils_h/type_traits.h"

using namespace utils_h;

class non_copyable_and_non_movable_class : non_copyable_and_non_movable {
};
static_assert(!is_copyable<non_copyable_and_non_movable_class>::value);
static_assert(!is_movable<non_copyable_and_non_movable_class>::value);
static_assert(std::is_default_constructible_v<non_copyable_and_non_movable_class>);
