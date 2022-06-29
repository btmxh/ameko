#pragma once

#include "../config/serialization/serialization_context.hpp"

// NOLINTBEGIN(*-easily-swappable-parameters, *-identifier-length)
namespace ameko
{
template<typename T>
struct offset2
{
  T x {};
  T y {};

  constexpr offset2() = default;

  constexpr offset2(T a_x, T a_y)
      : x(a_x)
      , y(a_y)
  {
  }

  template<typename SerializationContext>
  auto serialize(SerializationContext& context) -> void
  {
    context(AMEKO_SERIALIZATION_NVP(x), /*required=*/true);
    context(AMEKO_SERIALIZATION_NVP(y), /*required=*/true);
  }
};

template<typename T>
struct extent2
{
  T width;
  T height;

  constexpr extent2() = default;

  constexpr extent2(T a_width, T a_height)
      : width(a_width)
      , height(a_height)
  {
  }

  template<typename SerializationContext>
  auto serialize(SerializationContext& context) -> void
  {
    context(AMEKO_SERIALIZATION_NVP(width), /*required=*/true);
    context(AMEKO_SERIALIZATION_NVP(height), /*required=*/true);
  }

  [[nodiscard]] auto area() const -> decltype(auto) { return width * height; }
};

template<typename T>
struct rect
{
  offset2<T> offset;
  extent2<T> extent;

  constexpr rect() = default;

  constexpr rect(const offset2<T>& a_offset, const extent2<T>& a_extent)
      : offset(a_offset)
      , extent(a_extent)
  {
  }

  constexpr rect(T x, T y, const extent2<T>& a_extent)
      : offset(x, y)
      , extent(a_extent)
  {
  }

  constexpr rect(const offset2<T>& a_offset, T width, T height)
      : offset(a_offset)
      , extent(width, height)
  {
  }

  constexpr rect(T x, T y, T width, T height)
      : offset(x, y)
      , extent(width, height)
  {
  }

  auto min_x() const -> T { return offset.x; }
  auto max_x() const -> T { return offset.x + extent.width; }
  auto center_x() const -> T { return offset.x + extent.width / 2; }

  auto min_y() const -> T { return offset.y; }
  auto max_y() const -> T { return offset.y + extent.height; }
  auto center_y() const -> T { return offset.y + extent.height / 2; }

  template<typename SerializationContext>
  auto serialize(SerializationContext& context) -> void
  {
    context(AMEKO_SERIALIZATION_NVP(offset), /*required=*/true);
    context(AMEKO_SERIALIZATION_NVP(extent), /*required=*/true);
  }
};

using offset2z = offset2<size_t>;
using extent2z = extent2<size_t>;
using rect4z = rect<size_t>;

}  // namespace ameko
// NOLINTEND(*-easily-swappable-parameters, *-identifier-length)
