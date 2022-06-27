#include "executor.hpp"

#include "sync/of_sync.hpp"

namespace ameko
{
executor::executor()
    : m_main_thread_sync(std::make_unique<of_sync>())
{
}

auto executor::get_thread_optional(executor_thread_id tid)
    -> optional<executor_thread>*
{
  return tid == main_thread_id ? nullptr : &m_threads.at(tid - 1);
}

auto executor::set_mode(const executor_mode& mode) -> void
{
  pause();

  auto old_render_thread_id = m_mode
                                  .map([](const auto& mode_value)
                                       { return mode_value.render.thread_id; })
                                  .value_or(main_thread_id);

  if (old_render_thread_id != mode.render.thread_id) {
    run_in_thread_and_block(
        old_render_thread_id,
        [this]
        { m_loops.render->set_context_current_on_thread(/*current=*/false); });
    run_in_thread_and_block(
        mode.render.thread_id,
        [this]
        { m_loops.render->set_context_current_on_thread(/*current=*/true); });
  }

  m_mode = mode;
  lazy_init_thread(mode.event.thread_id);
  lazy_init_thread(mode.update.thread_id);
  lazy_init_thread(mode.render.thread_id);
  lazy_init_thread(mode.audio.thread_id);

  m_loops.render->set_vsync(
      [&]
      {
        switch (mode.vsync) {
          case vsync_mode::on:
            return true;
          case vsync_mode::off:
            return false;
          default:
            return mode.event.thread_id != mode.render.thread_id
                && mode.update.thread_id != mode.render.thread_id
                && mode.audio.thread_id != mode.render.thread_id;
        }
      }());

  resume();
}

auto executor::start(executor_loops loops) -> void
{
  m_running = true;
  m_loops = std::move(loops);
  for (auto& thread : m_threads) {
    if (thread.has_value()) {
      thread->start();
    }
  }
}

auto executor::main_thread_run() -> void
{
  while (m_running) {
    run_single_loop(main_thread_id, *m_main_thread_sync);
  }
}

auto executor::pause() -> void
{
  for (auto& thread : m_threads) {
    if (thread.has_value()) {
      thread->switch_to_idle_mode();
    }
  }

  // event loop may be blocking (by glfwWaitEvents() for example)
  // to unblock it, call cancel_loop() (which will translate to
  // glfwPostEmptyEvent() for example)
  m_loops.event->cancel_loop();
}

auto executor::resume() -> void
{
  for (auto& thread : m_threads) {
    if (thread.has_value()) {
      thread->switch_to_loop_mode();
    }
  }
}

auto executor::stop() -> void
{
  for (auto& thread : m_threads) {
    if (thread.has_value()) {
      thread->stop();
    }
  }
  m_running = false;
}

auto executor::join() -> void
{
  for (auto& thread : m_threads) {
    if (thread.has_value()) {
      thread->join();
    }
  }
}

auto executor::lazy_init_thread(executor_thread_id tid) -> void
{
  if (tid == main_thread_id) {
    return;
  }

  auto& opt_thread = *get_thread_optional(tid);
  if (!opt_thread.has_value()) {
    auto& thread = opt_thread.emplace(*this, tid);
    if (m_running) {
      thread.start();
    }
  }
}

auto executor::run_single_loop(executor_thread_id tid, sync& sync) -> void
{
  if (!m_mode.has_value()) {
    return;
  }

  std::vector<std::pair<executor_loop*, double*>> loops;
  loops.reserve(max_num_executor_threads);
  if (m_mode->event.thread_id == tid) {
    m_relative_event_timer += m_mode->event.relative_frequency;
    loops.emplace_back(m_loops.event.get(), &m_relative_event_timer);
  }
  if (m_mode->update.thread_id == tid) {
    m_relative_update_timer += m_mode->update.relative_frequency;
    loops.emplace_back(m_loops.update.get(), &m_relative_update_timer);
  }
  if (m_mode->render.thread_id == tid) {
    m_relative_render_timer += m_mode->render.relative_frequency;
    loops.emplace_back(m_loops.render.get(), &m_relative_render_timer);
  }
  if (m_mode->audio.thread_id == tid) {
    m_relative_audio_timer += m_mode->audio.relative_frequency;
    loops.emplace_back(m_loops.audio.get(), &m_relative_audio_timer);
  }

  for (auto [loop, timer] : loops) {
    while (*timer >= 1) {
      *timer -= 1;
      loop->run_single_loop(loops.size() == 1);
    }
  }

  if (m_mode.has_value()) {
    sync.sync_to(m_mode->executor_thread_frequencies.at(tid));
  }
}

auto executor::run_in_thread(executor_thread_id tid,
                             const executor_thread::work& func) -> bool
{
  auto* thread_opt = get_thread_optional(tid);
  if (thread_opt == nullptr) {
    // TODO: this will fail when call run_in_thread() from other threads
    func();
    return true;
  }

  if (thread_opt->has_value()) {
    thread_opt->value().run_in_thread(func);
    return true;
  }

  return false;
}

auto executor::run_in_thread_and_block(executor_thread_id tid,
                                       const executor_thread::work& func)
    -> bool
{
  auto* thread_opt = get_thread_optional(tid);
  if (thread_opt == nullptr) {
    // TODO: this will fail when call run_in_thread() from other threads
    func();
    return true;
  }

  if (thread_opt->has_value()) {
    thread_opt->value().run_in_thread_and_block(func);
    return true;
  }

  return false;
}

}  // namespace ameko
