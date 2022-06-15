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

add_library(
    ameko_lib OBJECT
    source/ameko/game.cpp
)

target_include_directories(
    ameko_lib ${warning_guard}
    PUBLIC
    "$<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/source>"
)

target_compile_features(ameko_lib PUBLIC cxx_std_17)

find_package(fmt REQUIRED)
target_link_libraries(ameko_lib PRIVATE fmt::fmt)

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
