#pragma once

#include <functional>

#include "../../graphics_context.hpp"

namespace ameko
{
// must be a C function pointer since glad can't do the capturing thingy
using gl_load_proc = void* (*)(const char*);
using gl_make_context_current_proc = std::function<void(bool)>;

auto create_opengl_graphics_context(
    bool gl_es,
    gl_load_proc load_proc,
    gl_make_context_current_proc make_context_current_proc)
    -> std::unique_ptr<graphics_context>;
}  // namespace ameko
