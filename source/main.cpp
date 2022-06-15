#include <iostream>
#include <string>

#include "lib.hpp"

auto main() noexcept -> int
{
  try {
    auto const lib = library {};
    auto const message = "Hello from " + lib.name + "!";
    std::cout << message << '\n';
    return 0;
  } catch (...) {
    return 1;
  }
}
