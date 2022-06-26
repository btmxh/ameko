#include <stdexcept>

#include "gl_context.hpp"

#include "../../../dependencies/glad.hpp"

namespace ameko
{
class gl_context : public graphics_context
{
public:
  gl_context(bool gl_es,
             gl_load_proc load_proc,
             gl_make_context_current_proc make_context_current_proc)
      : m_make_context_current_proc(std::move(make_context_current_proc))
  {
    if (gl_es) {
      if (gladLoadGLES2Loader(load_proc) == 0) {
        throw std::runtime_error("unable to initialize OpenGL ES");
      }
    } else {
      if (gladLoadGLLoader(load_proc) == 0) {
        throw std::runtime_error("unable to initialize OpenGL");
      }
    }
  }

  ~gl_context() override = default;

  gl_context(const gl_context&) = delete;
  gl_context(gl_context&&) = delete;
  auto operator=(const gl_context&) = delete;
  auto operator=(gl_context&&) = delete;

  auto make_current_on_thread(bool current) -> void override
  {
    m_make_context_current_proc(current);
  }

private:
  gl_make_context_current_proc m_make_context_current_proc;
};

auto create_opengl_graphics_context(
    bool gl_es,
    gl_load_proc load_proc,
    gl_make_context_current_proc make_context_current_proc)
    -> std::unique_ptr<graphics_context>
{
  return std::make_unique<gl_context>(
      gl_es, load_proc, std::move(make_context_current_proc));
}
}  // namespace ameko
