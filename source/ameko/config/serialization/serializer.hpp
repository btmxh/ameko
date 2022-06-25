#pragma once

#include <iosfwd>
#include <string_view>

#include "serialize_value.hpp"
#include "../../utils/enum.hpp"

namespace ameko
{
enum class serialize_format { toml };

class serializer
{
public:
  serializer() = default;
  virtual ~serializer() = default;

  serializer(const serializer&) = delete;
  serializer(serializer&&) = delete;
  auto operator=(const serializer&) = delete;
  auto operator=(serializer&&) = delete;

  virtual auto serialize(const serialize_value& value,
                         std::ostream& output,
                         std::string_view output_path) -> void = 0;
  virtual auto deserialize(std::istream& input, std::string_view input_path)
      -> serialize_value = 0;
};

auto create_serializer(serialize_format format) -> std::unique_ptr<serializer>;
}  // namespace ameko
