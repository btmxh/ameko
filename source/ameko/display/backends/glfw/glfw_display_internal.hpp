#pragma once

#include <functional>
#include <memory>

#include "../../../dependencies/glfw.hpp"
#include "../../../input/backends/glfw/glfw_input_context.hpp"
#include "../../../utils/rect.hpp"
#include "../../display.hpp"
#include "glfw_context.hpp"

namespace ameko
{
using glfw_window = std::unique_ptr<GLFWwindow, decltype(glfwDestroyWindow)*>;

struct glfw_window_user_pointer
{
  input_context* input_ctx = nullptr;
  graphics_context* graphics_ctx = nullptr;
  std::function<void()> on_close = nullptr;

  glfw_window_user_pointer(GLFWwindow* window);
};

class glfw_display : public display
{
public:
  glfw_display(display_config& display_config,
               graphics_config& graphics_config);
  ~glfw_display();

  glfw_display(const glfw_display&) = delete;
  glfw_display(glfw_display&&) = delete;
  auto operator=(const glfw_display&) = delete;
  auto operator=(glfw_display&&) = delete;

  auto set_visible(bool visible) -> void;
  auto poll_events(bool block) -> void;
  auto unblock_poll_events() -> void;
  auto swap_buffers() -> void;
  auto set_vsync(bool vsync) -> void;
  auto close() -> void;
  auto set_on_close(std::function<void()> callback) -> void;

  auto get_input_context() -> input_context&;
  auto get_graphics_context() -> graphics_context&;

private:
  glfw m_glfw;
  glfw_window m_window;
  glfw_window_user_pointer m_user_pointer;
  glfw_input_context m_input_context;
  std::unique_ptr<graphics_context> m_graphics_context;
  optional<bool> m_vsync;
};

auto get_glfw_window_user_pointer(GLFWwindow* window)
    -> glfw_window_user_pointer&;

}  // namespace ameko
