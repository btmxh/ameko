#include <filesystem>
#include <functional>

#include "log.hpp"

#include "logging_config.hpp"

// fallback
#include <iostream>

#include <spdlog/sinks/basic_file_sink.h>
#include <spdlog/sinks/rotating_file_sink.h>
#include <spdlog/sinks/stdout_color_sinks.h>

namespace ameko
{
auto log_string(std::string_view logger_name,
                log_level level,
                std::string_view message) -> void
{
  auto logger = spdlog::get(std::string(logger_name));
  if (logger == nullptr) {
    std::cerr << "attempting to log to uninitialized logger '" << logger_name
              << "': " << message << "\n";
    return;
  }

  logger->log(level, message);
}

auto create_spdlog_logger(std::string_view logger_name)
    -> std::shared_ptr<spdlog::logger>
{
  auto logger = std::make_shared<spdlog::logger>(std::string(logger_name));
  spdlog::register_logger(logger);
  return logger;
}

auto get_spdlog_logger(std::string_view logger_name)
    -> std::shared_ptr<spdlog::logger>
{
  return spdlog::get(std::string(logger_name));
}

constexpr inline std::string_view default_log_pattern =
    "[%H:%M:%S] [%-8n] [%-8l] [thread %5t] %v";

static auto make_colored_pattern(std::string_view pattern, bool color = true)
    -> std::string
{
  std::string out_pattern;
  if (color) {
    out_pattern += "%^";
  }

  out_pattern += pattern;

  if (color) {
    out_pattern += "%$";
  }

  return out_pattern;
}

template<typename SinkType>
static auto init_sink(std::shared_ptr<SinkType>& sink,
                      bool color = false,
                      std::string_view pattern = default_log_pattern,
                      log_level level = log_level::trace)
{
  sink->set_pattern(make_colored_pattern(pattern, color));
  sink->set_level(level);
}

static auto setup_logger(std::string_view logger_name,
                         optional<const logger_config&> config) -> void
{
  auto logger = get_or_create_spdlog_logger(logger_name);
  auto& sinks = logger->sinks();
  sinks.clear();

  if (config.has_value()) {
    const auto& config_value = config.value();
    auto pattern =
        config_value.log_pattern.value_or(std::string(default_log_pattern));
    auto stderr_sink = std::make_shared<spdlog::sinks::stderr_color_sink_mt>();
    init_sink(stderr_sink, /*color=*/true, pattern);
    sinks.push_back(stderr_sink);

    if (config_value.log_path
            .map([](const auto& path) { return !path.empty(); })
            .value_or(false))
    {
      auto log_path = config_value.log_path.value();
      spdlog::sink_ptr file_sink;
      if (config_value.log_rotate) {
        constexpr size_t max_size = 16384;
        constexpr size_t max_files = 4;
        file_sink = std::make_shared<spdlog::sinks::rotating_file_sink_mt>(
            log_path, max_size, max_files);
      } else {
        file_sink =
            std::make_shared<spdlog::sinks::basic_file_sink_mt>(log_path);
      }

      init_sink(file_sink, /*color=*/false, pattern);
      sinks.push_back(file_sink);
    }
  } else {
    auto stderr_sink = std::make_shared<spdlog::sinks::stderr_color_sink_mt>();
    init_sink(stderr_sink, /*color=*/true);
    sinks.push_back(stderr_sink);
  }
}

static auto setup_gl_logger(std::string_view logger_name,
                            optional<const gl_logger_config&> config) -> void
{
  setup_logger(logger_name,
               config.map([](const auto& gl_config) -> const logger_config&
                          { return gl_config; }));
}

auto setup_loggers() -> void
{
  setup_logger(ameko_logger_name, nullopt);
  setup_gl_logger(gl_logger_name, nullopt);
  setup_logger(ffmpeg_logger_name, nullopt);
}

auto setup_loggers_from_config(const logging_config& config) -> void
{
  setup_logger(ameko_logger_name, config.ameko);
  setup_gl_logger(gl_logger_name, config.gl);
  setup_logger(ffmpeg_logger_name, config.ffmpeg);
}
}  // namespace ameko
