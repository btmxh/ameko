#pragma once

#include <memory>

#include "../../clocks/clock.hpp"
#include "../../clocks/std_clock.hpp"
#include "sync.hpp"

namespace ameko
{
class of_sync : public sync
{
public:
  of_sync();
  explicit of_sync(std::shared_ptr<clock> clock);

  ~of_sync() override = default;

  of_sync(const of_sync&) = default;
  of_sync(of_sync&&) = default;
  auto operator=(const of_sync&) -> of_sync& = default;
  auto operator=(of_sync&&) -> of_sync& = default;

  auto sync_impl(double frequency) -> void override;

private:
  std::shared_ptr<clock> m_clock;
  double m_current_time;
  double m_last_frame_time;
  double m_sleep_error;
};
}  // namespace ameko
