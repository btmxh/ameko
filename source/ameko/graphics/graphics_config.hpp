#pragma once

#include "../config/serialization/serialization_context.hpp"
#include "../utils/optional.hpp"
#include "graphics_backend.hpp"

namespace ameko
{
struct graphics_config
{
  optional<graphics_backend> backend;

  template<typename SerializationContext>
  auto serialize(SerializationContext& context) -> void
  {
    context(AMEKO_SERIALIZATION_NVP(backend));
  }
};
}  // namespace ameko