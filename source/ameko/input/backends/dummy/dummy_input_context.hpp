#pragma once

#include "../../input_context.hpp"

namespace ameko
{
class dummy_input_context : public input_context
{
public:
  dummy_input_context() = default;
  ~dummy_input_context() override = default;

  dummy_input_context(const dummy_input_context&) = delete;
  dummy_input_context(dummy_input_context&&) = delete;
  auto operator=(const dummy_input_context&) = delete;
  auto operator=(dummy_input_context&&) = delete;

  auto set_key_callback(key_callback callback) -> void override;
  auto set_mouse_button_callback(mouse_button_callback callback)
      -> void override;
  auto set_cursor_pos_callback(cursor_pos_callback callback) -> void override;
  auto key_match(key_code key, std::string_view key_name) -> bool override;

  auto is_left_mouse_button(mouse_button_code button) -> bool override;
  auto is_right_mouse_button(mouse_button_code button) -> bool override;
  auto is_middle_mouse_button(mouse_button_code button) -> bool override;

private:
  key_callback m_key_callback;
  mouse_button_callback m_mouse_button_callback;
  cursor_pos_callback m_cursor_pos_callback;
};
}  // namespace ameko
