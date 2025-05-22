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

#include "webview/detail/frontend/frontend_strings.hh"
#include "webview/types/types.hh"
#if defined(__cplusplus) && !defined(WEBVIEW_HEADER)
#include "webview/detail/engine_base.hh"
#include "webview/detail/frontend/engine_frontend.hh"
#include "webview/lib/json.hh"
#include "webview/log/trace_log.hh"

using namespace webview::test;
using namespace webview::strings;
using namespace webview::log;
using namespace webview::detail::frontend;

engine_base::engine_base(bool owns_window)
    : tester_t{this}, m_owns_window{owns_window} {}

noresult engine_base::navigate(str_arg_t url) {
  if (url.empty()) {
    return navigate_impl("about:blank");
  }
  return navigate_impl(url);
}

noresult engine_base::bind(str_arg_t name, sync_binding_t fn) {
  auto wrapper = [this, fn](str_arg_t id, str_arg_t req, void * /*arg*/) {
    resolve(id, 0, fn(req));
  };
  auto res = bind(name, wrapper, nullptr, true);
  return res;
}

noresult engine_base::bind(str_arg_t name, binding_t fn, void *arg,
                           bool skip_queue) {
  trace::base.bind.start(name);
  dispatch_fn_t do_work = [=] {
    trace::base.bind.work(name);
    list.bindings.emplace(name, fn, arg);
    replace_bind_script();
    eval(front_end.js.onbind(name), true);
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

noresult engine_base::unbind(str_arg_t name, bool skip_queue) {
  trace::base.unbind.start(name);
  dispatch_fn_t do_work = [=]() {
    trace::base.unbind.work(name);
    eval(front_end.js.onunbind(name), true);
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

noresult engine_base::resolve(str_arg_t id, int status, str_arg_t result) {
  // Firstly notify the queue that the promise is resolving.
  std::string name = list.id_name_map.get(id);
  queue.promises.resolving(name, id);
  list.id_name_map.erase(id);

  dispatch_fn_t do_work = [=] {
    auto res = result.empty() ? "undefined" : json_escape(result);
    auto js = front_end.js.onreply(id, status, res);
    const char *escaped_js = js.c_str();
    eval(escaped_js, true);
  };
  return dispatch(do_work);
}

noresult engine_base::reject(str_arg_t id, str_arg_t err) {
  return resolve(id, 1, json_escape(err));
}

result<void *> engine_base::window() { return window_impl(); }

result<void *> engine_base::widget() { return widget_impl(); }

result<void *> engine_base::browser_controller() {
  return browser_controller_impl();
}

noresult engine_base::run() { return run_impl(); }

noresult engine_base::terminate() { return terminate_impl(); }

noresult engine_base::dispatch(std::function<void()> f) {
  return dispatch_impl(f);
}

noresult engine_base::set_title(str_arg_t title) {
  return set_title_impl(title);
}

noresult engine_base::set_size(int width, int height, webview_hint_t hints) {
  auto res = set_size_impl(width, height, hints);
  m_is_size_set = true;
  return res;
}

noresult engine_base::set_html(str_arg_t html) { return set_html_impl(html); }

noresult engine_base::init(str_arg_t js) {
  list.m_user_scripts.add(js, this);
  return {};
}

noresult engine_base::eval(str_arg_t js, bool skip_queue) {
  trace::base.eval.start(js, skip_queue);
  if (!skip_queue) {
    dispatch_fn_t do_work = [=] {
      auto wrapped_js = front_end.js.eval_wrapper(js);
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

void engine_base::add_init_script(str_arg_t post_fn) {
  auto init_js = front_end.js.init(post_fn);
  list.m_user_scripts.add(init_js, this);
  m_is_init_script_sent = true;
}

std::string engine_base::create_bind_script() {
  std::vector<std::string> bound_names;
  list.bindings.get_names(bound_names);
  return front_end.js.bind(bound_names);
}

void engine_base::on_message(str_arg_t msg) {
  auto id = json_parse(msg, "id", 0);
  if (id == sys_flag.testop) {
    auto test_value = json_parse(msg, "method", 0);
    tester.set_value(test_value);
    return;
  }
  auto name = json_parse(msg, "method", 0);
  if (queue.promises.is_system_message(id, name)) {
    return;
  }
  if (!list.bindings.has_name(name)) {
    auto message = front_end.err_message.reject_unbound(id, name);
    reject(id, message);
    return;
  }
  auto args = json_parse(msg, "params", 0);
  queue.promises.resolve(name, id, args, this);
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
