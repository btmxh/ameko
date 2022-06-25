#pragma once

#include "../utils/rect.hpp"

namespace ameko
{
class graphics_context
{
public:
  graphics_context() = default;
  virtual ~graphics_context() = default;

  graphics_context(const graphics_context&) = delete;
  graphics_context(graphics_context&&) = delete;
  auto operator=(const graphics_context&) = delete;
  auto operator=(graphics_context&&) = delete;

  virtual auto make_current_on_thread(bool current) -> void = 0;
};
}  // namespace ameko
