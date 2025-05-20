/*
 * MIT License
 *
 * Copyright (c) 2022 Steffen André Langnes
 * Copyright (c) 2025 Michael Jonker 
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

#ifndef WEBVIEW_ENGINE_QUEUE_CC
#define WEBVIEW_ENGINE_QUEUE_CC

#if defined(__cplusplus) && !defined(WEBVIEW_HEADER)
#include "webview/detail/engine_queue.hh"
#include "webview/detail/engine_base.hh"
#include "webview/detail/frontend/engine_frontend.hh"
#include <cstdio>
#include <mutex>

namespace webview {
detail::engine_queue::engine_queue() : queue{this}, atomic{this} {}

// PUBLIC API implementation
namespace detail {

using public_api_t = engine_queue::public_api_t;
using bind_api_t = engine_queue::bind_api_t;
using unbind_api_t = engine_queue::unbind_api_t;
using promise_api_t = engine_queue::promise_api_t;
using eval_api_t = engine_queue::eval_api_t;

noresult bind_api_t::enqueue(dispatch_fn_t fn, str_arg_t name) const {
  return self->queue_work(name, fn, self->ctx.bind);
};
bool bind_api_t::is_duplicate(str_arg_t name) const {
  auto i = self->list.pending.indices(name);
  auto will_be_bound = i.bind_i > -1 && i.bind_i > i.unbind_i;
  return self->list.bindings.count(name) > 0 || will_be_bound;
};

noresult unbind_api_t::enqueue(dispatch_fn_t fn, str_arg_t name) const {
  return self->queue_work(name, fn, self->ctx.unbind);
};
bool unbind_api_t::not_found(str_arg_t name) const {
  auto i = self->list.pending.indices(name);
  auto will_be_bound = i.bind_i > -1 && i.bind_i > i.unbind_i;
  return self->list.bindings.count(name) == 0 && !will_be_bound;
};

noresult eval_api_t::enqueue(dispatch_fn_t fn, str_arg_t js) const {
  return self->queue_work(js, fn, self->ctx.eval);
};

void promise_api_t::resolved(str_arg_t id) const {
  auto name = self->list.id_name_map.get(id);
  if (name.empty()) {
    return;
  };
  self->list.id_name_map.erase(id);
  self->list.unresolved_promises.remove_id(name, id);
  if (self->list.unresolved_promises.empty(name)) {
    self->cv.unbind_timeout.notify_one();
    self->list.unresolved_promises.erase(name);
  }
};
void promise_api_t::resolve(str_arg_t name, str_arg_t id, str_arg_t args,
                            engine_base *wv) const {
  self->list.id_name_map.set(id, name);
  self->list.unresolved_promises.add_id(name, id);
  self->cv.unbind_timeout.notify_one();
  std::thread resolver = std::thread(&engine_queue::resolve_thread_constructor,
                                     self, name, id, args, wv);
  resolver.detach();
}
bool promise_api_t::is_system_message(str_arg_t id, str_arg_t method) {
  if (id != SYSTEM_NOTIFICATION_FLAG) {
    return false;
  };
  if (method == frontend.sysops.dom_ready) {
    log::trace::queue.notify.on_message(method);
    self->atomic.dom.ready(true);
  }
  if (method == frontend.sysops.bind_done) {
    log::trace::queue.notify.on_message(method);
    self->atomic.done.bind(true);
  }
  if (method == frontend.sysops.unbind_done) {
    log::trace::queue.notify.on_message(method);
    self->atomic.done.unbind(true);
  }
  if (method == frontend.sysops.js_eval_start) {
    log::trace::queue.notify.on_message(method);
    self->atomic.done.eval(true);
  }
  return true;
}

void public_api_t::init(engine_base *wv_instance) const {
  self->queue_thread =
      std::thread(&engine_queue::queue_thread_constructor, self, wv_instance);
}
void public_api_t::shutdown() const {
  self->is_terminating.store(true);
  self->queue_empty.store(false);
  self->is_dom_ready.store(true);
  self->atomic.done.bind(true);
  self->atomic.done.unbind(true);
  self->atomic.done.eval(true);
  self->cv.notify_all();
  self->queue_thread.join();
}
bool public_api_t::shutting_down() const { return self->is_terminating.load(); }

noresult engine_queue::queue_work(str_arg_t name_or_js, dispatch_fn_t fn,
                                  context_t fn_ctx) {
  const auto &name = name_or_js;
  if (fn_ctx == ctx.bind) {
    list.pending.push_back("bind-" + name);
  }
  if (fn_ctx == ctx.unbind) {
    list.pending.push_back("unbind-" + name);
  }
  list.queue.push_back(fn_ctx, fn, name_or_js);
  log::trace::queue.enqueue.added(char(fn_ctx), list.queue.size(), name_or_js);
  //atomic.queue.update();
  cv.queue.notify_one();
  return {};
};

} // namespace detail
} // namespace webview

#endif // defined(__cplusplus) && !defined(WEBVIEW_HEADER)
#endif // WEBVIEW_ENGINE_QUEUE_CC
