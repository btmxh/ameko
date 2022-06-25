#pragma once

#include <cstdint>

#include "../utils/bitfield.hpp"
#include "../utils/rect.hpp"
#include "../utils/type_traits.hpp"

namespace ameko
{
using key_code = int32_t;
using key_scancode = int32_t;
using mouse_button_code = int32_t;
enum class modifier_key_flag
{
  control = 0x1,
  shift = 0x2,
  alt = 0x4,
  super = 0x8,
  num_lock = 0x10,
  caps_lock = 0x20
};

enum class key_action
{
  press,
  repeat,
  release
};

enum class mouse_button_action
{
  press,
  release
};

template<>
struct is_bitfield_enum<modifier_key_flag> : public true_type
{
};

using modifier_key_flags = bitfield<modifier_key_flag>;

struct key_event
{
  key_code key;
  key_scancode scancode;
  modifier_key_flags modifiers;
  key_action action;
};

struct mouse_button_event
{
  mouse_button_code button;
  modifier_key_flags modifiers;
  mouse_button_action action;
};

struct cursor_pos_event
{
  offset2<int32_t> old_pos;
  offset2<int32_t> new_pos;
};
}  // namespace ameko
