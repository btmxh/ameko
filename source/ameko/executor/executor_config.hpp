#pragma once

#include <algorithm>
#include <array>
#include <vector>

#include "../config/serialization/serialization_context.hpp"
#include "../utils/optional.hpp"
#include "executor_mode.hpp"
#include "threads/thread.hpp"

namespace ameko
{

struct executor_loop_config
{
  executor_thread_id thread_id;
  optional<double> relative_frequency;

  AMEKO_TWO_WAY_SERIALIZE_FUNC(context, {
    context(AMEKO_SERIALIZATION_NVP(thread_id));

    if constexpr (context.is_save) {
      if (!relative_frequency.has_value() || relative_frequency.value() == 1.0)
      {
        return;
      }
    }

    context(AMEKO_SERIALIZATION_NVP(relative_frequency));
  })
};

struct executor_mode_config
{
  optional<std::string> display_name;

  executor_loop_config event;
  executor_loop_config update;
  executor_loop_config render;
  executor_loop_config audio;

  std::array<double, max_num_executor_threads> executor_thread_frequencies;
  optional<vsync_mode> vsync;

  AMEKO_TWO_WAY_SERIALIZE_FUNC(context, {
    context(AMEKO_SERIALIZATION_NVP(display_name));
    context(AMEKO_SERIALIZATION_NVP(event));
    context(AMEKO_SERIALIZATION_NVP(update));
    context(AMEKO_SERIALIZATION_NVP(render));
    context(AMEKO_SERIALIZATION_NVP(audio));

    std::vector<double> thread_frequencies;
    if constexpr (context.is_save) {
      thread_frequencies.resize(4);
      std::copy(executor_thread_frequencies.begin(),
                executor_thread_frequencies.end(),
                thread_frequencies.begin());
    }

    context(AMEKO_SERIALIZATION_NVP(thread_frequencies));

    if constexpr (context.is_load) {
      for (size_t i = 0; i < executor_thread_frequencies.size(); ++i) {
        executor_thread_frequencies[i] =
            i >= thread_frequencies.size() ? 0.0 : thread_frequencies[i];
      }
    }

    context(AMEKO_SERIALIZATION_NVP(vsync));
  })
};

struct executor_config
{
  std::vector<executor_mode_config> executor_modes;
  vsync_mode default_vsync_mode;
  size_t current_mode_index = 0;

  AMEKO_TWO_WAY_SERIALIZE_FUNC(context, {
    context(AMEKO_SERIALIZATION_NVP(executor_modes));
    context(AMEKO_SERIALIZATION_NVP(default_vsync_mode));
    context(AMEKO_SERIALIZATION_NVP(current_mode_index));
  })
};

}  // namespace ameko
