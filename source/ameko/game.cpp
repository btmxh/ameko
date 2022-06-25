#include <csignal>
#include <iostream>

#include "game.hpp"

namespace ameko
{
static auto set_multi_thread_mode(executor& executor) -> void
{
  // NOLINTBEGIN(*-magic-numbers)
  executor_mode mode {};
  mode.vsync = vsync_mode::automatic;
  mode.event = {main_thread_id, 1.0};
  mode.update = {1, 1.0};
  mode.audio = {1, 0.1};
  mode.render = {2, 1.0};
  mode.executor_thread_frequencies = {0.0, 1000.0, 0.0, 0.0};

  executor.set_mode(mode);
  // NOLINTEND(*-magic-numbers)
}

game::game()
    : m_global_config(
        [&]
        {
          auto global_config =
              load_global_config(R"(E:\dev\ameko\build\config.toml)");
          setup_loggers_from_config(global_config.logging);
          return global_config;
        }())
    , m_executor(std::make_shared<executor>())
    , m_display(
          create_display(m_global_config.display, m_global_config.graphics))
{
}

static auto set_terminate_handlers(const std::shared_ptr<executor>& exec_ptr)
{
  // NOLINTNEXTLINE(*-avoid-non-const-global-variables)
  static std::weak_ptr<executor> weak_executor = exec_ptr;
  auto signal_func = [](auto)
  {
    if (auto shared_executor = weak_executor.lock(); shared_executor != nullptr)
    {
      shared_executor->stop();
    }
  };

  (void)std::signal(SIGTERM, signal_func);
  (void)std::signal(SIGINT, signal_func);
}

auto game::run() -> void
{
  m_global_config.executor.default_vsync_mode = vsync_mode::automatic;

  set_terminate_handlers(m_executor);

  executor_loops loops;
  loops.event = std::make_unique<event_loop>(m_display);
  loops.update = std::make_unique<update_loop>([] {});
  loops.render = std::make_unique<render_loop>(m_display, [] {});
  loops.audio = std::make_unique<audio_loop>([] {});

  m_executor->start(std::move(loops));
  set_multi_thread_mode(*m_executor);
  m_display->set_on_close([&] { m_executor->stop(); });
  m_display->set_visible(/*visible=*/true);
  m_executor->main_thread_run();
  m_executor->join();

  save_global_config(m_global_config);
}
}  // namespace ameko
