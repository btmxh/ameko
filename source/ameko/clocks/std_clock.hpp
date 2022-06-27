#pragma once

#include <chrono>

#include "clock.hpp"

namespace ameko
{
template<typename StdClock>
class std_clock : public clock
{
public:
  std_clock()
      : m_start_time(get_raw_time())
  {
  }
  ~std_clock() override = default;

  std_clock(const std_clock&) = default;
  std_clock(std_clock&&) noexcept = default;
  auto operator=(const std_clock&) -> std_clock& = delete;
  auto operator=(std_clock&&) -> std_clock& = delete;

  static auto get_raw_time() -> double
  {
    auto time = StdClock::now();
    std::chrono::duration<double> duration = time.time_since_epoch();
    return duration.count();
  }

  auto get_time() -> double override { return get_raw_time() - m_start_time; }

private:
  double m_start_time;
};

using std_high_res_clock = std_clock<std::chrono::high_resolution_clock>;
using std_steady_clock = std_clock<std::chrono::steady_clock>;
using std_system_clock = std_clock<std::chrono::system_clock>;
using game_clock = std_steady_clock;
}  // namespace ameko
