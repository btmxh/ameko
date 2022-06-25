#pragma once

#include <array>
#include <stdexcept>
#include <tuple>

#include "type_traits.hpp"

namespace ameko
{
template<typename T>
struct enum_string_converter : public false_type
{
};

template<typename T>
using enum_nvp_t = std::pair<const char*, T>;

template<typename T, size_t NumValues>
using enum_map_t = std::array<enum_nvp_t<T>, NumValues>;

template<typename T>
constexpr bool is_string_convertible_enum = enum_string_converter<T>::value;

template<typename T, size_t NumValues>
constexpr auto enum_map_from_string(const enum_map_t<T, NumValues>& map,
                                    std::string_view string) -> T
{
  for (const auto& [name, value] : map) {
    if (name == string) {
      return value;
    }
  }

  throw std::runtime_error("enum value not found");
}

template<typename T, size_t NumValues>
constexpr auto enum_map_to_string(const enum_map_t<T, NumValues>& map, T value)
    -> std::string_view
{
  for (const auto& [name, other_value] : map) {
    if (value == other_value) {
      return name;
    }
  }

  throw std::runtime_error("invalid enum value");
}
}  // namespace ameko

// NOLINTBEGIN(*-macro-usage, *-macro-parentheses)
#define AMEKO_ENUM_NVP(T, value) std::pair<const char*, T>(#value, T::value)

#define AMEKO_GENERATE_ENUM_STRING_CONVERTER(T, ...) \
  template<> \
  struct ::ameko::enum_string_converter<T> : public true_type \
  { \
    static constexpr size_t num_enum_values = \
        std::tuple_size_v<decltype(std::make_tuple(__VA_ARGS__))>; \
    static constexpr enum_map_t<T, num_enum_values> enum_map = {__VA_ARGS__}; \
    static constexpr auto from_string(std::string_view string) -> T \
    { \
      return ::ameko::enum_map_from_string(enum_map, string); \
    } \
    static constexpr auto to_string(T enum_value) -> std::string_view \
    { \
      return ::ameko::enum_map_to_string(enum_map, enum_value); \
    } \
  }
// NOLINTEND(*-macro-usage, *-macro-parentheses)
