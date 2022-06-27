#pragma once

#include <memory>
#include <string>
#include <string_view>
#include <utility>

#include "../../utils/enum.hpp"
#include "../../utils/optional.hpp"
#include "../../utils/type_traits.hpp"
#include "serialize_value.hpp"

// NOLINTBEGIN(*-macro-usage, *-macro-parentheses)
#define AMEKO_SERIALIZATION_NVP(x) #x, x
#define AMEKO_TWO_WAY_SERIALIZE_FUNC(context, func_content) \
  AMEKO_TWO_WAY_SERIALIZE_FUNC_NAMED(serialize, context, func_content)
#define AMEKO_TWO_WAY_SERIALIZE_FUNC_NAMED(func_name, context, func_content) \
  template<typename SerializationContext> \
  auto func_name(SerializationContext& context)->void \
  { \
    func_content \
  }
// NOLINTEND(*-macro-usage, *-macro-parentheses)

namespace ameko
{
template<typename T>
inline auto serialize_save(T& value) -> serialize_value;
template<typename T>
inline auto serialize_load(const serialize_value& value, T& loaded_value)
    -> void;

struct save_serialization_context
{
  static constexpr bool is_save = true;
  static constexpr bool is_load = false;
  save_serialization_context()
      : m_value(std::make_unique<serialize_object>())
  {
  }

  template<typename T>
  auto operator()(std::string_view name,
                  T& value,
                  [[maybe_unused]] bool optional = false) -> void
  {
    if constexpr (is_optional_v<T>) {
      if (value.has_value()) {
        insert(name, serialize_save(value.value()));
      }
    } else {
      insert(name, serialize_save(value));
    }
  }

  auto get_value() -> serialize_value
  {
    return serialize_value {std::move(m_value)};
  }

private:
  auto insert(std::string_view name, serialize_value&& value) -> void
  {
    m_value->try_emplace(std::string(name), std::move(value));
  }

  std::unique_ptr<serialize_object> m_value;
};

struct load_serialization_context
{
  static constexpr bool is_save = false;
  static constexpr bool is_load = true;

  explicit load_serialization_context(const serialize_object* value)
      : m_value(value)
  {
  }

  template<typename T>
  auto operator()(std::string_view name,
                  T& value,
                  [[maybe_unused]] bool optional = false) -> void
  {
    if constexpr (is_optional_v<T>) {
      const auto* serialize_value = get(name);
      if (serialize_value == nullptr) {
        value = nullopt;
      } else {
        serialize_load(*serialize_value, value.emplace());
      }
    } else {
      const auto* serialize_value = get(name);
      if (serialize_value == nullptr) {
        if (!optional) {
          throw std::runtime_error("missing property");
        }

        return;
      }

      serialize_load(*serialize_value, value);
    }
  }

private:
  const serialize_object* m_value;

  auto get(std::string_view name) -> const serialize_value*
  {
    if (m_value == nullptr) {
      return nullptr;
    }

    auto iterator = m_value->find(std::string(name));
    return iterator == m_value->end() ? nullptr : &iterator->second;
  }
};

template<typename T, typename = void>
struct serialize_helper
{
  static auto save(T& value) -> serialize_value
  {
    save_serialization_context context;
    value.serialize(context);
    return context.get_value();
  }

  static auto load(const serialize_value& value, T& loaded_value) -> void
  {
    if (!value.is_object()) {
      throw std::runtime_error("invalid value");
    }
    load_serialization_context context {&value.get_object()};
    loaded_value.serialize(context);
  }
};

template<typename T>
inline auto serialize_save(T& value) -> serialize_value
{
  return serialize_helper<remove_cv_t<T>>::save(value);
}

template<typename T>
inline auto serialize_load(const serialize_value& value, T& loaded_value)
    -> void
{
  serialize_helper<remove_cv_t<T>>::load(value, loaded_value);
}

template<typename NumType>
struct serialize_helper<NumType, enable_if_t<is_arithmetic_v<NumType>>>
{
  static auto save(const NumType& value) -> serialize_value
  {
    return serialize_value {static_cast<serialize_number>(value)};
  }

  static auto load(const serialize_value& value, NumType& loaded_value) -> void
  {
    if (!value.is_number()) {
      throw std::runtime_error("invalid value");
    }
    loaded_value = static_cast<NumType>(value.get_number());
  }
};

template<>
struct serialize_helper<std::string>
{
  static auto save(const std::string& value) -> serialize_value
  {
    return serialize_value {value};
  }
  static auto load(const serialize_value& value, std::string& loaded_value)
      -> void
  {
    if (!value.is_string()) {
      throw std::runtime_error("invalid value");
    }
    loaded_value = value.get_string();
  }
};

template<>
struct serialize_helper<bool>
{
  static auto save(const bool& value) -> serialize_value
  {
    return serialize_value {value};
  }
  static auto load(const serialize_value& value, bool& loaded_value) -> void
  {
    if (!value.is_bool()) {
      throw std::runtime_error("invalid value");
    }
    loaded_value = value.get_bool();
  }
};

template<typename T>
struct serialize_helper<std::vector<T>>
{
  static auto save(std::vector<T>& value) -> serialize_value
  {
    auto array_ptr = std::make_unique<std::vector<serialize_value>>();
    array_ptr->reserve(value.size());
    for (auto& element : value) {
      array_ptr->push_back(serialize_save(element));
    }

    return serialize_value {std::move(array_ptr)};
  }

  static auto load(const serialize_value& value, std::vector<T>& loaded_value)
      -> void
  {
    if (!value.is_array()) {
      throw std::runtime_error("invalid value");
    }

    const auto& array = value.get_array();
    loaded_value.clear();
    loaded_value.resize(array.size());

    for (size_t i = 0; i < array.size(); ++i) {
      serialize_load(array[i], loaded_value[i]);
    }
  }
};

template<typename EnumType>
struct serialize_helper<EnumType,
                        enable_if_t<is_string_convertible_enum<EnumType>>>
{
  using convert = enum_string_converter<EnumType>;
  static auto save(const EnumType& value) -> serialize_value
  {
    std::string string_value {convert::to_string(value)};
    return serialize_helper<std::string>::save(string_value);
  }

  static auto load(const serialize_value& value, EnumType& loaded_value) -> void
  {
    std::string string_value;
    serialize_helper<std::string>::load(value, string_value);
    loaded_value = convert::from_string(string_value);
  }
};
}  // namespace ameko
