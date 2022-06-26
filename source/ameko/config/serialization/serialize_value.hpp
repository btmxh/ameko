#pragma once

#include <cstdint>
#include <filesystem>
#include <iosfwd>
#include <memory>
#include <string>
#include <string_view>
#include <unordered_map>
#include <variant>
#include <vector>

namespace ameko
{
class serialize_value;
using serialize_string = std::string;
using serialize_number = double;
using serialize_bool = bool;
using serialize_array = std::vector<serialize_value>;
using serialize_object = std::unordered_map<std::string, serialize_value>;

class serialize_value
{
public:
  explicit serialize_value(serialize_string string_value)
      : m_value(std::move(string_value))
  {
  }

  explicit serialize_value(serialize_number number_value)
      : m_value(number_value)
  {
  }

  explicit serialize_value(serialize_bool bool_value)
      : m_value(bool_value)
  {
  }

  explicit serialize_value(std::unique_ptr<serialize_array> array_value)
      : m_value(std::move(array_value))
  {
  }

  explicit serialize_value(std::unique_ptr<serialize_object> object_value)
      : m_value(std::move(object_value))
  {
  }

  [[nodiscard]] auto is_string() const -> bool
  {
    return std::holds_alternative<serialize_string>(m_value);
  }

  [[nodiscard]] auto is_number() const -> bool
  {
    return std::holds_alternative<serialize_number>(m_value);
  }

  [[nodiscard]] auto is_bool() const -> bool
  {
    return std::holds_alternative<serialize_bool>(m_value);
  }

  [[nodiscard]] auto is_array() const -> bool
  {
    return std::holds_alternative<std::unique_ptr<serialize_array>>(m_value);
  }

  [[nodiscard]] auto is_object() const -> bool
  {
    return std::holds_alternative<std::unique_ptr<serialize_object>>(m_value);
  }

  [[nodiscard]] auto get_string() -> serialize_string&
  {
    return std::get<serialize_string>(m_value);
  }

  [[nodiscard]] auto get_number() -> serialize_number&
  {
    return std::get<serialize_number>(m_value);
  }

  [[nodiscard]] auto get_bool() -> serialize_bool&
  {
    return std::get<serialize_bool>(m_value);
  }

  [[nodiscard]] auto get_array() -> serialize_array&
  {
    return *std::get<std::unique_ptr<serialize_array>>(m_value);
  }

  [[nodiscard]] auto get_object() -> serialize_object&
  {
    return *std::get<std::unique_ptr<serialize_object>>(m_value);
  }

  [[nodiscard]] auto get_string() const -> const serialize_string&
  {
    return std::get<serialize_string>(m_value);
  }

  [[nodiscard]] auto get_number() const -> const serialize_number&
  {
    return std::get<serialize_number>(m_value);
  }

  [[nodiscard]] auto get_bool() const -> const serialize_bool&
  {
    return std::get<serialize_bool>(m_value);
  }

  [[nodiscard]] auto get_array() const -> const serialize_array&
  {
    return *std::get<std::unique_ptr<serialize_array>>(m_value);
  }

  [[nodiscard]] auto get_object() const -> const serialize_object&
  {
    return *std::get<std::unique_ptr<serialize_object>>(m_value);
  }

private:
  std::variant<serialize_string,
               serialize_number,
               serialize_bool,
               std::unique_ptr<serialize_array>,
               std::unique_ptr<serialize_object>>
      m_value;
};

}  // namespace ameko
