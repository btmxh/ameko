cmake_minimum_required(VERSION 3.14)

include(cmake/prelude.cmake)

project(
    ameko
    VERSION 0.1.0
    DESCRIPTION "haha ortensia reference"
    HOMEPAGE_URL "https://example.com/"
    LANGUAGES CXX
)

include(cmake/project-is-top-level.cmake)
include(cmake/variables.cmake)

# ---- Declare library ----

set(AMEKO_SOURCES
  source/ameko/game.cpp
  source/ameko/config/global_config.cpp
  source/ameko/config/serialization/toml_serializer.cpp
  source/ameko/executor/executor.cpp
  source/ameko/executor/threads/thread.cpp
  source/ameko/executor/loops/event_loop.cpp
  source/ameko/executor/loops/update_loop.cpp
  source/ameko/executor/loops/render_loop.cpp
  source/ameko/executor/loops/audio_loop.cpp
  source/ameko/executor/sync/of_sync.cpp
  source/ameko/display/display.cpp
  source/ameko/display/backends/dummy/dummy_display.cpp
  source/ameko/graphics/backends/dummy/dummy_graphics_context.cpp
  source/ameko/input/backends/dummy/dummy_input_context.cpp
  source/ameko/logging/log.cpp
)

find_package(fmt CONFIG REQUIRED)
find_package(tl-optional CONFIG REQUIRED)
find_package(tomlplusplus CONFIG REQUIRED)
find_package(spdlog CONFIG REQUIRED)

find_package(glfw3 CONFIG)
if(glfw3_FOUND)
  list(APPEND AMEKO_SOURCES
    source/ameko/display/backends/glfw/glfw_context.cpp
    source/ameko/display/backends/glfw/glfw_display.cpp
    source/ameko/display/backends/glfw/glfw_display_internal.cpp
    source/ameko/input/backends/glfw/glfw_input_context.cpp
  )
endif()

find_package(glad CONFIG)
if(glad_FOUND)
  list(APPEND AMEKO_SOURCES
    source/ameko/graphics/backends/gl/gl_context.cpp
  )
endif()

add_library(ameko_lib OBJECT ${AMEKO_SOURCES})

target_include_directories(
    ameko_lib ${warning_guard}
    PUBLIC
    "$<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/source>"
)

target_compile_features(ameko_lib PUBLIC cxx_std_17)

target_link_libraries(ameko_lib PUBLIC fmt::fmt tl::optional tomlplusplus::tomlplusplus spdlog::spdlog)

if(glfw3_FOUND)
  target_link_libraries(ameko_lib PUBLIC glfw)
  target_compile_definitions(ameko_lib PUBLIC AMEKO_HAS_GLFW)
  target_compile_definitions(ameko_lib PUBLIC GLFW_INCLUDE_NONE)
endif()

if(glad_FOUND)
  target_link_libraries(ameko_lib PUBLIC glad::glad)
  target_compile_definitions(ameko_lib PUBLIC AMEKO_HAS_OPENGL)
  target_compile_definitions(ameko_lib PUBLIC AMEKO_HAS_OPENGL_ES)
  target_compile_definitions(ameko_lib PUBLIC AMEKO_HAS_GLAD)
endif()

# ---- Declare executable ----

add_executable(ameko_exe source/main.cpp)
add_executable(ameko::exe ALIAS ameko_exe)

set_target_properties(
    ameko_exe PROPERTIES
    OUTPUT_NAME ameko
    EXPORT_NAME exe
)

target_compile_features(ameko_exe PRIVATE cxx_std_17)

target_link_libraries(ameko_exe PRIVATE ameko_lib)

# ---- Install rules ----

if(NOT CMAKE_SKIP_INSTALL_RULES)
  include(cmake/install-rules.cmake)
endif()

# ---- Developer mode ----

if(NOT ameko_DEVELOPER_MODE)
  return()
elseif(NOT PROJECT_IS_TOP_LEVEL)
  message(
      AUTHOR_WARNING
      "Developer mode is intended for developers of ameko"
  )
endif()

include(cmake/dev-mode.cmake)
