#pragma once

#include "../../input_context.hpp"

namespace ameko
{
class dummy_input_context : public input_context
{
public:
  dummy_input_context() = default;
  ~dummy_input_context() = default;

  auto set_key_callback(key_callback callback) -> void;
  auto set_mouse_button_callback(mouse_button_callback callback) -> void;
  auto set_cursor_pos_callback(cursor_pos_callback callback) -> void;

  auto key_match(key_code key, std::string_view key_name) -> bool;
  auto is_left_mouse_button(mouse_button_code button) -> bool;
  auto is_right_mouse_button(mouse_button_code button) -> bool;
  auto is_middle_mouse_button(mouse_button_code button) -> bool;

private:
  key_callback m_key_callback;
  mouse_button_callback m_mouse_button_callback;
  cursor_pos_callback m_cursor_pos_callback;
};
}  // namespace ameko
