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

  constexpr offset2(T x, T y)
      : x(x)
      , y(y)
  {
  }

  AMEKO_TWO_WAY_SERIALIZE_FUNC(context, {
    context(AMEKO_SERIALIZATION_NVP(x));
    context(AMEKO_SERIALIZATION_NVP(y));
  })
};

template<typename T>
struct extent2
{
  T width;
  T height;

  constexpr extent2() = default;

  constexpr extent2(T width, T height)
      : width(width)
      , height(height)
  {
  }

  AMEKO_TWO_WAY_SERIALIZE_FUNC(context, {
    context(AMEKO_SERIALIZATION_NVP(width));
    context(AMEKO_SERIALIZATION_NVP(height));
  })

  [[nodiscard]] auto area() const -> decltype(auto) { return width * height; }
};

template<typename T>
struct rect
{
  offset2<T> offset;
  extent2<T> extent;

  constexpr rect() = default;

  constexpr rect(const offset2<T>& offset, const extent2<T>& extent)
      : offset(offset)
      , extent(extent)
  {
  }

  constexpr rect(T x, T y, const extent2<T>& extent)
      : offset(x, y)
      , extent(extent)
  {
  }

  constexpr rect(const offset2<T>& offset, T width, T height)
      : offset(offset)
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

  AMEKO_TWO_WAY_SERIALIZE_FUNC(context, {
    context(AMEKO_SERIALIZATION_NVP(offset));
    context(AMEKO_SERIALIZATION_NVP(extent));
  })
};

using offset2z = offset2<size_t>;
using extent2z = extent2<size_t>;
using rect4z = rect<size_t>;

}  // namespace ameko
// NOLINTEND(*-easily-swappable-parameters, *-identifier-length)
