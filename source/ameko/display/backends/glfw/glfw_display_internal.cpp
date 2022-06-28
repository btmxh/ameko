#include <stdexcept>

#include "../../../dependencies/gl.hpp"

#include <fmt/core.h>

#include "../../../graphics/backends/dummy/dummy_graphics_context.hpp"
#include "../../../graphics/backends/gl/gl_context.hpp"
#include "../../../graphics/graphics_config.hpp"
#include "../../../utils/optional.hpp"
#include "glfw_context.hpp"
#include "glfw_display_internal.hpp"

namespace ameko
{

glfw_window_user_pointer::glfw_window_user_pointer(GLFWwindow* window)
{
  glfwSetWindowUserPointer(window, this);
}

static auto get_primary_monitor()
    -> std::tuple<GLFWmonitor*, const GLFWvidmode*>
{
  auto* primary_monitor = glfwGetPrimaryMonitor();
  if (primary_monitor == nullptr) {
    int num_monitors = 0;
    auto** monitors = glfwGetMonitors(&num_monitors);
    primary_monitor = *monitors;
    if (primary_monitor == nullptr) {
      throw std::runtime_error("primary monitor not found");
    }
  }

  const auto* video_mode = glfwGetVideoMode(primary_monitor);
  if (video_mode == nullptr) {
    throw std::runtime_error("unable to determine primary monitor video mode");
  }

  return std::make_tuple(primary_monitor, video_mode);
}

static auto get_default_graphics_backend() -> graphics_backend
{
  if constexpr (deps::has_opengl) {
    return graphics_backend::opengl;
  } else if constexpr (deps::has_opengl_es) {
    return graphics_backend::opengl_es;
  } else {
    return graphics_backend::dummy;
  }
}

static auto create_graphics_context(graphics_backend backend,
                                    GLFWwindow* window)
    -> std::unique_ptr<graphics_context>
{
  switch (backend) {
    case graphics_backend::opengl:
      if constexpr (deps::has_opengl) {
        return create_opengl_graphics_context(
            /*gl_es=*/false,
            [](const auto* name)
            {
              // NOLINTNEXTLINE(*-reinterpret-cast)
              return reinterpret_cast<void*>(glfwGetProcAddress(name));
            },
            [=](bool current)
            { glfwMakeContextCurrent(current ? window : nullptr); });
      } else {
        throw std::runtime_error("OpenGL graphics backend is unsupported");
      }
    case graphics_backend::opengl_es:
      if constexpr (deps::has_opengl_es) {
        return create_opengl_graphics_context(
            /*gl_es=*/true,
            [](const auto* name)
            {
              // NOLINTNEXTLINE(*-reinterpret-cast)
              return reinterpret_cast<void*>(glfwGetProcAddress(name));
            },
            [&](bool current)
            { glfwMakeContextCurrent(current ? window : nullptr); });
      } else {
        throw std::runtime_error("OpenGL ES graphics backend is unsupported");
      }
    default:
      return std::make_unique<dummy_graphics_context>();
  }
}

glfw_display::glfw_display(display_config& display_config,
                           graphics_config& graphics_config)
    : m_glfw(init_glfw())
    , m_window(
          [&]
          {
            GLFWwindow* window = nullptr;
            std::string title = "ameko";
            glfwDefaultWindowHints();
            glfwWindowHint(GLFW_VISIBLE, GLFW_FALSE);
            switch (graphics_config.backend.value_or(
                lazy_value_or {get_default_graphics_backend}))
            {
              case graphics_backend::opengl:
                glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_API);
                break;
              case graphics_backend::opengl_es:
                glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_ES_API);
                break;
              default:
                glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
                break;
            }

