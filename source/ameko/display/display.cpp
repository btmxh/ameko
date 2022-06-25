#include "display.hpp"

#include "backends/glfw/glfw_display.hpp"

namespace ameko
{
auto create_display(display_config& display_config,
                    graphics_config& graphics_config)
    -> std::unique_ptr<display>
{
  return std::make_unique<glfw_display>(display_config, graphics_config);
}
}  // namespace ameko
