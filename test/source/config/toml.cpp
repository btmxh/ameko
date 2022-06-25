#include <memory>
#include <sstream>

#include <ameko/config/config.hpp>
#include <catch2/catch.hpp>

template<typename T>
using own = std::unique_ptr<T>;

using own_value = own<ameko::config_value>;

TEST_CASE("TOML config parsing", "[config]")
{
  auto serializer = ameko::create_config_serializer(ameko::config_format::toml);
  SECTION("serialize")
  {
    auto obj = std::make_unique<ameko::config_object>();
    obj->emplace("a", ameko::config_value(static_cast<ameko::config_number>(2)));
    obj->emplace("b", ameko::config_value(static_cast<ameko::config_number>(32)));
    obj->emplace("c", ameko::config_value(static_cast<ameko::config_number>(4)));
    own_value value = std::make_unique<ameko::config_value>(std::move(obj));
    std::stringstream str_stream;
    serializer->serialize(*value, str_stream, "<unknown>");
    auto output = str_stream.str();
    REQUIRE(output == "a = 2.0\nb = 32.0\nc = 4.0");
  }
}