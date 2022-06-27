#pragma once

#include <algorithm>
#include <cmath>

#include "type_traits.hpp"

namespace ameko
{

template<typename FloatType,
         typename = enable_if_t<is_floating_point_v<FloatType>>>
inline auto float_equals(FloatType lhs, FloatType rhs) -> bool
{
  const double relative_tolerance = 1e-5;
  const double absolute_tolerance = 1e-5;
  return std::abs(lhs - rhs)
      <= std::max(absolute_tolerance,
                  relative_tolerance * std::max(std::abs(lhs), std::abs(rhs)));
}

template<typename FloatType,
         typename = enable_if_t<is_floating_point_v<FloatType>>>
inline auto is_integral_value(FloatType value) -> bool
{
  return float_equals(value, std::trunc(value));
}

}  // namespace ameko
