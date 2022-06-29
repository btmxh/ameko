#pragma once

#include <filesystem>

#include "../display/display_config.hpp"
#include "../executor/executor_config.hpp"
#include "../graphics/graphics_config.hpp"
#include "../logging/logging_config.hpp"
#include "serialization/serialization_context.hpp"

namespace ameko
{
struct global_config
{
  std::filesystem::path config_file;

  display_config display;
  graphics_config graphics;
  logging_config logging;
  executor_config executor;

  template<typename SerializationContext>
  auto serialize(SerializationContext& context) -> void
  {
    context(AMEKO_SERIALIZATION_NVP(display));
    context(AMEKO_SERIALIZATION_NVP(graphics));
    context(AMEKO_SERIALIZATION_NVP(logging));
    context(AMEKO_SERIALIZATION_NVP(executor));
  }
};

auto load_global_config(const std::filesystem::path& config_file)
    -> global_config;
auto save_global_config(global_config& config) -> void;

}  // namespace ameko
