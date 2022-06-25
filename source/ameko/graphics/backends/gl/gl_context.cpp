#include <stdexcept>

#include "gl_context.hpp"

#include <glad/glad.h>

namespace ameko
{
gl_context::gl_context(gl_load_proc load_proc,
                       gl_make_context_current_proc make_context_current_proc)
    : m_load_proc(load_proc)
    , m_make_context_current_proc(std::move(make_context_current_proc))
{
  if (gladLoadGLLoader(load_proc) == 0) {
    throw std::runtime_error("unable to initialize OpenGL");
  }
}

auto gl_context::make_current_on_thread(bool current) -> void
{
  m_make_context_current_proc(current);
}

auto gl_context::load_gl_proc(const char* name) -> void*
{
  return m_load_proc(name);
}
}  // namespace ameko
