#pragma once

#include <memory>

#include "../../display/display.hpp"
#include "loop.hpp"

namespace ameko
{
class event_loop : public executor_loop
{
public:
  explicit event_loop(std::shared_ptr<display> display);
  event_loop(const event_loop&) = delete;
  event_loop(event_loop&&) = delete;
  auto operator=(const event_loop&) -> event_loop& = delete;
  auto operator=(event_loop&&) -> event_loop& = delete;
  ~event_loop() override = default;

  auto run_single_loop(bool single) -> void override;
  auto cancel_loop() -> void;

private:
  std::shared_ptr<display> m_display;
};
}  // namespace ameko
