#pragma once

#include "../utils/enum.hpp"

namespace ameko
{
enum class graphics_backend
{
  dummy,
  opengl,
  opengl_es,
  vulkan
};

AMEKO_GENERATE_ENUM_STRING_CONVERTER(graphics_backend,
                                     AMEKO_ENUM_NVP(graphics_backend, dummy),
                                     AMEKO_ENUM_NVP(graphics_backend, opengl),
                                     AMEKO_ENUM_NVP(graphics_backend,
                                                    opengl_es),
                                     AMEKO_ENUM_NVP(graphics_backend, vulkan));
}  // namespace ameko
