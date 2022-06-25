#include "thread.hpp"

#include "../executor.hpp"
#include "../sync/of_sync.hpp"

namespace ameko
{
executor_thread::executor_thread(executor& owner, executor_thread_id tid)
    : m_owner(&owner)
    , m_id(tid)
    , m_sync(std::make_unique<of_sync>())
{
}

auto executor_thread::start() -> void
{
  m_thread = std::thread(
      [this]
      {
        while (!m_stopped) {
          if (!m_works.empty()) {
            m_works.pop()();
          }

          if (!m_is_idle_mode) {
            m_owner->run_single_loop(m_id, *m_sync);
          }
        }
      });
}

auto executor_thread::switch_to_idle_mode() -> void
{
  m_is_idle_mode = true;
  run_in_thread_and_block([] {});
}

auto executor_thread::switch_to_loop_mode() -> void
{
  m_is_idle_mode = false;
  run_in_thread([] {});
}

auto executor_thread::stop() -> void
{
  m_stopped = true;
  run_in_thread([] {});
}

auto executor_thread::join() -> void
{
  if (m_thread.joinable()) {
    m_thread.join();
  }
}

auto executor_thread::run_in_thread(const work& func) -> void
{
  m_works.push(func);
}

auto executor_thread::run_in_thread_and_block(const work& func) -> void
{
  work func_copy = func;
  struct wait_objs
  {
    std::mutex mtx {};
    std::condition_variable condvar {};
    std::atomic_bool done = false;
  };

  auto shared_wait_objs = std::make_shared<wait_objs>();
  m_works.push(
      [func_copy, shared_wait_objs]
      {
        func_copy();
        {
          std::scoped_lock lock(shared_wait_objs->mtx);
          shared_wait_objs->done = true;
        }
        shared_wait_objs->condvar.notify_one();
      });

  std::unique_lock lock(shared_wait_objs->mtx);
  shared_wait_objs->condvar.wait(
      lock, [shared_wait_objs]() -> bool { return shared_wait_objs->done; });
}

}  // namespace ameko
