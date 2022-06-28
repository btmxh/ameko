#pragma once

#include "../utils/enum.hpp"

namespace ameko
{
enum class display_backend
{
  dummy,
  glfw
};

AMEKO_GENERATE_ENUM_STRING_CONVERTER(display_backend,
                                     AMEKO_ENUM_NVP(display_backend, dummy),
                                     AMEKO_ENUM_NVP(display_backend, glfw));
}  // namespace ameko
