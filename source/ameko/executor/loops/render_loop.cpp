#include "render_loop.hpp"

#include "../../graphics/graphics_context.hpp"

namespace ameko
{
render_loop::render_loop(std::shared_ptr<display> display,
                         std::function<void()> render_proc)
    : m_display(std::move(display))
    , m_render_proc(std::move(render_proc))
{
}

auto render_loop::run_single_loop(bool /*single*/) -> void
{
  m_render_proc();
  m_display->swap_buffers();
}

auto render_loop::set_context_current_on_thread(bool current) -> void
{
  m_display->get_graphics_context().make_current_on_thread(current);
}

auto render_loop::set_vsync(bool vsync) -> void
{
  m_display->set_vsync(vsync);
}

}  // namespace ameko
