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
#include "webview/frontend/engine_frontend.hh"
#include "webview/log/trace_log.hh"
#include <cstdio>
#include <initializer_list>
#include <mutex>

namespace webview {
detail::engine_queue::~engine_queue() { queue_thread.join(); };
detail::engine_queue::engine_queue() : queue{this}, atomic{this} {}

// PUBLIC API implementation
namespace detail {

using public_api_t = engine_queue::public_api_t;
using bind_api_t = engine_queue::bind_api_t;
using unbind_api_t = engine_queue::unbind_api_t;
using promise_api_t = engine_queue::promise_api_t;
using eval_api_t = engine_queue::eval_api_t;

noresult bind_api_t::enqueue(do_work_t fn, str_arg_t name) const {
  return self->queue_work(name, fn, self->ctx.bind);
};
bool bind_api_t::is_duplicate(str_arg_t name) const {
  auto i = self->list.pending.indices(name);
  auto will_be_bound = i.bind_i > -1 && i.bind_i > i.unbind_i;
  return self->list.bindings.count(name) > 0 || will_be_bound;
};

noresult unbind_api_t::enqueue(do_work_t fn, str_arg_t name) const {
  return self->queue_work(name, fn, self->ctx.unbind);
};
bool unbind_api_t::not_found(str_arg_t name) const {
  auto i = self->list.pending.indices(name);
  auto will_be_bound = i.bind_i > -1 && i.bind_i > i.unbind_i;
  return self->list.bindings.count(name) == 0 && !will_be_bound;
};

noresult eval_api_t::enqueue(do_work_t fn, str_arg_t js) const {
  return self->queue_work(js, fn, self->ctx.eval);
};

void promise_api_t::resolved(str_arg_t id) const {
  auto name = self->list.id_name_map.get(id);
  if (name == "") {
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
  {
    std::mutex mtx;
    std::lock_guard<std::mutex> lock(mtx);
    std::thread resolver = std::thread(
        &engine_queue::resolve_thread_constructor, self, name, id, args, wv);
    resolver.detach();
  }
}
bool promise_api_t::is_system_message(str_arg_t id, str_arg_t method) {
  if (id != SYSTEM_NOTIFICATION_FLAG) {
    return false;
  };
  if (method == DOM_READY_M) {
    log::trace::queue.notify.on_message(method);
    self->atomic.dom.ready(true);
  }
  if (method == BIND_DONE_M) {
    log::trace::queue.notify.on_message(method);
    self->atomic.done.bind(true);
  }
  if (method == UNBIND_DONE_M) {
    log::trace::queue.notify.on_message(method);
    self->atomic.done.unbind(true);
  }
  if (method == EVAL_READY_M) {
    log::trace::queue.notify.on_message(method);
    self->atomic.done.eval(true);
  }
  return true;
}

void public_api_t::init(engine_base *wv_instance) const {
  std::mutex mtx;
  std::lock_guard<std::mutex> lock(mtx);
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
}
bool public_api_t::shutting_down() const { return self->is_terminating.load(); }
void public_api_t::shutting_down(bool flag) const {
  self->is_terminating.store(flag);
}

void engine_queue::queue_thread_constructor(engine_base *wv_instance) {
  std::mutex queue_thread_mtx;
  std::unique_lock<std::mutex> lock(queue_thread_mtx);
  while (true) {
    log::trace::queue.loop.wait(list.queue.size(), atomic.queue.empty(),
                                atomic.dom.ready());
    cv.queue.wait(lock, [this] {
      return atomic.AND({atomic.dom.ready(), !atomic.queue.empty()});
    });
    if (atomic.terminating()) {
      break;
    }
    log::trace::queue.loop.start(list.queue.size());
    auto action = std::move(list.queue.front());
    context_t work_ctx = action.ctx;
    std::string name = action.name_or_js;
    std::string js = action.name_or_js;
    auto work_fn = action.work_fn;

    // `bind` user work unit
    if (work_ctx == ctx.bind) {
      log::trace::queue.bind.start(name);
      wv_instance->dispatch(work_fn);
      cv.bind.wait(lock, [this] { return atomic.AND({atomic.done.bind()}); });
      if (atomic.terminating()) {
        break;
      }
      list.pending.pop_front();
      log::trace::queue.bind.done(atomic.done.bind(), name);
      atomic.done.bind(false);
    }

    // `unbind` user work unit
    if (work_ctx == ctx.unbind) {
      log::trace::queue.unbind.wait(name);
      auto timeout = std::chrono::milliseconds(WEBVIEW_UNBIND_TIMEOUT);
      cv.unbind_timeout.wait_for(lock, timeout, [this, &name] {
        return atomic.AND({list.unresolved_promises.empty(name)});
      });
      if (atomic.terminating()) {
        break;
      }
      log::trace::queue.unbind.start(name);
      auto promises = list.unresolved_promises.get_copy(name);
      for (auto &id : promises) {
        auto err = utility::frontend.err_message.reject_unbound(id, name);
        wv_instance->reject(id, err);
      }

      wv_instance->dispatch(work_fn);
      cv.unbind.wait(lock,
                     [this] { return atomic.AND({atomic.done.unbind()}); });
      list.pending.pop_front();
      log::trace::queue.unbind.done(atomic.done.unbind(), name);
      atomic.done.unbind(false);
    }

    // `eval` user work unit
    if (work_ctx == ctx.eval) {
      log::trace::queue.eval.start(js);
      wv_instance->dispatch(work_fn);
      cv.eval.wait(lock, [this] { return atomic.AND({atomic.done.eval()}); });
      if (atomic.terminating()) {
        break;
      }
      log::trace::queue.eval.done(atomic.done.eval());
      atomic.done.eval(false);
    }

    atomic.queue.update();
    log::trace::queue.loop.end();
  }
}

void engine_queue::resolve_thread_constructor(str_arg_t name, str_arg_t id,
                                              str_arg_t args, engine_base *wv) {
  if (atomic.terminating()) {
    return;
  }
  try {
    list.bindings.at(name).call(id, args);
  } catch (const std::exception &err_) {
    auto err =
        utility::frontend.err_message.uncaught_exception(name, err_.what());
    wv->reject(id, err);
  } catch (...) {
    perror(utility::frontend.err_message.webview_terminated(name).c_str());
  };
}

noresult engine_queue::queue_work(str_arg_t name_or_js, do_work_t fn,
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
  atomic.queue.empty(false);
  return {};
};

bool engine_queue::atomic_done_t::bind() { return self->bind_done.load(); }
void engine_queue::atomic_done_t::bind(bool val) {
  self->bind_done.store(val);
  self->cv.bind.notify_one();
}
bool engine_queue::atomic_done_t::unbind() { return self->unbind_done.load(); }
void engine_queue::atomic_done_t::unbind(bool val) {
  self->unbind_done.store(val);
  self->cv.unbind.notify_one();
}
bool engine_queue::atomic_done_t::eval() { return self->eval_done.load(); }
void engine_queue::atomic_done_t::eval(bool val) {
  self->eval_done.store(val);
  self->cv.eval.notify_one();
}

bool engine_queue::atomic_dom_ready_t::ready() const {
  return self->is_dom_ready.load();
};
void engine_queue::atomic_dom_ready_t::ready(bool flag) const {
  self->is_dom_ready.store(flag);
  self->cv.queue.notify_one();
};

void engine_queue::atomic_queue_t::update() const {
  if (self->list.queue.size() > 1) {
    self->list.queue.pop_front();
  } else {
    self->list.queue.clear();
  }
  self->atomic.queue.empty(self->list.queue.empty());
}

bool engine_queue::atomic_queue_t::empty() const {
  return self->queue_empty.load();
};
void engine_queue::atomic_queue_t::empty(bool val) const {
  self->queue_empty.store(val);
  self->cv.queue.notify_one();
}

bool engine_queue::atomic_api_t::AND(std::initializer_list<bool> flags) const {
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
  return self->is_terminating.load();
};

} // namespace detail
} // namespace webview

#endif // defined(__cplusplus) && !defined(WEBVIEW_HEADER)
#endif // WEBVIEW_ENGINE_QUEUE_CC
