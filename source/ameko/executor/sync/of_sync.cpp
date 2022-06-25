#include <thread>

#include "of_sync.hpp"

namespace ameko
{
of_sync::of_sync()
    : of_sync(std::make_shared<game_clock>())
{
}

of_sync::of_sync(std::shared_ptr<clock> clock)
    : m_clock(std::move(clock))
    , m_current_time(m_clock->get_time())
    , m_last_frame_time(m_current_time)
    , m_sleep_error(0.0)
{
}

auto of_sync::sync_impl(double frequency) -> void
{
  const double min_lag = -1.0 / 30.0;
  m_last_frame_time = m_current_time;
  m_current_time = m_clock->get_time();

  auto excess_time = 1.0 / frequency - (m_current_time - m_last_frame_time);

  auto before = m_current_time;
  auto sleep_time = std::max(0.0, excess_time + m_sleep_error);
  std::this_thread::sleep_for(std::chrono::duration<double>(sleep_time));
  auto time_slept = (m_current_time = m_clock->get_time()) - before;

  m_sleep_error += excess_time - time_slept;
  m_sleep_error = std::max(min_lag, m_sleep_error);
}
}  // namespace ameko
