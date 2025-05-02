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
#include "webview/detail/engine_frontend.hh"
#include <cstdio>

namespace webview {
detail::engine_queue::engine_queue() : queue{this}, flags{this} {};

// PUBLIC API implementation
namespace detail {

using public_api_t = engine_queue::public_api_t;
using bind_api_t = public_api_t::bind_api_t;
using unbind_api_t = public_api_t::unbind_api_t;
using promise_api_t = public_api_t::promise_api_t;
using eval_api_t = public_api_t::eval_api_t;

public_api_t::public_api_t(engine_queue *self)
    : api_base(self), bind(self), unbind{self}, eval{self}, promises{self} {};

noresult bind_api_t::enqueue(do_work_t fn, str_arg_t name) const {
  return self->queue_work(name, fn, self->ctx.bind);
};

noresult unbind_api_t::enqueue(do_work_t fn, str_arg_t name) const {
  return self->queue_work(name, fn, self->ctx.unbind);
};
bool unbind_api_t::awaits_bind(str_arg_t name) const {
  return self->unbind_awaiting_bind(name);
};

noresult eval_api_t::enqueue(do_work_t fn, str_arg_t js) const {
  return self->queue_work(js, fn, self->ctx.eval);
};

void promise_api_t::list_init(str_arg_t name) const {
  self->promise_list_init(name);
};
void promise_api_t::resolved(str_arg_t id) const { self->promise_erase(id); };
void promise_api_t::resolve(engine_base *wv, str_arg_t name, str_arg_t id,
                            str_arg_t args) const {

  self->list.promise_id_name[id] = name;
  self->list.name_unres_promises[name].push_back(id);

  std::thread resolver = std::thread(&engine_queue::resolve_thread_constructor,
                                     self, wv, name, id, args);
  resolver.detach();
}
bool promise_api_t::is_system_message(str_arg_t id, str_arg_t method) {
  if (id != SYSTEM_NOTIFICATION_FLAG) {
    return false;
  };
  if (method == DOM_READY_M) {
    self->trace.queue.notify.on_message(method);
    self->flags.set_dom_ready();
  }
  if (method == BIND_DONE_M) {
    self->trace.queue.notify.on_message(method);
    self->flags.done.bind(true);
  }
  if (method == UNBIND_DONE_M) {
    self->trace.queue.notify.on_message(method);
    self->flags.done.unbind(true);
  }
  if (method == EVAL_READY_M) {
    self->trace.queue.notify.on_message(method);
    self->flags.done.eval(true);
  }
  return true;
}

void public_api_t::init_queue(engine_base &wv) {
  if (self->queue_thread_is_constructed) {
    return;
  };
  self->queue_thread =
      std::thread(&engine_queue::queue_thread_constructor, self, &wv);
  self->queue_thread_is_constructed = true;
}

void public_api_t::terminate_queue() {
  std::mutex mtx;
  std::unique_lock<std::mutex> lock(mtx);

  self->flags.set_terminating();
  self->cv.queue.wait(lock, [this] { return self->queue_thread.joinable(); });
  self->queue_thread.join();
}

} // namespace detail

namespace detail {

void engine_queue::queue_thread_constructor(engine_base *wv) {
  std::mutex mtx;
  std::unique_lock<std::mutex> lock(mtx);

  while (true) {
    trace.queue.loop.wait(list.queue.size(), flags.queue_empty(),
                          flags.get_dom_ready());
    cv.queue.wait(
        lock, [this] { return flags.get_dom_ready() && !flags.queue_empty(); });
    if (flags.get_terminating()) {
      break;
    }
    trace.queue.loop.start(list.queue.size());
    auto work = &list.queue.front();
    auto val = work->val;
    auto fn = work->fn;

    // `bind` user work unit
    if (work->ctx == ctx.bind) {
      trace.queue.bind.start(val);
      wv->dispatch(fn);
      cv.bind.wait(lock, [this] { return flags.done.bind(); });
      list.pending_binds.pop_front();
      trace.queue.bind.done(flags.done.bind(), val);
      flags.done.bind(false);
    }
    // `unbind` user work unit
    if (work->ctx == ctx.unbind) {
      trace.queue.unbind.wait(val);
      cv.unbind_timeout.wait_for(
          lock, std::chrono::milliseconds(WEBVIEW_UNBIND_TIMEOUT), [this, val] {
            return flags.get_terminating() ||
                   list.name_unres_promises[val].empty();
          });
      auto promises = list.name_unres_promises[val];
      for (auto &id : promises) {
        wv->reject(id, utility::frontend.err_message.reject_unbound(id, val));
      }
      list.name_unres_promises.erase(val);
      trace.queue.unbind.start(val);
      wv->dispatch(fn);
      cv.unbind.wait(lock, [this] { return flags.done.unbind(); });
      trace.queue.unbind.done(flags.done.unbind(), val);
      flags.done.unbind(false);
    }
    // `eval` user work unit
    if (work->ctx == ctx.eval) {
      trace.queue.eval.start(val);
      wv->dispatch(fn);
      cv.eval.wait(lock, [this] { return flags.done.eval(); });
      trace.queue.eval.done(flags.done.eval());
      flags.done.eval(false);
    }

    flags.update_queue_size();
    trace.queue.loop.end();
  }
}

void engine_queue::resolve_thread_constructor(engine_base *wv, str_arg_t name,
                                              str_arg_t id, str_arg_t args) {
  try {
    wv->bindings.at(name).call(id, args);
  } catch (const std::exception &err) {
    wv->reject(
        id, utility::frontend.err_message.uncaught_exception(name, err.what()));
  } catch (...) {
    perror(utility::frontend.err_message.webview_terminated(name).c_str());
  };
}

noresult engine_queue::queue_work(str_arg_t name_or_js, do_work_t fn,
                                  context_t fn_ctx) {
  if (fn_ctx == ctx.bind) {
    list.pending_binds.push_back(name_or_js);
  }

  trace.queue.enqueue.added(fn_ctx, list.queue.size(), name_or_js);
  list.queue.push_back({fn_ctx, fn, name_or_js});
  flags.queue_empty(false);
  return {};
};

void engine_queue::set_done(bool val, context_t fn_ctx) {
  if (fn_ctx == ctx.bind) {
    bind_done.store(val);
    if (val) {
      cv.bind.notify_one();
    }
  }
  if (fn_ctx == ctx.unbind) {
    unbind_done.store(val);
    if (val) {
      cv.unbind.notify_one();
    }
  }
  if (fn_ctx == ctx.eval) {
    eval_done.store(val);
    if (val) {
      cv.eval.notify_one();
    }
  }
};

bool engine_queue::unbind_awaiting_bind(str_arg_t name) {
  bool found{};
  for (auto &val : list.pending_binds) {
    if (val == name) {
      found = true;
      break;
    }
  }
  return found;
};

void engine_queue::promise_list_init(str_arg_t name) {
  list.name_unres_promises[name] = {};
}

void engine_queue::promise_erase(str_arg_t id) {
  auto name = list.promise_id_name[id];
  list.promise_id_name.erase(list.promise_id_name.at(id));
  list.name_unres_promises[name].remove(id);
  cv.unbind_timeout.notify_one();
  trace.queue.unbind.print_here("Promise erased: " + name + " | " + id);
};

bool engine_queue::done_t::bind() { return self->bind_done.load(); }
void engine_queue::done_t::bind(bool val) {
  self->set_done(val, self->ctx.bind);
}
bool engine_queue::done_t::unbind() { return self->unbind_done.load(); }
void engine_queue::done_t::unbind(bool val) {
  self->set_done(val, self->ctx.unbind);
}
bool engine_queue::done_t::eval() { return self->eval_done.load(); }
void engine_queue::done_t::eval(bool val) {
  self->set_done(val, self->ctx.eval);
}

bool engine_queue::flags_api_t::get_dom_ready() const {
  return self->is_dom_ready.load();
};
void engine_queue::flags_api_t::set_dom_ready() const {
  self->is_dom_ready.store(true);
  self->cv.queue.notify_one();
};
bool engine_queue::flags_api_t::get_terminating() const {
  return self->is_terminating.load();
};
void engine_queue::flags_api_t::set_terminating() const {
  self->is_terminating.store(true);
  self->queue_empty.store(false);
  self->is_dom_ready.store(true);
  self->flags.done.bind(true);
  self->flags.done.unbind(true);
  self->flags.done.eval(true);
  self->cv.queue.notify_one();
  self->cv.unbind_timeout.notify_one();
};
void engine_queue::flags_api_t::update_queue_size() const {
  if (self->list.queue.size() > 1) {
    self->list.queue.pop_front();
  } else {
    self->list.queue.clear();
  }
  queue_empty(self->list.queue.empty());
}
bool engine_queue::flags_api_t::queue_empty() const {
  return self->queue_empty.load();
};
void engine_queue::flags_api_t::queue_empty(bool val) const {
  self->queue_empty.store(val);
  self->cv.queue.notify_one();
}

} // namespace detail
} // namespace webview

#endif // defined(__cplusplus) && !defined(WEBVIEW_HEADER)
#endif // WEBVIEW_ENGINE_QUEUE_CC
