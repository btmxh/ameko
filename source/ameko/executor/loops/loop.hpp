#pragma once

namespace ameko
{
class executor_loop
{
public:
  executor_loop() = default;
  executor_loop(const executor_loop&) = default;
  executor_loop(executor_loop&&) = default;
  auto operator=(const executor_loop&) -> executor_loop& = default;
  auto operator=(executor_loop&&) -> executor_loop& = default;
  virtual ~executor_loop() = default;

  virtual auto run_single_loop(bool single) -> void = 0;
};
}  // namespace ameko
