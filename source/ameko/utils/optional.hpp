#pragma once

#include "tl/optional.hpp"
#include "type_traits.hpp"

namespace ameko
{
using tl::make_optional;
using tl::nullopt;
using tl::nullopt_t;
using tl::optional;

template<typename T>
struct is_optional : public false_type
{
};

template<typename T>
struct is_optional<optional<T>> : public true_type
{
};

template<typename T>
inline constexpr bool is_optional_v = is_optional<T>::value;

template<typename T>
inline auto value_or_assign(optional<T>& opt, T&& value) -> T&
{
  if (!opt.has_value()) {
    opt = value;
  }

  return opt.value();
}

template<typename T, typename Func>
inline auto or_else_assign(optional<T>& opt, Func&& callback) -> T&
{
  if (!opt.has_value()) {
    opt = callback();
  }

  return opt.value();
}

template<typename F>
struct lazy_value_or
{
  F func;
  operator decltype(auto)() const { return func(); }
};

template<typename F>
lazy_value_or(F func) -> lazy_value_or<F>;

}  // namespace ameko