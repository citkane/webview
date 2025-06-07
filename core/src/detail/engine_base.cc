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
#include "webview/detail/threading/thread_detector.hh"
#include "webview/log/console_log.hh"
#include "webview/log/trace_log.hh"
#include "webview/strings/json.hh"
#include "webview/strings/string_api.hh"

using namespace webview::detail;
using namespace webview::detail::user;
using namespace webview::log;
using namespace webview::strings;
using namespace webview::detail::threading;

/* PUBLIC 
 * ∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇ */

/* Constructor
 * ↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓ */

engine_base::engine_base(bool owns_window)
    : engine_queue{this}, m_owns_window{owns_window} {
  if (!thread::is_main_thread()) {
    throw exception{WEBVIEW_ERROR_INVALID_STATE,
                    "Webview must be created from the main thread."};
  };
}

/* API Methods
 * ↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓ */

noresult engine_base::navigate(cnst_str_r url) {
  auto do_work = [this, url] {
    if (url.empty()) {
      navigate_impl("about:blank");
    } else {
      navigate_impl(url);
    }
  };
  if (thread::is_main_thread()) {
    do_work();
  } else {
    dispatch_(do_work);
  }
  return {};
}
noresult engine_base::bind(cnst_str_r name, sync_binding_t fn) {
  console.warn(
      "Synchronous bind is deprecated and may lead to undefined behaviour");

  auto wrapper = [this, fn](cnst_str_r id, cnst_str_r req, void * /*arg*/) {
    resolve(id, 0, fn(req));
  };
  auto do_work = [this, name, wrapper] { bind(name, wrapper, nullptr); };
  if (thread::is_main_thread()) {
    do_work();
  } else {
    dispatch_(do_work);
  }
  return {};
}
noresult engine_base::bind(cnst_str_r name, binding_t fn, void *arg) {
  trace::base.bind.start(name);
  if (queue.bind.is_duplicate(name)) {
    return error_info{WEBVIEW_ERROR_DUPLICATE};
  }

  auto do_work = [this, name, fn, arg] {
    trace::base.bind.work(name);
    list.bindings.emplace(name, fn, arg);
    replace_bind_script();
    eval(string::js.onbind(name), true);
  };
  // The user may want to bind before running so that they can use
  // bindings in `webview_init` or `webview_set_html`.
  // In this scenario, we execute `bind` directly.
  if (thread::is_main_thread() && !atomic.dom.ready()) {
    do_work();
    return {};
  }
  return queue.bind.enqueue(do_work, name);
}
noresult engine_base::unbind(cnst_str_r name) {
  trace::base.unbind.start(name);
  if (queue.unbind.not_found(name)) {
    return error_info{WEBVIEW_ERROR_NOT_FOUND};
  }

  auto do_work = [this, name]() {
    trace::base.unbind.work(name);
    eval(string::js.onunbind(name), true);
    list.bindings.erase(name);
    replace_bind_script();
  };
  return queue.unbind.enqueue(do_work, name);
}
noresult engine_base::eval(cnst_str_r js, bool skip_queue) {
  trace::base.eval.start(js, skip_queue);
  auto do_work = [this, js, skip_queue] {
    if (!skip_queue) {
      auto wrapped_js = string::js.eval_wrapper(js);
      trace::base.eval.work(wrapped_js, skip_queue);
      if (thread::is_main_thread()) {
        eval_impl(wrapped_js);
      } else {
        dispatch_([this, wrapped_js] { eval_impl(wrapped_js); });
      }
    } else {
      trace::base.eval.work(js, skip_queue);
      if (thread::is_main_thread()) {
        eval_impl(js);
      } else {
        dispatch_([this, js] { eval_impl(js); });
      }
    }
  };
  if (!skip_queue) {
    return queue.eval.enqueue(do_work, js);
  }
  do_work();
  return {};
}
template <typename T>
typename std::enable_if<std::is_arithmetic<T>::value &&
                            !std::is_same<T, bool>::value,
                        noresult>::type
engine_base::resolve(cnst_str_r id, int status, T result) {
  return resolve(id, status, std::to_string(result));
};
template <typename T>
typename std::enable_if<std::is_same<T, bool>::value, noresult>::type
engine_base::resolve(cnst_str_r id, int status, T result) {
  std::string bool_string = result ? "true" : "false";
  return resolve(id, status, bool_string);
};

