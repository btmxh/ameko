#include "update_loop.hpp"

namespace ameko
{
update_loop::update_loop(std::function<void()> update_proc)
    : m_update_proc(std::move(update_proc))
{
}

auto update_loop::run_single_loop(bool /*single*/) -> void
{
  m_update_proc();
}
}  // namespace ameko
