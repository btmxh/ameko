#pragma once

#include <array>
#include <atomic>
#include <cstdint>

#include "../utils/optional.hpp"
#include "executor_mode.hpp"
#include "loops/audio_loop.hpp"
#include "loops/event_loop.hpp"
#include "loops/loop.hpp"
#include "loops/render_loop.hpp"
#include "loops/update_loop.hpp"
#include "sync/sync.hpp"
#include "threads/thread.hpp"

namespace ameko
{
struct executor_loops
{
  std::unique_ptr<event_loop> event;
  std::unique_ptr<update_loop> update;
  std::unique_ptr<render_loop> render;
  std::unique_ptr<audio_loop> audio;
};

class executor
{
public:
  executor();

  auto set_mode(const executor_mode& mode) -> void;
  auto start(executor_loops loops) -> void;
  auto pause() -> void;
  auto resume() -> void;
  auto stop() -> void;

  auto main_thread_run() -> void;
  auto join() -> void;

  auto run_in_thread(executor_thread_id tid, const executor_thread::work& func)
      -> bool;
  auto run_in_thread_and_block(executor_thread_id tid,
                               const executor_thread::work& func) -> bool;

  auto get_thread_optional(executor_thread_id tid)
      -> optional<executor_thread>*;

private:
  std::array<optional<executor_thread>, max_num_executor_threads - 1>
      m_threads;  // main thread is omitted
  optional<executor_mode> m_mode;
  std::atomic_bool m_running {false};
  executor_loops m_loops;

  double m_relative_event_timer = 0.0;
  double m_relative_update_timer = 0.0;
  double m_relative_render_timer = 0.0;
  double m_relative_audio_timer = 0.0;

  std::unique_ptr<sync> m_main_thread_sync;

  auto lazy_init_thread(executor_thread_id tid) -> void;

  friend class executor_thread;
  auto run_single_loop(executor_thread_id tid, sync& sync) -> void;
};
}  // namespace ameko
