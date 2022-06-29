#pragma once

#include <optional>
#include <vector>

#include "../config/serialization/serialization_context.hpp"
#include "../utils/optional.hpp"

namespace ameko
{
struct logger_config
{
  optional<std::string> log_path;
  optional<bool> log_rotate;
  optional<std::string> log_pattern;

  template<typename SerializationContext>
  auto base_serialize(SerializationContext& context) -> void
  {
    context(AMEKO_SERIALIZATION_NVP(log_path));
    context(AMEKO_SERIALIZATION_NVP(log_rotate));
    context(AMEKO_SERIALIZATION_NVP(log_pattern));
  }

  template<typename SerializationContext>
  auto serialize(SerializationContext& context) -> void
  {
    base_serialize(context);
  }
};

struct gl_logger_config : public logger_config
{
  optional<bool> log_debug_messenger;
  std::vector<size_t> disabled_ids {};

  template<typename SerializationContext>
  auto serialize(SerializationContext& context) -> void
  {
    base_serialize(context);
    context(AMEKO_SERIALIZATION_NVP(log_debug_messenger));
    context(AMEKO_SERIALIZATION_NVP(disabled_ids));
  }
};

struct logging_config
{
  logger_config ameko;
  gl_logger_config gl;
  logger_config ffmpeg;

  template<typename SerializationContext>
  auto serialize(SerializationContext& context) -> void
  {
    context(AMEKO_SERIALIZATION_NVP(ameko));
    context(AMEKO_SERIALIZATION_NVP(gl));
    context(AMEKO_SERIALIZATION_NVP(ffmpeg));
  }
};
}  // namespace ameko
