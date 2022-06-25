#include "audio_loop.hpp"

namespace ameko
{
audio_loop::audio_loop(std::function<void()> update_proc)
    : m_update_proc(std::move(update_proc))
{
}

auto audio_loop::run_single_loop(bool /*single*/) -> void
{
  m_update_proc();
}
}  // namespace ameko
