#pragma once

#include <functional>

#include "loop.hpp"

namespace ameko
{
class update_loop : public executor_loop
{
public:
  explicit update_loop(std::function<void()> update_proc);
  update_loop(const update_loop&) = delete;
  update_loop(update_loop&&) = delete;
  auto operator=(const update_loop&) -> update_loop& = delete;
  auto operator=(update_loop&&) -> update_loop& = delete;
  ~update_loop() override = default;

  auto run_single_loop(bool single) -> void override;

private:
  std::function<void()> m_update_proc;
};
}  // namespace ameko
