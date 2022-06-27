#pragma once

#include <memory>

#include "display_config.hpp"

namespace ameko
{
class input_context;
class graphics_context;
struct graphics_config;

class display
{
public:
  display() = default;
  virtual ~display() = default;

  display(const display&) = default;
  display(display&&) = default;
  auto operator=(const display&) -> display& = default;
  auto operator=(display&&) -> display& = default;

  virtual auto set_visible(bool visible) -> void = 0;
  virtual auto poll_events(bool block) -> void = 0;
  virtual auto unblock_poll_events() -> void = 0;
  virtual auto swap_buffers() -> void = 0;
  virtual auto set_vsync(bool vsync) -> void = 0;
  virtual auto close() -> void = 0;
  virtual auto set_on_close(std::function<void()> callback) -> void = 0;

  virtual auto get_input_context() -> input_context& = 0;
  virtual auto get_graphics_context() -> graphics_context& = 0;
};

auto create_display(display_config& display_config,
                    graphics_config& graphics_config)
    -> std::unique_ptr<display>;

}  // namespace ameko
