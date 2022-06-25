#pragma once

namespace ameko
{
class sync
{
public:
  sync() = default;
  sync(const sync&) = default;
  sync(sync&&) = default;
  auto operator=(const sync&) -> sync& = default;
  auto operator=(sync&&) -> sync& = default;
  virtual ~sync() = default;

  virtual auto sync_impl(double frequency) -> void = 0;

  auto sync_to(double frequency) -> void
  {
    if (frequency > 0) {
      sync_impl(frequency);
    }
  }
};
}  // namespace ameko
