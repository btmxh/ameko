#pragma once

#include <atomic>
#include <functional>
#include <memory>
#include <thread>

#include "../sync/sync.hpp"
#include "blocking_queue.hpp"

namespace ameko
{
class executor;
inline constexpr size_t max_num_executor_threads = 4;
using executor_thread_id = uint_fast16_t;
inline constexpr executor_thread_id main_thread_id = 0;

class executor_thread
{
public:
  using work = std::function<void()>;

  executor_thread(executor& owner, executor_thread_id tid);

  auto start() -> void;
  // call executor_loop::run_single_loop for all managed loops and do works
  auto switch_to_loop_mode() -> void;
  // only do works
  auto switch_to_idle_mode() -> void;
  auto stop() -> void;

  auto join() -> void;

  auto run_in_thread(const work& func) -> void;
  auto run_in_thread_and_block(const work& func) -> void;

private:
  std::thread m_thread;
  executor* m_owner;
  executor_thread_id m_id;
  std::unique_ptr<sync> m_sync;
  std::atomic_bool m_stopped;
  std::atomic_bool m_is_idle_mode;
  blocking_queue<work> m_works;
};
}  // namespace ameko
