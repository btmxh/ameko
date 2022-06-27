#pragma once

#include "serializer.hpp"

namespace ameko
{
class toml_serializer : public serializer
{
public:
  toml_serializer() = default;

  auto serialize(const serialize_value& value,
                 std::ostream& output,
                 std::string_view output_path) -> void override;
  auto deserialize(std::istream& input, std::string_view input_path)
      -> serialize_value override;
};
}  // namespace ameko
