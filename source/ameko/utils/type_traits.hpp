#pragma once

#include <type_traits>

namespace ameko
{
using std::remove_cv_t;
using std::remove_reference_t;

template<typename T>
using remove_cvref_t = remove_cv_t<remove_reference_t<T>>;

using std::is_arithmetic_v;
using std::is_floating_point_v;
using std::is_integral_v;
using std::is_same_v;

using std::enable_if_t;

using std::false_type;
using std::true_type;
}  // namespace ameko
