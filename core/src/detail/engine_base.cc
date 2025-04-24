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
#include "webview/detail/engine_js.hh"
#include "webview/detail/engine_queue.hh"
#include "webview/detail/json.hh"

#include <functional>
#include <string>

namespace webview {
/* **************************************************************
 * `engine_base` PUBLIC implementations
 ****************************************************************/
namespace detail {
engine_base::engine_base(bool owns_window)
    : m_owns_window{owns_window}, q(new engine_queue(this)) {}

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
  skip_queue = true;
  auto res = bind(name, wrapper, nullptr);
  skip_queue = false;
  return res;
}
noresult engine_base::bind(str_arg_t name, binding_t fn, void *arg) {
  trace.base.bind.start(name.c_str());
  do_work_t do_work = [this, name, fn, arg] {
    trace.base.bind.work(name.c_str());
    bindings.emplace(name, binding_ctx_t(fn, arg));
    replace_bind_script();
    skip_queue = true;
    eval(engine_js::onbind(name));
    skip_queue = false;
  };
  auto const is_error = bindings.count(name) > 0;
  if (is_error) {
    return error_info{WEBVIEW_ERROR_DUPLICATE};
  }
  q->promises.list_init(name);
  if (!skip_queue) {
    return q->bind.enqueue(do_work, name);
  }
  do_work();
  return {};
}
noresult engine_base::unbind(str_arg_t name) {
  trace.base.unbind.start(name.c_str());
  do_work_t do_work = [this, name]() {
    trace.base.unbind.work(name.c_str());
    bindings.erase(name);
    replace_bind_script();
    skip_queue = true;
    eval(engine_js::onunbind(name));
    skip_queue = false;
  };

  auto const is_error =
      bindings.count(name) == 0 && !q->unbind.awaits_bind(name);
  if (is_error) {
    return error_info{WEBVIEW_ERROR_NOT_FOUND};
  }
  return q->unbind.enqueue(do_work, name);
}
noresult engine_base::resolve(str_arg_t id, int status, str_arg_t result) {
  q->promises.erase(id);
  str_arg_t escaped_result = result.empty() ? "undefined" : json_escape(result);
  str_arg_t promised_js = engine_js::onreply(id, status, escaped_result);

  return dispatch([this, promised_js] {
    skip_queue = true;
    eval(promised_js);
    skip_queue = false;
  });
}
noresult engine_base::reject(str_arg_t id, str_arg_t err) {
  return resolve(id, 1, json_escape(err));
}
noresult engine_base::run() { return run_impl(); }
noresult engine_base::terminate() {
  q->terminate();
  return terminate_impl();
}
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
  add_user_script(js);
  return {};
}
noresult engine_base::eval(str_arg_t js) {
  trace.base.eval.start(js.c_str(), skip_queue);
  auto skip = skip_queue;
  do_work_t do_work = [this, js, skip] {
    trace.base.eval.work(js.c_str());
    eval_impl(js);
    if (!skip) {
      dispatch([this] { q->eval.set_done(true); });
    }
  };

  if (!skip_queue) {
    //q->register_unresolved_binds(js);
    return q->eval.enqueue(do_work);
  }
  do_work();
  return {};
}
} // namespace detail

/* **************************************************************
 * `engine_base` PROTECTED implementations
 ****************************************************************/
namespace detail {
user_script *engine_base::add_user_script(str_arg_t js) {
  return std::addressof(
      *m_user_scripts.emplace(m_user_scripts.end(), add_user_script_impl(js)));
}

user_script *engine_base::replace_user_script(const user_script &old_script,
                                              str_arg_t new_script_code) {
  remove_all_user_scripts_impl(m_user_scripts);
  user_script *old_script_ptr{};
  for (auto &script : m_user_scripts) {
    auto is_old_script = are_user_scripts_equal_impl(script, old_script);
    script = add_user_script_impl(is_old_script ? new_script_code
                                                : script.get_code());
    if (is_old_script) {
      old_script_ptr = std::addressof(script);
    }
  }
  return old_script_ptr;
}
void engine_base::replace_bind_script() {
  auto replacement_js = create_bind_script();
  if (m_bind_script) {
    m_bind_script = replace_user_script(*m_bind_script, replacement_js);
  } else {
    m_bind_script = add_user_script(replacement_js);
  }
}
void engine_base::on_message(str_arg_t msg) {
  auto name = json_parse(msg, "method", 0);
  auto id = json_parse(msg, "id", 0);
  if (id == "sysop") {
    q->notify(name);
    return;
  }
  auto found = bindings.find(name);
  auto const is_unbound = found == bindings.end();
  if (is_unbound) {
    std::string err = "Promise " + id + " was rejected because binding `" +
                      name + "` got unbound.";
    reject(id, err);
    return;
  }
  q->promises.flag_bind(name);
  q->promises.add(name, id);
  binding_ctx_t ctx = found->second;
  q->resolve(&ctx, name, id, msg);
  found = bindings.end();
}
} // namespace detail

