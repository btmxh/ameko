#pragma once

#include <memory>

namespace ameko
{
struct glfw_init_hints
{
  bool joystick_hat_buttons = true;
  bool cocoa_chdir_resources = true;
  bool cocoa_menubar = true;
};

class glfw_handle;
using glfw = std::shared_ptr<glfw_handle>;

auto init_glfw(glfw_init_hints& hints) -> glfw;

inline auto init_glfw() -> glfw
{
  glfw_init_hints hints {};
  return init_glfw(hints);
}
}  // namespace ameko
