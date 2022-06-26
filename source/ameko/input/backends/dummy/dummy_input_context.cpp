#include "dummy_input_context.hpp"

namespace ameko
{
auto dummy_input_context::set_key_callback(key_callback callback) -> void
{
  m_key_callback = std::move(callback);
}

auto dummy_input_context::set_mouse_button_callback(
    mouse_button_callback callback) -> void
{
  m_mouse_button_callback = std::move(callback);
}

auto dummy_input_context::set_cursor_pos_callback(cursor_pos_callback callback)
    -> void
{
  m_cursor_pos_callback = std::move(callback);
}

auto dummy_input_context::key_match([[maybe_unused]] key_code key,
                                    [[maybe_unused]] std::string_view key_name)
    -> bool
{
  return false;
}

auto dummy_input_context::is_left_mouse_button(
    [[maybe_unused]] mouse_button_code button) -> bool
{
  return false;
}

auto dummy_input_context::is_right_mouse_button(
    [[maybe_unused]] mouse_button_code button) -> bool
{
  return false;
}

auto dummy_input_context::is_middle_mouse_button(
    [[maybe_unused]] mouse_button_code button) -> bool
{
  return false;
}
}  // namespace ameko