            switch (value_or_assign(display_config.mode,
                                    display_mode::AMEKO_DEFAULT_WINDOW_MODE))
            {
              case display_mode::windowed: {
                const auto& size = or_else_assign(
                    display_config.size,
                    []
                    {
                      constexpr extent2<int> default_size(1280, 720);
                      auto [monitor, video_mode] = get_primary_monitor();
                      if (video_mode->width < default_size.width
                          || video_mode->height < default_size.height)
                      {
                        return make_optional<extent2z>(
                            static_cast<size_t>(video_mode->width / 2),
                            static_cast<size_t>(video_mode->height / 2));
                      }

                      return make_optional<extent2z>(default_size.width,
                                                     default_size.height);
                    });
                window = glfwCreateWindow(static_cast<int>(size.width),
                                          static_cast<int>(size.height),
                                          title.c_str(),
                                          nullptr,
                                          nullptr);
                break;
              }
              case display_mode::fullscreen: {
                auto [monitor, video_mode] = get_primary_monitor();
                window = glfwCreateWindow(video_mode->width,
                                          video_mode->height,
                                          title.c_str(),
                                          monitor,
                                          nullptr);
                break;
              }

              case display_mode::borderless_fullscreen: {
                auto [monitor, video_mode] = get_primary_monitor();
                glfwWindowHint(GLFW_DECORATED, GLFW_FALSE);

                window = glfwCreateWindow(video_mode->width,
                                          video_mode->height,
                                          title.c_str(),
                                          nullptr,
                                          nullptr);
                offset2<int> monitor_offset;
                glfwGetMonitorPos(
                    monitor, &monitor_offset.x, &monitor_offset.y);
                glfwSetWindowPos(window, monitor_offset.x, monitor_offset.y);
                break;
              }

              default:
                throw std::runtime_error("invalid window mode");
            }

            glfwMakeContextCurrent(window);
            return window;
          }(),
          glfwDestroyWindow)
    , m_user_pointer(m_window.get())
    , m_input_context(m_window.get())
    , m_graphics_context(create_graphics_context(
          graphics_config.backend.value_or(
              lazy_value_or {get_default_graphics_backend}),
          m_window.get()))
{
  glfwSwapInterval(1);
  m_vsync = true;

  glfwSetWindowCloseCallback(m_window.get(),
                             [](GLFWwindow* window)
                             {
                               auto user_ptr =
                                   get_glfw_window_user_pointer(window);
                               if (user_ptr.on_close != nullptr) {
                                 user_ptr.on_close();
                               }
                             });
}

glfw_display::~glfw_display() = default;

auto glfw_display::set_visible(bool visible) -> void
{
  if (visible) {
    glfwShowWindow(m_window.get());
  } else {
    glfwHideWindow(m_window.get());
  }
}

auto glfw_display::poll_events(bool block) -> void
{
  constexpr static double timeout = 0.05;  // 50ms
  if (block) {
    // in theory glfwWaitEvents() can work here, but in case something happens
    glfwWaitEventsTimeout(timeout);
  } else {
    glfwPollEvents();
  }
}

auto glfw_display::unblock_poll_events() -> void
{
  glfwPostEmptyEvent();
}

auto glfw_display::swap_buffers() -> void
{
  glfwSwapBuffers(m_window.get());
}

auto glfw_display::set_vsync(bool vsync) -> void
{
  if (!m_vsync.has_value() || m_vsync.value() != vsync) {
    glfwSwapInterval(vsync ? 1 : 0);
  }
}

auto glfw_display::close() -> void
{
  glfwSetWindowShouldClose(m_window.get(), GLFW_TRUE);
}

auto glfw_display::set_on_close(std::function<void()> callback) -> void
{
  if (glfwWindowShouldClose(m_window.get()) != GLFW_FALSE) {
    callback();
  }

  m_user_pointer.on_close = callback;
}

auto glfw_display::get_input_context() -> input_context&
{
  return m_input_context;
}

auto glfw_display::get_graphics_context() -> graphics_context&
{
  return *m_graphics_context;
}

auto get_glfw_window_user_pointer(GLFWwindow* window)
    -> glfw_window_user_pointer&
{
  return *static_cast<glfw_window_user_pointer*>(
      glfwGetWindowUserPointer(window));
}

}  // namespace ameko
