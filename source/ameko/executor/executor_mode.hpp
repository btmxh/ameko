#pragma once

#include <array>

#include "../utils/enum.hpp"
#include "threads/thread.hpp"

namespace ameko
{
enum class vsync_mode
{
  automatic,
  off,
  on
};

AMEKO_GENERATE_ENUM_STRING_CONVERTER(vsync_mode,
                                     AMEKO_ENUM_NVP(vsync_mode, automatic),
                                     AMEKO_ENUM_NVP(vsync_mode, off),
                                     AMEKO_ENUM_NVP(vsync_mode, on));

struct executor_loop_mode
{
  executor_thread_id thread_id;
  double relative_frequency;
};

struct executor_mode
{
  executor_loop_mode event;
  executor_loop_mode update;
  executor_loop_mode render;
  executor_loop_mode audio;

  std::array<double, max_num_executor_threads> executor_thread_frequencies;
  vsync_mode vsync;
};
}  // namespace ameko