#include "serialize_value.hpp"

namespace ameko
{
auto is_empty_construct(const serialize_value& value) -> bool
{
  return (value.is_array() && value.get_array().empty())
      || (value.is_object() && value.get_object().empty());
}

auto remove_empty_constructs(serialize_value& value) -> void
{
  if (value.is_array()) {
    auto& array = value.get_array();
    for (auto& elem : array) {
      remove_empty_constructs(elem);
    }
  }

  if (value.is_object()) {
    auto& object = value.get_object();
    std::vector<const std::string*> remove_keys;
    for (auto& [pair_key, pair_value] : object) {
      remove_empty_constructs(pair_value);
      if (is_empty_construct(pair_value)) {
        remove_keys.push_back(&pair_key);
      }
    }

    for (const auto* remove_key : remove_keys) {
      object.erase(*remove_key);
    }
  }
}

}  // namespace ameko
