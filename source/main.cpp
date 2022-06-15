#include <iostream>
#include <string>

#include "ameko/game.hpp"

auto main() noexcept -> int
{
  try {
    ameko::game game {};
    game.run();
    return 0;
  } catch (...) {
    return 1;
  }
}
