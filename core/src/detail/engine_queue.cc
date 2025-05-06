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
#include <initializer_list>
#include <mutex>

namespace webview {
detail::engine_queue::engine_queue()
    : user_queue{this}, atomic{this}, wv(nullptr) {}

// PUBLIC API implementation
namespace detail {

using public_api_t = engine_queue::public_api_t;
using bind_api_t = engine_queue::bind_api_t;
using unbind_api_t = engine_queue::unbind_api_t;
using promise_api_t = engine_queue::promise_api_t;
using eval_api_t = engine_queue::eval_api_t;
using bindings_api_t = engine_queue::bindings_api_t;

noresult bind_api_t::enqueue(do_work_t fn, str_arg_t name) const {
  return self->queue_work(name, fn, self->ctx.bind);
};

noresult unbind_api_t::enqueue(do_work_t fn, str_arg_t name) const {
  return self->queue_work(name, fn, self->ctx.unbind);
};
bool unbind_api_t::awaits_bind(str_arg_t name) const {
  bool found_name{};
  for (auto &val : self->lists.pending_binds) {
    if (val == name) {
      found_name = true;
      break;
    }
  }
  return found_name;
};

noresult eval_api_t::enqueue(do_work_t fn, str_arg_t js) const {
  return self->queue_work(js, fn, self->ctx.eval);
};

void promise_api_t::list_init(str_arg_t name) const {
  self->atomic.lists.locked(true);
  self->lists.name_unres_promises[name] = {};
  self->atomic.lists.locked(false);
};

void promise_api_t::resolved(str_arg_t id) const {
  self->atomic.lists.locked(true);
  auto &promise_name_list = self->lists.promise_id_name;
  auto name = promise_name_list[id];
  promise_name_list.erase(promise_name_list.at(id));
  self->lists.name_unres_promises[name].remove(id);
  self->atomic.lists.locked(false);
};
void promise_api_t::resolve(str_arg_t name, str_arg_t id,
                            str_arg_t args) const {
  {
    self->atomic.lists.locked(true);
    self->lists.promise_id_name[id] = name;
    self->lists.name_unres_promises[name].push_back(id);
    self->atomic.lists.locked(false);
  }
  std::thread resolver = std::thread(&engine_queue::resolve_thread_constructor,
                                     self, name, id, args);
  resolver.detach();
}
bool promise_api_t::is_system_message(str_arg_t id, str_arg_t method) {
  if (id != SYSTEM_NOTIFICATION_FLAG) {
    return false;
  };
  if (method == DOM_READY_M) {
    self->trace.queue.notify.on_message(method);
    self->atomic.dom.ready(true);
  }
  if (method == BIND_DONE_M) {
    self->trace.queue.notify.on_message(method);
    self->atomic.done.bind(true);
  }
  if (method == UNBIND_DONE_M) {
    self->trace.queue.notify.on_message(method);
    self->atomic.done.unbind(true);
  }
  if (method == EVAL_READY_M) {
    self->trace.queue.notify.on_message(method);
    self->atomic.done.eval(true);
  }
  return true;
}

void bindings_api_t::locked(bool val) const {
  self->atomic.bindings.locked(val);
}

void public_api_t::init(engine_base *wv_instance) {
  self->wv = wv_instance;
  self->queue_thread =
      std::thread(&engine_queue::queue_thread_constructor, self);
};

void public_api_t::shutdown() {
  //std::mutex main_thread_mtx;
  //std::unique_lock<std::mutex> lock(self->main_thread_mtx);
  //self->atomic.terminate.init();
  //self->cv.queue.wait(lock, [this] { return self->queue_thread.joinable(); });
  self->queue_empty.store(false);
  self->is_dom_ready.store(true);
  self->atomic.done.bind(true);
  self->atomic.done.unbind(true);
  self->atomic.done.eval(true);
  self->atomic.lists.locked(false);
  self->atomic.bindings.locked(false);
  for (auto &this_cv : self->cv.all) {
    this_cv->notify_all();
  }
  self->queue_thread.join();
}

} // namespace detail

// PRIVATE implementation
namespace detail {

void engine_queue::queue_thread_constructor() {
  std::mutex queue_thread_mtx;
  std::unique_lock<std::mutex> lock(queue_thread_mtx);
  while (true) {
    trace.queue.loop.wait(lists.queue.size(), atomic.queue.empty(),
                          atomic.dom.ready());
    cv.queue.wait(lock, [this] {
      return atomic.and_(
          {atomic.dom.ready(), !atomic.queue.empty(), !atomic.lists.locked()});
    });
    if (atomic.terminating()) {
      break;
    }
    trace.queue.loop.start(lists.queue.size());
    auto work = &lists.queue.front();
    auto val = work->val;
    auto fn = work->fn;

    // `bind` user work unit
    if (work->ctx == ctx.bind) {
      trace.queue.bind.start(val);
      wv->dispatch(fn);
      cv.bind.wait(lock, [this] { return atomic.and_({atomic.done.bind()}); });
      if (atomic.lists.locked()) {
        cv.lists.wait(lock,
                      [this] { return atomic.and_({!atomic.lists.locked()}); });
      }
      lists.pending_binds.pop_front();
      atomic.done.bind(false);
    }
    // `unbind` user work unit
    if (work->ctx == ctx.unbind) {
      trace.queue.unbind.wait(val);
      auto &list = lists.name_unres_promises[val];
      auto timeout = std::chrono::milliseconds(WEBVIEW_UNBIND_TIMEOUT);
      cv.unbind_timeout.wait_for(lock, timeout, [this, list] {
        return atomic.and_({!atomic.lists.locked(), list.empty()});
      });
      if (atomic.lists.locked()) {
        cv.lists.wait(lock,
                      [this] { return atomic.and_({!atomic.lists.locked()}); });
      }
      trace.queue.unbind.start(val);
      auto &promises = lists.name_unres_promises[val];
      for (auto &id : promises) {
        wv->reject(id, utility::frontend.err_message.reject_unbound(id, val));
      }
      lists.name_unres_promises.erase(val);
      wv->dispatch(fn);
      cv.unbind.wait(lock,
                     [this] { return atomic.and_({atomic.done.unbind()}); });
      trace.queue.unbind.done(atomic.done.unbind(), val);
      atomic.done.unbind(false);
    }
    // `eval` user work unit
    if (work->ctx == ctx.eval) {
      trace.queue.eval.start(val);
      wv->dispatch(fn);
      cv.eval.wait(lock, [this] { return atomic.and_({atomic.done.eval()}); });
      trace.queue.eval.done(atomic.done.eval());
      atomic.done.eval(false);
    }
    if (atomic.lists.locked()) {
      cv.lists.wait(lock,
                    [this] { return atomic.and_({!atomic.lists.locked()}); });
    }
    atomic.queue.update();
    trace.queue.loop.end();
  }
}

void engine_queue::resolve_thread_constructor(str_arg_t name, str_arg_t id,
                                              str_arg_t args) {
  if (atomic.bindings.locked()) {
    std::mutex resolve_thread_mtx;
    std::unique_lock<std::mutex> lock(resolve_thread_mtx);
    cv.bindings.wait(
        lock, [this] { return atomic.and_({!atomic.bindings.locked()}); });
  }
  if (atomic.terminating()) {
    return;
  }
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
  atomic.lists.locked(true);
  if (fn_ctx == ctx.bind) {
    lists.pending_binds.push_back(name_or_js);
  }
  lists.queue.push_back({fn_ctx, fn, name_or_js});
  trace.queue.enqueue.added(char(fn_ctx), lists.queue.size(), name_or_js);
  atomic.lists.locked(false);
  atomic.queue.empty(false);
  return {};
};

bool engine_queue::atomic_done_t::bind() { return self->bind_done.load(); }
void engine_queue::atomic_done_t::bind(bool val) {
  self->bind_done.store(val);
  //if (val) {
  self->cv.bind.notify_one();
  //}
}
bool engine_queue::atomic_done_t::unbind() { return self->unbind_done.load(); }
void engine_queue::atomic_done_t::unbind(bool val) {
  self->unbind_done.store(val);
  //if (val) {
  self->cv.unbind.notify_one();
  //}
}
bool engine_queue::atomic_done_t::eval() { return self->eval_done.load(); }
void engine_queue::atomic_done_t::eval(bool val) {
  self->eval_done.store(val);
  //if (val) {
  self->cv.eval.notify_one();
  //}
}

bool engine_queue::atomic_dom_ready_t::ready() const {
  return self->is_dom_ready.load();
};
void engine_queue::atomic_dom_ready_t::ready(bool flag) const {
  self->is_dom_ready.store(flag);
  self->cv.queue.notify_one();
};

/*
void engine_queue::atomic_terminate_t::init() const {
  self->queue_empty.store(false);
  self->is_dom_ready.store(true);
  self->atomic.done.bind(true);
  self->atomic.done.unbind(true);
  self->atomic.done.eval(true);
  self->atomic.lists.locked(false);
  for (auto &this_cv : self->cv.all) {
    this_cv->notify_all();
  }
};
*/
void engine_queue::atomic_queue_t::update() const {
  self->atomic.lists.locked(true);
  if (self->lists.queue.size() > 1) {
    self->lists.queue.pop_front();
  } else {
    self->lists.queue.clear();
  }
  self->atomic.lists.locked(false);
  self->atomic.queue.empty(self->lists.queue.empty());
}
bool engine_queue::atomic_queue_t::empty() const {
  return self->queue_empty.load();
};
void engine_queue::atomic_queue_t::empty(bool val) const {
  self->queue_empty.store(val);
  self->cv.queue.notify_one();
}
void engine_queue::atomic_bindings_t::locked(bool val) const {
  self->bindings_locked.store(val);
  self->cv.bindings.notify_all();
}
bool engine_queue::atomic_bindings_t::locked() const {
  return self->bindings_locked.load();
}
void engine_queue::atomic_lists_t::locked(bool val) const {
  self->lists_locked.store(val);
  self->cv.queue.notify_one();
  self->cv.lists.notify_all();
}
bool engine_queue::atomic_lists_t::locked() const {
  return self->bindings_locked.load();
}
bool engine_queue::atomic_api_t::and_(std::initializer_list<bool> flags) const {
  if (self->atomic.terminating()) {
    return true;
  };
  auto res = true;
  for (auto &flag : flags) {
    if (!flag) {
      res = false;
      break;
    }
  }
  return res;
};
bool engine_queue::atomic_api_t::terminating() const {
  return self->wv->is_terminating.load();
};

} // namespace detail
} // namespace webview

#endif // defined(__cplusplus) && !defined(WEBVIEW_HEADER)
#endif // WEBVIEW_ENGINE_QUEUE_CC
