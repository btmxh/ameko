#include "glfw_input_context.hpp"

#include "../../../display/backends/glfw/glfw_display.hpp"

namespace ameko
{

static auto glfw_mods_to_ameko_flags(int s_mods) -> modifier_key_flags
{
  auto mods = static_cast<unsigned>(s_mods);
  modifier_key_flags flags {};
  if ((mods & static_cast<unsigned>(GLFW_MOD_CONTROL)) != 0U) {
    flags |= modifier_key_flag::control;
  }
  if ((mods & static_cast<unsigned>(GLFW_MOD_SHIFT)) != 0U) {
    flags |= modifier_key_flag::shift;
  }
  if ((mods & static_cast<unsigned>(GLFW_MOD_ALT)) != 0U) {
    flags |= modifier_key_flag::alt;
  }
  if ((mods & static_cast<unsigned>(GLFW_MOD_SUPER)) != 0U) {
    flags |= modifier_key_flag::super;
  }
  if ((mods & static_cast<unsigned>(GLFW_MOD_NUM_LOCK)) != 0U) {
    flags |= modifier_key_flag::num_lock;
  }
  if ((mods & static_cast<unsigned>(GLFW_MOD_CAPS_LOCK)) != 0U) {
    flags |= modifier_key_flag::caps_lock;
  }
  return flags;
}

glfw_input_context::glfw_input_context(GLFWwindow* window)
    : m_window(window)
{
  get_glfw_window_user_pointer(window).input_ctx = this;
  glfwSetKeyCallback(
      window,
      [](auto wnd, auto key, auto scancode, auto action, auto mods)
      {
        auto& user_ptr = get_glfw_window_user_pointer(wnd);
        key_event event {static_cast<int32_t>(key),
                         static_cast<int32_t>(scancode),
                         glfw_mods_to_ameko_flags(mods),
                         [action]
                         {
                           switch (action) {
                             case GLFW_REPEAT:
                               return key_action::repeat;
                             case GLFW_RELEASE:
                               return key_action::release;
                             default:
                               return key_action::press;
                           }
                         }()};
        const auto& callback =
            dynamic_cast<glfw_input_context*>(user_ptr.input_ctx)
                ->m_key_callback;
        if (callback != nullptr) {
          callback(event);
        }
      });

  glfwSetMouseButtonCallback(
      window,
      [](auto wnd, auto button, auto action, auto mods)
      {
        auto& user_ptr = get_glfw_window_user_pointer(wnd);
        mouse_button_event event {button,
                                  glfw_mods_to_ameko_flags(mods),
                                  action == GLFW_RELEASE
                                      ? mouse_button_action::release
                                      : mouse_button_action::press};
        const auto& callback =
            dynamic_cast<glfw_input_context*>(user_ptr.input_ctx)
                ->m_mouse_button_callback;
        if (callback != nullptr) {
          callback(event);
        }
      });
  glfwSetCursorPosCallback(
      window,
      [](auto wnd, auto cursor_x, auto cursor_y)
      {
        auto& user_ptr = get_glfw_window_user_pointer(wnd);
        auto& ctx = *dynamic_cast<glfw_input_context*>(user_ptr.input_ctx);
        cursor_pos_event event {};
        event.old_pos = ctx.m_old_cursor_pos;
        ctx.m_old_cursor_pos = event.new_pos = {static_cast<int32_t>(cursor_x),
                                                static_cast<int32_t>(cursor_y)};
        if (ctx.m_cursor_pos_callback != nullptr) {
          ctx.m_cursor_pos_callback(event);
        }
      });
}

glfw_input_context::~glfw_input_context()
{
  (void)glfwSetKeyCallback(m_window, nullptr);
  (void)glfwSetMouseButtonCallback(m_window, nullptr);
  (void)glfwSetCursorPosCallback(m_window, nullptr);
}

auto glfw_input_context::set_key_callback(key_callback callback) -> void
{
  m_key_callback = callback;
}

auto glfw_input_context::set_mouse_button_callback(
    mouse_button_callback callback) -> void
{
  m_mouse_button_callback = callback;
}

auto glfw_input_context::set_cursor_pos_callback(cursor_pos_callback callback)
    -> void
{
  m_cursor_pos_callback = callback;
}

auto glfw_input_context::key_match(key_code key, std::string_view key_name)
    -> bool
{
  if (key_name.length() == 1) {
    auto key_char = key_name[0];
    if ('a' <= key_char && key_char <= 'z') {
      return key == key_char - 'a' + GLFW_KEY_A;
    }
    if ('A' <= key_char && key_char <= 'Z') {
      return key == key_char - 'A' + GLFW_KEY_A;
    }
    if ('0' <= key_char && key_char <= '9') {
      return key == key_char - '0' + GLFW_KEY_0;
    }
  }

  if ((key == GLFW_KEY_LEFT_CONTROL || key == GLFW_KEY_RIGHT_CONTROL)
      && key_name == "ctrl")
  {
    return true;
  }

  if ((key == GLFW_KEY_LEFT_ALT || key == GLFW_KEY_RIGHT_ALT)
      && key_name == "alt")
  {
    return true;
  }

  if ((key == GLFW_KEY_LEFT_SUPER || key == GLFW_KEY_RIGHT_SUPER)
      && key_name == "super")
  {
    return true;  // NOLINT(*-simplify-boolean-expr)
  }

  return false;
}

auto glfw_input_context::is_left_mouse_button(mouse_button_code button) -> bool
{
  return button == GLFW_MOUSE_BUTTON_LEFT;
}

auto glfw_input_context::is_right_mouse_button(mouse_button_code button) -> bool
{
  return button == GLFW_MOUSE_BUTTON_RIGHT;
}

auto glfw_input_context::is_middle_mouse_button(mouse_button_code button)
    -> bool
{
  return button == GLFW_MOUSE_BUTTON_MIDDLE;
}
}  // namespace ameko
