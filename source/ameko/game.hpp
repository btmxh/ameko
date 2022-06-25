#pragma once

#include <memory>

#include "config/global_config.hpp"
#include "executor/executor.hpp"
#include "logging/log_context.hpp"

namespace ameko
{

class game
{
public:
  game();
  auto run() -> void;

private:
  log_context m_log_context;
  global_config m_global_config;
  std::shared_ptr<executor> m_executor;
  std::shared_ptr<display> m_display;

  auto update() -> void;
  auto render() -> void;
  auto poll_events(bool blocking) -> void;
  auto send_empty_event() -> void;
  auto update_audio() -> void;
};
}  // namespace ameko
