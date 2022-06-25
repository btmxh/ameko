#include "event_loop.hpp"

namespace ameko
{
event_loop::event_loop(std::shared_ptr<display> display)
    : m_display(std::move(display))
{
}

auto event_loop::run_single_loop(bool single) -> void
{
  m_display->poll_events(single);
}

auto event_loop::cancel_loop() -> void
{
  m_display->unblock_poll_events();
}
}  // namespace ameko
