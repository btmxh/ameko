#pragma once

#include <map>
#include <string_view>

#include "../config/serialization/serialization_context.hpp"
#include "../utils/enum.hpp"
#include "../utils/optional.hpp"
#include "../utils/rect.hpp"
#include "display_backend.hpp"

namespace ameko
{
enum class display_mode
{
  windowed,
  fullscreen,
  borderless_fullscreen
};

AMEKO_GENERATE_ENUM_STRING_CONVERTER(display_mode,
                                     AMEKO_ENUM_NVP(display_mode, windowed),
                                     AMEKO_ENUM_NVP(display_mode, fullscreen),
                                     AMEKO_ENUM_NVP(display_mode,
                                                    borderless_fullscreen));

struct display_config
{
  optional<display_backend> backend;
  optional<extent2z> size;
  optional<display_mode> mode;

  AMEKO_TWO_WAY_SERIALIZE_FUNC(context, {
    context(AMEKO_SERIALIZATION_NVP(backend));
    context(AMEKO_SERIALIZATION_NVP(size));
    context(AMEKO_SERIALIZATION_NVP(mode));
  })
};
}  // namespace ameko
