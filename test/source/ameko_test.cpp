#include <catch2/catch.hpp>

#include "lib.hpp"

TEST_CASE("Name is ameko", "[library]")
{
  auto const lib = library {};
  REQUIRE(lib.name == "ameko");
}
