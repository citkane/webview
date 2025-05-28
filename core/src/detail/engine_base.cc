/*
 * MIT License
 *
 * Copyright (c) 2017 Serge Zaitsev
 * Copyright (c) 2022 Steffen André Langnes
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#ifndef WEBVIEW_DETAIL_ENGINE_BASE_CC
#define WEBVIEW_DETAIL_ENGINE_BASE_CC

#if defined(__cplusplus) && !defined(WEBVIEW_HEADER)
#include "webview/detail/engine_base.hh"
#include "webview/log/trace_log.hh"
#include "webview/strings/string_api.hh"

using namespace webview::detail;
using namespace webview::detail::user;
using namespace webview::log;
using namespace webview::strings;

engine_base::engine_base(bool owns_window)
    : engine_queue{this}, m_owns_window{owns_window} {}

noresult engine_base::navigate(const_str_ref url) {
  if (url.empty()) {
    return navigate_impl("about:blank");
  }
  return navigate_impl(url);
}

noresult engine_base::bind(const_str_ref name, sync_binding_t fn) {
  auto wrapper = [this, fn](const_str_ref id, const_str_ref req,
                            void * /*arg*/) { resolve(id, 0, fn(req)); };
  auto res = bind(name, wrapper, nullptr, true);
  return res;
}

noresult engine_base::bind(const_str_ref name, binding_t fn, void *arg,
                           bool skip_queue) {
  trace::base.bind.start(name);
  dispatch_fn_t do_work = [this, name, fn, arg] {
    trace::base.bind.work(name);
    list.bindings.emplace(name, fn, arg);
    replace_bind_script();
    eval(string::js.onbind(name), true);
  };
  if (queue.bind.is_duplicate(name)) {
    return error_info{WEBVIEW_ERROR_DUPLICATE};
  }
  if (!skip_queue) {
    return queue.bind.enqueue(do_work, name);
  }
  do_work();
  return {};
}

noresult engine_base::unbind(const_str_ref name, bool skip_queue) {
  trace::base.unbind.start(name);
  dispatch_fn_t do_work = [this, name]() {
    trace::base.unbind.work(name);
    eval(string::js.onunbind(name), true);
    list.bindings.erase(name);
    replace_bind_script();
  };
  if (queue.unbind.not_found(name)) {
    return error_info{WEBVIEW_ERROR_NOT_FOUND};
  }
  if (!skip_queue) {
    return queue.unbind.enqueue(do_work, name);
  }
  do_work();
  return {};
}

noresult engine_base::resolve(const_str_ref id, int status,
                              const_str_ref result) {
  // Firstly notify the queue that the promise is resolving.
  std::string name = list.id_name_map.get(id);
  queue.promises.resolving(name, id);
  list.id_name_map.erase(id);

  dispatch_fn_t do_work = [this, id, status, result] {
    auto res = result.empty() ? "undefined" : string::json.escape(result);
    auto js = string::js.onreply(id, status, res);
    const char *escaped_js = js.c_str();
    eval(escaped_js, true);
  };
  return dispatch(do_work);
}

noresult engine_base::reject(const_str_ref id, const_str_ref err) {
  return resolve(id, 1, string::json.escape(err));
}

result<void *> engine_base::window() { return window_impl(); }

result<void *> engine_base::widget() { return widget_impl(); }

result<void *> engine_base::browser_controller() {
  return browser_controller_impl();
}

noresult engine_base::run() { return run_impl(); }

noresult engine_base::terminate() {
  // terminate_impl would normally be called from a child thread, so we dispatch it to the main thread.
  return dispatch([this] {
    terminate_queue();
    terminate_impl();
  });
}

noresult engine_base::dispatch(std::function<void()> f) {
  return dispatch_impl(f);
}

noresult engine_base::set_title(const_str_ref title) {
  return set_title_impl(title);
}

noresult engine_base::set_size(int width, int height, webview_hint_t hints) {
  auto res = set_size_impl(width, height, hints);
  m_is_size_set = true;
  return res;
}

noresult engine_base::set_html(const_str_ref html) {
  return set_html_impl(html);
}

noresult engine_base::init(const_str_ref js) {
  list.m_user_scripts.add(js, this);
  return {};
}

noresult engine_base::eval(const_str_ref js, bool skip_queue) {
  trace::base.eval.start(js, skip_queue);
  if (!skip_queue) {
    dispatch_fn_t do_work = [this, js] {
      auto wrapped_js = string::js.eval_wrapper(js);
      trace::base.eval.work(wrapped_js);
      eval_impl(wrapped_js);
    };
    return queue.eval.enqueue(do_work, js);
  }
  trace::base.eval.work(js);
  eval_impl(js);
  return {};
}

void engine_base::replace_bind_script() {
  auto replacement_js = create_bind_script();
  if (m_bind_script) {
    m_bind_script =
        list.m_user_scripts.replace(*m_bind_script, replacement_js, this);
  } else {
    m_bind_script = list.m_user_scripts.add(replacement_js, this);
  }
}

void engine_base::add_init_script(const_str_ref post_fn) {
  auto init_js = string::js.init(post_fn);
  list.m_user_scripts.add(init_js, this);
  m_is_init_script_sent = true;
}

std::string engine_base::create_bind_script() {
  std::vector<std::string> bound_names;
  list.bindings.get_names(bound_names);
  return string::js.bind(bound_names);
}

void engine_base::on_message(const_str_ref msg) {
  auto id = string::json.parse(msg, "id", 0);
  auto name = string::json.parse(msg, "method", 0);
  if (id == sys_flags.testop) {
    tester::set_value(name);
    return;
  }
  if (queue.promises.exec_system_message(id, name)) {
    return;
  }
  if (!list.bindings.has_name(name)) {
    auto message = string::err.reject_unbound(id, name);
    reject(id, message);
    return;
  }
  auto args = string::json.parse(msg, "params", 0);
  // Keep the user defined native callback work on the main thread.
  // Used for synchronous testing purposes
  if (tester::resolve_on_main_thread()) {
    dispatch([this, name, id, args] { list.bindings.at(name).call(id, args); });
    return;
  }
  queue.promises.resolve(name, id, args);
}

void engine_base::on_window_created() { inc_window_count(); }

void engine_base::on_window_destroyed(bool skip_termination) {
  if (dec_window_count() <= 0) {
    if (!skip_termination) {
      terminate();
    }
  }
}

void engine_base::deplete_run_loop_event_queue() {
  bool done{};
  dispatch([&] { done = true; });
  run_event_loop_while([&] { return !done; });
}

void engine_base::dispatch_size_default() {
  if (!owns_window() || !m_is_init_script_sent) {
    return;
  };
  dispatch([this]() {
    if (!m_is_size_set) {
      set_size(m_initial_width, m_initial_height, WEBVIEW_HINT_NONE);
    }
  });
}

void engine_base::set_default_size_guard(bool guarded) {
  m_is_size_set = guarded;
}

bool engine_base::owns_window() const { return m_owns_window; }

std::atomic_uint &engine_base::window_ref_count() {
  static std::atomic_uint ref_count{0};
  return ref_count;
}

unsigned int engine_base::inc_window_count() { return ++window_ref_count(); }

unsigned int engine_base::dec_window_count() {
  auto &count = window_ref_count();
  if (count > 0) {
    return --count;
  }
  return 0;
}

#endif // defined(__cplusplus) && !defined(WEBVIEW_HEADER)
#endif // WEBVIEW_DETAIL_ENGINE_BASE_CC
