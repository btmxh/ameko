#include <cmath>
#include <cstdint>
#include <iostream>
#include <memory>

#include "toml_serializer.hpp"

#include <toml++/toml.h>

namespace ameko
{
// NOLINTBEGIN(*-no-recursion)
static auto toml_value_to_serialize_value(const toml::node& toml_value)
    -> serialize_value
{
  return toml_value.visit(
      [](const auto& node) -> serialize_value
      {
        using type = decltype(node);
        if constexpr (toml::is_string<type>) {
          return serialize_value {*node};
        } else if constexpr (toml::is_number<type>) {
          return serialize_value {static_cast<serialize_number>(*node)};
        } else if constexpr (toml::is_boolean<type>) {
          return serialize_value {static_cast<serialize_bool>(*node)};
        } else if constexpr (toml::is_array<type>) {
          auto array = std::make_unique<serialize_array>();
          for (const auto& element : node) {
            array->push_back(toml_value_to_serialize_value(element));
          }
          return serialize_value {std::move(array)};
        } else if constexpr (toml::is_table<type>) {
          auto map = std::make_unique<serialize_object>();
          for (const auto& [key, value] : node) {
            map->emplace(key.str(), toml_value_to_serialize_value(value));
          }
          return serialize_value {std::move(map)};
        } else {
          throw std::runtime_error("unsupported toml type");
        }
      });
}

static auto serialize_value_to_toml_value(
    const serialize_value& serialize_value) -> std::unique_ptr<toml::node>
{
  if (serialize_value.is_array()) {
    const auto& serialize_array = serialize_value.get_array();
    auto toml_array = std::make_unique<toml::array>();
    for (const auto& serialize_array_elem : serialize_array) {
      auto node_ptr = serialize_value_to_toml_value(serialize_array_elem);
      toml_array->push_back(*node_ptr);
    }

    return toml_array;
  }

  if (serialize_value.is_bool()) {
    return std::make_unique<toml::value<bool>>(serialize_value.get_bool());
  }

  if (serialize_value.is_number()) {
    auto value = serialize_value.get_number();
    // this will make toml++ format integers without ".0"
    if (std::trunc(value) == value
        && value < static_cast<double>(std::numeric_limits<int64_t>::max())
        && value > static_cast<double>(std::numeric_limits<int64_t>::min()))
    {
      return std::make_unique<toml::value<int64_t>>(
          static_cast<int64_t>(value));
    }

    return std::make_unique<toml::value<serialize_number>>(
        serialize_value.get_number());
  }

  if (serialize_value.is_object()) {
    const auto& serialize_object = serialize_value.get_object();
    auto toml_object = std::make_unique<toml::table>();
    for (const auto& [key, value] : serialize_object) {
      auto node_ptr = serialize_value_to_toml_value(value);
      toml_object->insert(key, *node_ptr);
    }

    return toml_object;
  }

  if (serialize_value.is_string()) {
    return std::make_unique<toml::value<std::string>>(
        serialize_value.get_string());
  }

  throw std::runtime_error("invalid serialize value");
}
// NOLINTEND(*-no-recursion)

auto toml_serializer::serialize(const serialize_value& value,
                                std::ostream& output,
                                [[maybe_unused]] std::string_view output_path)
    -> void
{
  auto toml = serialize_value_to_toml_value(value);
  output << toml::toml_formatter(*toml);
}

auto toml_serializer::deserialize(std::istream& input,
                                  std::string_view input_path)
    -> serialize_value
{
  auto toml_data = toml::parse(input, input_path);
  return toml_value_to_serialize_value(toml_data);
}
}  // namespace ameko
