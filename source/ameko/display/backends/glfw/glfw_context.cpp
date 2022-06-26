#include <stdexcept>

#include "glfw_context.hpp"

#include "../../../dependencies/glfw.hpp"

namespace ameko
{
class glfw_handle
{
public:
  explicit glfw_handle(const glfw_init_hints& hints)
  {
    glfwInitHint(GLFW_JOYSTICK_HAT_BUTTONS,
                 hints.joystick_hat_buttons ? GLFW_TRUE : GLFW_FALSE);
    glfwInitHint(GLFW_COCOA_CHDIR_RESOURCES,
                 hints.cocoa_chdir_resources ? GLFW_TRUE : GLFW_FALSE);
    glfwInitHint(GLFW_COCOA_MENUBAR,
                 hints.cocoa_menubar ? GLFW_TRUE : GLFW_FALSE);
    if (glfwInit() == GLFW_FALSE) {
      throw std::runtime_error("Unable to initialize GLFW");
    }
  }

  ~glfw_handle() { glfwTerminate(); }
  glfw_handle(const glfw_handle&) = delete;
  glfw_handle(glfw_handle&&) = delete;
  auto operator=(const glfw_handle&) = delete;
  auto operator=(glfw_handle&&) = delete;

  auto get_init_hints() -> const glfw_init_hints& { return m_hints; }

private:
  glfw_init_hints m_hints;
};

auto init_glfw(glfw_init_hints& hints) -> glfw
{
  static std::weak_ptr<glfw_handle> static_instance {};
  auto handle = static_instance.lock();
  if (handle != nullptr) {
    hints = handle->get_init_hints();
    return handle;
  }

  handle = std::make_shared<glfw_handle>(hints);
  static_instance = handle;
  return handle;
}
}  // namespace ameko
