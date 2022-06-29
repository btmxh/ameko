#include <fstream>

#include "global_config.hpp"

#include "../logging/log.hpp"
#include "serialization/toml_serializer.hpp"

namespace ameko
{

auto load_global_config(const std::filesystem::path& config_file)
    -> global_config
{
  global_config config {};
  config.config_file = config_file;

  try {
    toml_serializer serializer;
    std::ifstream stream(config_file);
    stream.exceptions(std::ios::failbit);

    auto config_file_path = config_file.string();
    auto serialized_value = serializer.deserialize(stream, config_file_path);
    serialize_load<global_config>(serialized_value, config);
  } catch (std::exception& ex) {
    log_warn(ameko_logger_name,
             "unable to load config file from '{}': {}",
             config_file.string(),
             ex.what());
  }

  return config;
}

auto save_global_config(global_config& config) -> void
{
  try {
    toml_serializer serializer;
    std::ofstream stream(config.config_file);
    stream.exceptions(std::ios::failbit);

    auto config_file_path = config.config_file.string();
    auto config_serialize_value = serialize_save(config);
    remove_empty_constructs(config_serialize_value);

    serializer.serialize(config_serialize_value, stream, config_file_path);
  } catch (std::exception& ex) {
    log_error(ameko_logger_name,
              "unable to save config file to '{}': {}",
              config.config_file.string(),
              ex.what());
  }
}

}  // namespace ameko