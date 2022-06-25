#pragma once

#include "../utils/optional.hpp"
#include "config_value.hpp"

namespace ameko
{
template<typename T, bool IsInheritable>
using inheritable = config_value<T, IsInheritable>;

inline constexpr nullopt_t inherit = nullopt;
}  // namespace ameko
