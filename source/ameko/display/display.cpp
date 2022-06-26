#include <stdexcept>

#include "display.hpp"

#include "backends/dummy/dummy_display.hpp"
#include "backends/glfw/glfw_display.hpp"

namespace ameko
{
auto create_display([[maybe_unused]] display_config& display_config,
                    [[maybe_unused]] graphics_config& graphics_config)
    -> std::unique_ptr<display>
{
  if constexpr (deps::has_glfw) {
    return create_glfw_display(display_config, graphics_config);
  } else {
    return std::make_unique<dummy_display>(display_config, graphics_config);
  }
}
}  // namespace ameko
