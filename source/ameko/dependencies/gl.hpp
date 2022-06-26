#pragma once

namespace ameko::deps
{
#ifdef AMEKO_HAS_OPENGL
constexpr bool has_opengl = true;
#else
constexpr bool has_opengl = false;
#endif

#ifdef AMEKO_HAS_OPENGL_ES
constexpr bool has_opengl_es = true;
#else
constexpr bool has_opengl_es = false;
#endif
}  // namespace ameko::deps
