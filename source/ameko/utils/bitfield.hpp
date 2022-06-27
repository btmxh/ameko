#pragma once

#include "type_traits.hpp"

namespace ameko
{
template<typename Enum>
struct is_bitfield_enum : public false_type
{
};

template<typename Enum>
inline constexpr bool is_bitfield_enum_v = is_bitfield_enum<Enum>::value;

template<typename Enum, typename = std::enable_if_t<is_bitfield_enum_v<Enum>>>
struct bitfield
{
  using base_type = std::make_unsigned_t<std::underlying_type_t<Enum>>;
  base_type value {};

  constexpr explicit bitfield() = default;

  constexpr explicit bitfield(base_type a_value)
      : value(a_value)
  {
  }

  constexpr explicit bitfield(Enum a_value)
      : value(static_cast<base_type>(a_value))
  {
  }

  constexpr auto operator|(Enum rhs) const -> decltype(auto)
  {
    return *this | bitfield<Enum>(rhs);
  }

  constexpr auto operator&(Enum rhs) const -> decltype(auto)
  {
    return *this & bitfield<Enum>(rhs);
  }

  constexpr auto operator|(bitfield rhs) const -> bitfield
  {
    return bitfield(value | rhs.value);
  }

  constexpr auto operator&(bitfield rhs) const -> bool
  {
    return (value & rhs.value) != 0;
  }

  constexpr friend auto operator|(Enum lhs, bitfield rhs) -> decltype(auto)
  {
    return rhs | lhs;
  }

  constexpr friend auto operator&(Enum lhs, bitfield rhs) -> decltype(auto)
  {
    return rhs & lhs;
  }

  constexpr auto operator|=(bitfield rhs) -> bitfield&
  {
    value |= rhs.value;
    return *this;
  }

  constexpr auto operator|=(Enum rhs) -> bitfield&
  {
    return (*this |= bitfield<Enum>(rhs));
  }
};

template<typename Enum, typename = std::enable_if_t<is_bitfield_enum_v<Enum>>>
inline auto operator|(Enum lhs, Enum rhs) -> decltype(auto)
{
  return bitfield<Enum>(lhs) | rhs;
}

template<typename Enum, typename = std::enable_if_t<is_bitfield_enum_v<Enum>>>
inline auto operator&(Enum lhs, Enum rhs) -> decltype(auto)
{
  return bitfield<Enum>(lhs) & rhs;
}
}  // namespace ameko
