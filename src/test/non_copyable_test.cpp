#include "utils_h/non_copyable.h"
#include "utils_h/type_traits.h"

using namespace utils_h;

class non_copyable_but_movable_class : non_copyable {
};
static_assert(!is_copyable<non_copyable_but_movable_class>::value);
static_assert(is_movable<non_copyable_but_movable_class>::value);
static_assert(std::is_default_constructible_v<non_copyable_but_movable_class>);
