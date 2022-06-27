#pragma once

#include "../../graphics_context.hpp"

namespace ameko
{
class dummy_graphics_context : public graphics_context
{
public:
  dummy_graphics_context() = default;
  ~dummy_graphics_context() = default;

  dummy_graphics_context(const dummy_graphics_context&) = delete;
  dummy_graphics_context(dummy_graphics_context&&) = delete;
  auto operator=(const dummy_graphics_context&) = delete;
  auto operator=(dummy_graphics_context&&) = delete;

  auto make_current_on_thread([[maybe_unused]] bool current) -> void override;
};
}  // namespace ameko
