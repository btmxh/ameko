#pragma once

#include "log.hpp"

namespace ameko
{
class log_context
{
public:
  log_context() { setup_loggers(); }
};
}  // namespace ameko
