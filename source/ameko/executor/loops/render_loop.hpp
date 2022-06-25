#pragma once

#include <functional>
#include <memory>

#include "../../display/display.hpp"
#include "loop.hpp"

namespace ameko
{
class render_loop : public executor_loop
{
public:
  render_loop(std::shared_ptr<display> display,
              std::function<void()> render_proc);

  render_loop(const render_loop&) = delete;
  render_loop(render_loop&&) = delete;
  auto operator=(const render_loop&) -> render_loop& = delete;
  auto operator=(render_loop&&) -> render_loop& = delete;
  ~render_loop() override = default;

  auto run_single_loop(bool single) -> void override;
  auto set_context_current_on_thread(bool current) -> void;

  auto set_vsync(bool vsync) -> void;

private:
  std::shared_ptr<display> m_display;
  std::function<void()> m_render_proc;
};
}  // namespace ameko
