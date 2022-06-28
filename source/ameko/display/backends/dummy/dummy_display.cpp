#include "dummy_display.hpp"

#include "../../../graphics/backends/dummy/dummy_graphics_context.hpp"
#include "../../../graphics/graphics_config.hpp"

namespace ameko
{
static const double dummy_refresh_rate = 60.0;

dummy_display::dummy_display([[maybe_unused]] display_config& display_config,
                             graphics_config& graphics_config)
    : m_graphics_context(std::make_unique<dummy_graphics_context>())
    , m_vsync(std::make_unique<of_sync>())
{
  auto current_graphics_backend =
      graphics_config.backend.value_or(graphics_backend::dummy);
  if (current_graphics_backend != graphics_backend::dummy) {
    // TODO: add vulkan headless support
    throw std::runtime_error("unsupported graphics backend");
  }
}

dummy_display::~dummy_display() = default;

auto dummy_display::set_visible([[maybe_unused]] bool visible) -> void {}
auto dummy_display::poll_events(bool block) -> void
{
  if (block) {
    (void)m_event_queue.pop();
  }
}
auto dummy_display::unblock_poll_events() -> void
{
  if (m_event_queue.empty()) {
    m_event_queue.push(0);
  }
}

auto dummy_display::swap_buffers() -> void
{
  if (m_vsync_flag) {
    m_vsync->sync_to(dummy_refresh_rate);
  }
}

auto dummy_display::set_vsync([[maybe_unused]] bool vsync) -> void
{
  m_vsync_flag = vsync;
}

auto dummy_display::close() -> void
{
  m_on_close_callback();
}

auto dummy_display::set_on_close(std::function<void()> callback) -> void
{
  m_on_close_callback = std::move(callback);
}

auto dummy_display::get_input_context() -> input_context&
{
  return m_input_context;
}

auto dummy_display::get_graphics_context() -> graphics_context&
{
  return *m_graphics_context;
}
}  // namespace ameko
