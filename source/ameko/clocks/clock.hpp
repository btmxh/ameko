#pragma once

namespace ameko
{
class clock {
public:
  clock() = default;
  virtual ~clock() = default;

  clock(const clock&) = default;
  clock(clock&&) = default;
  auto operator=(const clock&) -> clock& = default;
  auto operator=(clock&&) -> clock& = default;

  virtual auto get_time() -> double = 0;
};
} // namespace ameko
