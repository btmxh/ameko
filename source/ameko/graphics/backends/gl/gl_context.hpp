#pragma once

#include <functional>

#include "../../graphics_context.hpp"

namespace ameko
{
// must be a C function pointer since glad can't do the capturing thingy
using gl_load_proc = void* (*)(const char*);
using gl_make_context_current_proc = std::function<void(bool)>;

class gl_context : public graphics_context
{
public:
  gl_context(gl_load_proc load_proc,
             gl_make_context_current_proc make_context_current_proc);

  ~gl_context() = default;

  gl_context(const gl_context&) = delete;
  gl_context(gl_context&&) = delete;
  auto operator=(const gl_context&) = delete;
  auto operator=(gl_context&&) = delete;

  auto make_current_on_thread(bool current) -> void override;
  auto load_gl_proc(const char* name) -> void*;

private:
  gl_load_proc m_load_proc;
  gl_make_context_current_proc m_make_context_current_proc;
};
}  // namespace ameko
