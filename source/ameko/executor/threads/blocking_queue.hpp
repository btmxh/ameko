#pragma once

#include <condition_variable>
#include <mutex>
#include <queue>

namespace ameko
{
template<typename T>
class blocking_queue
{
public:
  blocking_queue() = default;

  auto push(const T& elem) -> void
  {
    {
      std::unique_lock lock(m_mutex);
      m_queue.push_front(elem);
    }
    m_condvar.notify_one();
  }

  auto pop() -> T
  {
    std::unique_lock lock(m_mutex);
    m_condvar.wait(lock, [this] { return !this->m_queue.empty(); });
    T ret = std::move(m_queue.back());
    m_queue.pop_back();
    return ret;
  }

  auto empty() -> bool
  {
    std::unique_lock lock(m_mutex);
    return m_queue.empty();
  }

private:
  std::deque<T> m_queue;
  std::mutex m_mutex;
  std::condition_variable m_condvar;
};
}  // namespace ameko
