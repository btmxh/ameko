#pragma once

#include <string_view>

#include <fmt/core.h>
#include <spdlog/spdlog.h>

namespace ameko
{
using log_level = spdlog::level::level_enum;

inline constexpr std::string_view ameko_logger_name = "ameko";
inline constexpr std::string_view gl_logger_name = "gl";
inline constexpr std::string_view ffmpeg_logger_name = "ffmpeg";

auto log_string(std::string_view logger_name,
                log_level level,
                std::string_view message) -> void;

auto create_spdlog_logger(std::string_view logger_name)
    -> std::shared_ptr<spdlog::logger>;
auto get_spdlog_logger(std::string_view logger_name)
    -> std::shared_ptr<spdlog::logger>;

inline auto get_or_create_spdlog_logger(std::string_view logger_name)
{
  auto logger = get_spdlog_logger(logger_name);
  return logger == nullptr ? create_spdlog_logger(logger_name) : logger;
}

// this is called before loading any config files
auto setup_loggers() -> void;
// this is called after the global config file (which contains the logger
// config) was loaded
struct logging_config;
auto setup_loggers_from_config(const logging_config& config) -> void;

template<typename FormatStr, typename... Args>
inline auto log(std::string_view logger_name,
                log_level level,
                FormatStr&& format_string,
                Args&&... args)
{
  auto formatted_string = fmt::format(std::forward<FormatStr>(format_string),
                                      std::forward<Args>(args)...);
  log_string(logger_name, level, formatted_string);
}

template<typename FormatStr, typename... Args>
inline auto log_trace(std::string_view logger_name,
                      FormatStr&& format_string,
                      Args&&... args)
{
  log(logger_name,
      log_level::trace,
      std::forward<FormatStr>(format_string),
      std::forward<Args>(args)...);
}

template<typename FormatStr, typename... Args>
inline auto log_debug(std::string_view logger_name,
                      FormatStr&& format_string,
                      Args&&... args)
{
  log(logger_name,
      log_level::debug,
      std::forward<FormatStr>(format_string),
      std::forward<Args>(args)...);
}

template<typename FormatStr, typename... Args>
inline auto log_info(std::string_view logger_name,
                     FormatStr&& format_string,
                     Args&&... args)
{
  log(logger_name,
      log_level::info,
      std::forward<FormatStr>(format_string),
      std::forward<Args>(args)...);
}

template<typename FormatStr, typename... Args>
inline auto log_warn(std::string_view logger_name,
                     FormatStr&& format_string,
                     Args&&... args)
{
  log(logger_name,
      log_level::warn,
      std::forward<FormatStr>(format_string),
      std::forward<Args>(args)...);
}

template<typename FormatStr, typename... Args>
inline auto log_error(std::string_view logger_name,
                      FormatStr&& format_string,
                      Args&&... args)
{
  log(logger_name,
      log_level::err,
      std::forward<FormatStr>(format_string),
      std::forward<Args>(args)...);
}

template<typename FormatStr, typename... Args>
inline auto log_critical(std::string_view logger_name,
                         FormatStr&& format_string,
                         Args&&... args)
{
  log(logger_name,
      log_level::critical,
      std::forward<FormatStr>(format_string),
      std::forward<Args>(args)...);
}

}  // namespace ameko
