#include "utils_h/struct_aliasing_disable.h"
#include <array>
#include <cstdint>

struct AliasedStruct {
    std::array<uint8_t, 3> a;
    uint32_t b;
};
static_assert(sizeof(AliasedStruct) > 7, "Compiler should add padding");

UTILS_H_DISABLE_STRUCT_ALIASING_BEGIN
struct PackedStruct {
    std::array<uint8_t, 3> a;
    uint32_t b;
};
UTILS_H_DISABLE_STRUCT_ALIASING_END
static_assert(sizeof(PackedStruct) == 7, "Compiler shouldn't add padding");
