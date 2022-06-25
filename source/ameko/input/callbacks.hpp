#pragma once

#include <functional>

#include "events.hpp"

namespace ameko
{
template<typename EventType>
using event_callback = std::function<void(const EventType&)>;

using key_callback = event_callback<key_event>;
using mouse_button_callback = event_callback<mouse_button_event>;
using cursor_pos_callback = event_callback<cursor_pos_event>;
}  // namespace ameko
