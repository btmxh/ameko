#pragma once

#include <memory>

// TODO: move these out of the 'executor' directory
#include "../../../executor/sync/of_sync.hpp"
#include "../../../executor/threads/blocking_queue.hpp"
#include "../../../input/backends/dummy/dummy_input_context.hpp"
#include "../../display.hpp"

namespace ameko
{
class dummy_display : public display
{
public:
  dummy_display(display_config& display_config,
                graphics_config& graphics_config);
  ~dummy_display() override;

  dummy_display(const dummy_display&) = delete;
  dummy_display(dummy_display&&) = delete;
  auto operator=(const dummy_display&) = delete;
  auto operator=(dummy_display&&) = delete;

  auto set_visible([[maybe_unused]] bool visible) -> void;
  auto poll_events([[maybe_unused]] bool block) -> void;
  auto unblock_poll_events() -> void;
  auto swap_buffers() -> void;
  auto set_vsync([[maybe_unused]] bool vsync) -> void;
  auto close() -> void;

  auto set_on_close(std::function<void()> callback) -> void;

  auto get_input_context() -> input_context&;
  auto get_graphics_context() -> graphics_context&;

private:
  std::function<void()> m_on_close_callback;
  std::unique_ptr<graphics_context> m_graphics_context;
  dummy_input_context m_input_context;
  blocking_queue<int> m_event_queue;
  bool m_vsync_flag;
  std::unique_ptr<sync> m_vsync;
};
}  // namespace ameko
