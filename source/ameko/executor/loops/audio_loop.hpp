#pragma once

#include <functional>

#include "loop.hpp"

namespace ameko
{
class audio_loop : public executor_loop
{
public:
  explicit audio_loop(std::function<void()> update_proc);
  audio_loop(const audio_loop&) = delete;
  audio_loop(audio_loop&&) = delete;
  auto operator=(const audio_loop&) -> audio_loop& = delete;
  auto operator=(audio_loop&&) -> audio_loop& = delete;
  ~audio_loop() override = default;

  auto run_single_loop(bool single) -> void override;

private:
  std::function<void()> m_update_proc;
};
}  // namespace ameko
