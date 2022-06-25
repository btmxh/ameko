#pragma once

#include "callbacks.hpp"

namespace ameko
{
class input_context
{
public:
  input_context() = default;
  virtual ~input_context() = default;

  virtual auto set_key_callback(key_callback callback) -> void = 0;
  virtual auto set_mouse_button_callback(mouse_button_callback callback)
      -> void = 0;
  virtual auto set_cursor_pos_callback(cursor_pos_callback callback)
      -> void = 0;

  virtual auto key_match(key_code key, std::string_view key_name) -> bool = 0;
  virtual auto is_left_mouse_button(mouse_button_code button) -> bool = 0;
  virtual auto is_right_mouse_button(mouse_button_code button) -> bool = 0;
  virtual auto is_middle_mouse_button(mouse_button_code button) -> bool = 0;
};
}  // namespace ameko
