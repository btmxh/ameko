#include <stdexcept>

#include "display.hpp"

#include "backends/dummy/dummy_display.hpp"
#include "backends/glfw/glfw_display.hpp"

namespace ameko
{
static auto get_default_display_backend() -> display_backend
{
  if constexpr (deps::has_glfw) {
    return display_backend::glfw;
  } else {
    return display_backend::dummy;
  }
}

auto create_display([[maybe_unused]] display_config& display_config,
                    [[maybe_unused]] graphics_config& graphics_config)
    -> std::unique_ptr<display>
{
  switch (
      value_or_assign(display_config.backend, get_default_display_backend())) {
    case display_backend::glfw:
      if constexpr (deps::has_glfw) {
        return create_glfw_display(display_config, graphics_config);
      } else {
        throw std::runtime_error("the GLFW display backend is unsupported");
      }
    case display_backend::dummy:
      return std::make_unique<dummy_display>(display_config, graphics_config);
    default:
      throw std::runtime_error("unknown display backend");
  }
}
}  // namespace ameko
