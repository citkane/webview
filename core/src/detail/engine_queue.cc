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
#include "webview/cc_api.hh"
#include "webview/log/trace_log.hh"
#include "webview/strings/string_api.hh"
#include <cstdio>

using namespace webview::detail;
using namespace webview::strings;
using namespace webview::log;
using namespace webview::types;

using public_api_t = engine_queue::public_api_t;
using bind_api_t = engine_queue::bind_api_t;
using unbind_api_t = engine_queue::unbind_api_t;
using promise_api_t = engine_queue::promise_api_t;
using eval_api_t = engine_queue::eval_api_t;

engine_queue::engine_queue(engine_base *wv)
    : queue{this}, atomic{this}, wv(wv) {
  queue_thread = std::thread(&engine_queue::queue_thread_constructor, this);
}

engine_queue::~engine_queue() { queue_thread.join(); }

void engine_queue::terminate_queue() {
  printf("terminate_queue\n");
  is_terminating.store(true);
  cv.notify_all();
  printf("Notified, is joinable:%s\n",
         (queue_thread.joinable() ? "true" : "false"));
  if (!queue_thread.joinable()) {
    perror("Thread not joinable");
    throw std::exception();
  }
};

bool engine_queue::will_be_bound(const_str_ref name) const {
  auto i = list.pending.indices(name);
  auto is_bound = list.bindings.count(name) > 0;
  if (is_bound) {
    auto will_be_unbound = i.unbind_i > -1 && i.unbind_i > i.bind_i;
    return !will_be_unbound;
  } else {
    auto will_be_bound = i.bind_i > -1 && i.bind_i > i.unbind_i;
    return will_be_bound;
  };
};

noresult bind_api_t::enqueue(dispatch_fn_t fn, const_str_ref name) {
  return self->queue_work(name, fn, self->ctx.bind);
};
bool bind_api_t::is_duplicate(const_str_ref name) const {
  return self->will_be_bound(name);
};

bool unbind_api_t::not_found(const_str_ref name) const {
  return !self->will_be_bound(name);
};
noresult unbind_api_t::enqueue(dispatch_fn_t fn, const_str_ref name) {
  return self->queue_work(name, fn, self->ctx.unbind);
};

noresult eval_api_t::enqueue(dispatch_fn_t fn, const_str_ref js) {
  return self->queue_work(js, fn, self->ctx.eval);
};

void promise_api_t::resolving(const_str_ref name, const_str_ref id) {
  self->list.unresolved_promises.remove_id(name, id);
  if (self->list.unresolved_promises.empty(name)) {
    self->cv.unbind_timeout.notify_one();
    self->list.unresolved_promises.erase(name);
  }
};
void promise_api_t::resolve(const_str_ref name, const_str_ref id,
                            const_str_ref args) {
  self->list.id_name_map.set(id, name);
  self->list.unresolved_promises.add_id(name, id);
  self->cv.unbind_timeout.notify_one();
  // Send the user defined native callback work to a detached thread.
  // @todo Thread pooling and resource management.
  std::thread resolver = std::thread(&engine_queue::resolve_thread_constructor,
                                     self, name, id, args);
  resolver.detach();
}
bool promise_api_t::exec_system_message(const_str_ref id,
                                        const_str_ref method) {
  if (id != sys_flags.sysop) {
    return false;
  };
  if (method == sys_ops.dom_ready) {
    trace::queue.notify.on_message(method);
    self->atomic.dom.ready(true);
  }
  if (method == sys_ops.bind_done) {
    trace::queue.notify.on_message(method);
    self->atomic.done.bind(true);
  }
  if (method == sys_ops.unbind_done) {
    trace::queue.notify.on_message(method);
    self->atomic.done.unbind(true);
  }
  if (method == sys_ops.js_eval_start) {
    trace::queue.notify.on_message(method);
    self->atomic.done.eval(true);
  }
  return true;
}

noresult engine_queue::queue_work(const_str_ref name_or_js, dispatch_fn_t fn,
                                  context_t fn_ctx) {
  const auto &name = name_or_js;
  if (fn_ctx == ctx.bind) {
    list.pending.push_back("bind-" + name);
  }
  if (fn_ctx == ctx.unbind) {
    list.pending.push_back("unbind-" + name);
  }
  list.queue.push_back(fn_ctx, fn, name_or_js);
  trace::queue.enqueue.added(char(fn_ctx), list.queue.size(), name_or_js);
  cv.queue.notify_one();
  return {};
};

#endif // defined(__cplusplus) && !defined(WEBVIEW_HEADER)
#endif // WEBVIEW_ENGINE_QUEUE_CC
