#include <iostream>
#include <string>

#include "ameko/game.hpp"
#include "ameko/logging/log.hpp"
#include "ameko/utils/optional.hpp"

static auto get_exception_message() noexcept
    -> ameko::optional<std::string_view>
{
  try {
    throw;
  } catch (std::exception& ex) {
    return ameko::make_optional<std::string_view>(ex.what());
  } catch (...) {
    return ameko::nullopt;
  }
}

static auto log_error() -> int
{
  try {
    auto message = get_exception_message();
    std::cerr << "Fatal error occurred";
    if (message.has_value()) {
      std::cerr << ": " << message.value();
    }
    std::cerr << "\n";
    return 1;
  } catch (...) {
    // your system is beyond broken
    return 2;
  }
}

auto main() noexcept -> int
{
  try {
    ameko::game game {};
    game.run();
    return 0;
  } catch (...) {
    return log_error();
  }
}
