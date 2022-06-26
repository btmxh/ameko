#pragma once

#ifdef AMEKO_HAS_GLFW
#  include <GLFW/glfw3.h>
#endif

namespace ameko::deps
{
#ifdef AMEKO_HAS_GLFW
constexpr bool has_glfw = true;
#else
constexpr bool has_glfw = false;
#endif
}  // namespace ameko::deps
