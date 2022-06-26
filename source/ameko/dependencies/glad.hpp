#pragma once

#ifdef AMEKO_HAS_GLAD
#  include <glad/glad.h>
#endif

#include "gl.hpp"

namespace ameko::deps
{
#ifdef AMEKO_HAS_GLAD
constexpr bool has_glad = true;
#else
constexpr bool has_glad = false;
#endif
}  // namespace ameko::deps