noresult engine_base::resolve(cnst_str_r id, int status, cnst_str_r result) {
  // Get the promise binding name and
  // notify the queue that the promise is resolving.
  std::string name = list.id_name_map.get(id);
  queue.promises.resolving(name, id);
  list.id_name_map.erase(id);

  auto res_m = result.empty() ? "undefined" : result;
  auto action = status == 0 ? "resolving" : "rejecting";
  auto message = "Bound function \"" + name + "\" is " + action + " promise " +
                 id + " with result: " + res_m;
  status == 0 ? console.info(message) : console.warn(message);

  auto res_escaped = result.empty() ? "undefined" : json.escape(result);

  trace::base.eval.print_here(res_escaped);

  auto js = string::js.onreply(id, status, res_escaped);
  return eval(js, true);
}
noresult engine_base::reject(cnst_str_r id, cnst_str_r err) {
  return resolve(id, 1, err);
}
result<void *> engine_base::window() { return window_impl(); }
result<void *> engine_base::widget() { return widget_impl(); }
result<void *> engine_base::browser_controller() {
  return browser_controller_impl();
}
noresult engine_base::run() {
  if (!thread::is_main_thread()) {
    throw exception{WEBVIEW_ERROR_INVALID_STATE,
                    "Webview must be run from the main thread."};
  }

  return run_impl();
}
noresult engine_base::terminate() {
  // terminate_impl should normally be called from a child thread,
  // so we always dispatch it to the main thread.
  return dispatch_([this] {
    queue.terminate();
    terminate_impl();
  });
}

IGNORE_DEPRECATED_DECLARATIONS
noresult engine_base::dispatch(std::function<void()> f) { return dispatch_(f); }
RESTORE_IGNORED_WARNINGS

noresult engine_base::set_title(cnst_str_r title) {
  auto do_work = [this, title] { set_title_impl(title); };
  if (thread::is_main_thread()) {
    do_work();
  } else {
    dispatch_(do_work);
  }
  return {};
}
noresult engine_base::set_size(int width, int height, webview_hint_t hints) {
  auto do_work = [this, width, height, hints] {
    set_size_impl(width, height, hints);
    m_is_size_set = true;
  };
  if (thread::is_main_thread()) {
    do_work();
  } else {
    dispatch_(do_work);
  }
  return {};
}
noresult engine_base::set_html(cnst_str_r html) {
  auto do_work = [this, html] { set_html_impl(html); };
  if (thread::is_main_thread()) {
    do_work();
  } else {
    dispatch_(do_work);
  }
  return {};
}
noresult engine_base::init(cnst_str_r js) {
  if (!thread::is_main_thread()) {
    throw exception{WEBVIEW_ERROR_INVALID_STATE,
                    "Webview init must be called from the main thread."};
  }
  list.m_user_scripts.add(js, this);
  return {};
}

/* ∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆
 * PUBLIC
 * ----------------------------------------------------------------------------------------------------------- 
 * PROTECTED 
 * ∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇ */

noresult engine_base::dispatch_(std::function<void()> f) {
  return dispatch_impl(f);
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
void engine_base::add_init_script(cnst_str_r post_fn) {
  auto init_js = string::js.init(post_fn);
  list.m_user_scripts.add(init_js, this);
  m_is_init_script_sent = true;
}
std::string engine_base::create_bind_script() {
  std::vector<std::string> bound_names;
  list.bindings.get_names(bound_names);
  return string::js.bind(bound_names);
}
void engine_base::on_message(cnst_str_r msg) {
  auto id = json.parse(msg, "id", 0);
  auto name = json.parse(msg, "method", 0);
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
  auto args = json.parse(msg, "params", 0);
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
  dispatch_([&] { done = true; });
  run_event_loop_while([&] { return !done; });
}
void engine_base::dispatch_size_default() {
  if (!owns_window() || !m_is_init_script_sent) {
    return;
  };
  dispatch_([this]() {
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

/* ∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆
 * PROTECTED
 * ----------------------------------------------------------------------------------------------------------- 
 * PRIVATE
 * ∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇ */

unsigned int engine_base::inc_window_count() { return ++window_ref_count(); }
unsigned int engine_base::dec_window_count() {
  auto &count = window_ref_count();
  if (count > 0) {
    return --count;
  }
  return 0;
}

/* ∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆
 * PRIVATE */

#endif // defined(__cplusplus) && !defined(WEBVIEW_HEADER)
#endif // WEBVIEW_DETAIL_ENGINE_BASE_CC