/* **************************************************************
 * `engine_base` PRIVATE implementations
 ****************************************************************/
namespace detail {

/*
void engine_base::bind_thread_constructor() {
  std::mutex mtx;
  std::unique_lock<std::mutex> lock(mtx);

  while (true) {
    bind_cv.wait(lock, [this] { return bind_busy.load(); });
    if (is_terminating.load()) {
      break;
    }

    q->api().bind.dispatch(true);
    list_t<bind_q_ctx_t> active_queue = bind_queue;
    for (auto &ctx : active_queue) {
      // bind may be re-binding unbind name, so defer for it's queue to drain without locking the process.
      if (!list_contains(unbind_queue, ctx.name)) {
        list_erase(bind_queue, ctx.name);
        bind(ctx.name, ctx.fn, ctx.arg);
      } else {
        bind_busy.store(false);
        // eval may be waiting for bind to complete
        eval_cv.notify_one();
        bind_busy.store(true);
      }
    }
    if (bind_queue.empty()) {
      //bind_queue.shrink_to_fit();
      dispatch_f.bind.store(false);
      bind_busy.store(false);
      // eval may be waiting for bind to complete
      eval_cv.notify_one();
    }
  }
}
void engine_base::eval_thread_constructor() {
  std::mutex mtx;
  std::unique_lock<std::mutex> lock(mtx);

  while (true) {
    eval_cv.wait(lock, [this] { return eval_busy.load(); });
    if (is_terminating.load()) {
      break;
    }
    // eval may contain a call to a bind, so wait for it's queue to drain
    eval_cv.wait(lock, [this] { return !bind_busy.load(); });

    dispatch_f.eval.store(true);
    list_t<> active_queue = eval_queue;
    for (auto &js : active_queue) {
      list_erase(eval_queue, js);
      eval(js);
    }
    if (eval_queue.empty()) {
      //eval_queue.shrink_to_fit();
      dispatch_f.eval.store(false);
      eval_busy.store(false);
      // unbind may be waiting for eval to complete
      unbind_cv.notify_one();
    }
  }
}
void engine_base::unbind_thread_constructor() {
  std::mutex mtx;
  std::unique_lock<std::mutex> lock(mtx);

  const auto timeout = std::chrono::milliseconds(WEBVIEW_UNBIND_TIMEOUT);
  const auto get_rej_message = [&](std::string name) {
    std::string message =
        "The native callback function `" + name + "` timed out after " +
        std::to_string(WEBVIEW_UNBIND_TIMEOUT) +
        "ms while unbinding.\nYou can adjust the default timeout when "
        "compiling your application:\nWEBVIEW_UNBIND_TIMEOUT=`int value`";
    return message;
  };

  while (true) {
    unbind_cv.wait(lock, [this] { return unbind_busy.load(); });
    if (is_terminating.load()) {
      break;
    }
    // eval may have called a bound function, so we we wait for it's queue to drain
    //unbind_cv.wait(lock, [this] { return !eval_busy.load(); });

    dispatch_f.unbind.store(true);
    list_t<std::string> active_queue = unbind_queue;
    for (auto &name : active_queue) {
      printf("Webiew: unbind thread: %s\n", name.c_str());

      list_erase(unbind_queue, name);
      // The bound function has no unresolved promises, so unbind it immediately
      if (!list_contains(has_unres_promises, name)) {
        unbind(name);
        return;
      }
      // The backend has not yet messaged the native code, so give it a sane amount of time to do so.
      if (promise_ids[name].empty()) {
        unbind_cv.wait_for(lock, timeout,
                           [&] { return !promise_ids[name].empty(); });
      }
      if (!promise_ids[name].empty()) {
        // Get the latest promise to be sent to the bound function
        auto latest_promise_id = promise_ids[name].back();
        // Give the latest promise a sane amount of time to resolve
        resolver_cv.wait_for(lock, timeout, [this, latest_promise_id] {
          return is_promise_resolved[latest_promise_id].load();
        });
        // The function is now going to unbind, so reject all it's remaining unresolved promises
        for (auto &id : promise_ids[name]) {
          auto found = is_promise_resolved.find(id);
          auto is_resolved = found->second.load();
          if (!is_resolved) {
            resolve(id, 1, json_escape(get_rej_message(name)));
          }
        }
        promise_ids.erase(name);
      }

      list_erase(has_unres_promises, name);
      unbind(name);
    }

    if (unbind_queue.empty()) {
      //unbind_queue.shrink_to_fit();
      dispatch_f.unbind.store(false);
      unbind_busy.store(false);
    }
  }
}
*/

} // namespace detail
} // namespace webview
#endif // defined(__cplusplus) && !defined(WEBVIEW_HEADER)
#endif // WEBVIEW_DETAIL_ENGINE_BASE_CC
