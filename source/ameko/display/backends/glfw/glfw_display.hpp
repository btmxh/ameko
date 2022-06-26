#pragma once

#include <memory>

#include "../../../dependencies/glfw.hpp"
#include "../../display.hpp"

namespace ameko
{
auto create_glfw_display(display_config& display_config,
                         graphics_config& graphics_config)
    -> std::unique_ptr<display>;
}  // namespace ameko
