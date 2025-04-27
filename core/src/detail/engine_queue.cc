#ifndef WEBVIEW_ENGINE_QUEUE_CC
#define WEBVIEW_ENGINE_QUEUE_CC

#if defined(__cplusplus) && !defined(WEBVIEW_HEADER)

#include "webview/detail/engine_queue.hh"
#include "webview/detail/engine_frontend.hh"
#include <cstdio>

namespace webview {
detail::engine_queue::engine_queue(engine_base *base)
    : promises{this},
      bind{this},
      unbind{this},
      eval{this},
      queue_thread(&engine_queue::queue_thread_constructor, this),
      flags{this},
      base(base) {};

// Container: PRIVATE structures for public API
namespace detail {

engine_queue::promise_api_t::promise_api_t(engine_queue *s) : api_base(s) {}
void engine_queue::promise_api_t::list_init(str_arg_t name) const {
  s->promise_list_init(name);
};
void engine_queue::promise_api_t::resolved(str_arg_t id) const {
  s->promise_erase(id);
};

engine_queue::bind_api_t::bind_api_t(engine_queue *s) : api_base(s) {};
noresult engine_queue::bind_api_t::enqueue(do_work_t fn, str_arg_t name) const {
  return s->enqueue_work(name, fn, ctx::bind);
};

engine_queue::unbind_api_t::unbind_api_t(engine_queue *s) : api_base(s) {};
noresult engine_queue::unbind_api_t::enqueue(do_work_t fn,
                                             str_arg_t name) const {
  return s->enqueue_work(name, fn, ctx::unbind);
};
bool engine_queue::unbind_api_t::awaits_bind(str_arg_t name) const {
  return s->unbind_awaiting_bind(name);
};

engine_queue::eval_api_t::eval_api_t(engine_queue *s) : api_base(s) {};
noresult engine_queue::eval_api_t::enqueue(do_work_t fn, str_arg_t js) const {
  return s->enqueue_work(js, fn, ctx::eval);
};

} // namespace detail

// Container: PUBLIC API
namespace detail {

void engine_queue::resolve(str_arg_t msg,
                           std::map<std::string, binding_ctx_t> *bindings) {
  auto name = json_parse(msg, "method", 0);
  auto id = json_parse(msg, "id", 0);
  if (frontend_notification(id, name)) {
    return;
  }
  list.promise_id_name[id] = name;
  list.name_unres_promises[name].push_back(id);
  std::thread resolver = std::thread(&engine_queue::resolve_thread_constructor,
                                     this, name, id, msg, bindings);
  resolver.detach();
}

void engine_queue::terminate() {
  std::mutex mtx;
  std::unique_lock<std::mutex> lock(mtx);

  flags.set_terminating();
  cv.queue.wait(lock, [this] { return queue_thread.joinable(); });
  queue_thread.join();
}

} // namespace detail

// Container: PRIVATE thread management
namespace detail {

void engine_queue::queue_thread_constructor() {
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
    auto ctx = work->ctx;
    auto fn = work->fn;

    // `bind` user work unit
    if (ctx == ctx::bind) {
      trace.queue.bind.start(val);
      base->dispatch(fn);
      cv.bind.wait(lock, [this] { return flags.done.bind(); });
      list.pending_binds.pop_front();
      trace.queue.bind.done(flags.done.bind(), val);
      flags.done.bind(false);
    }

    // `unbind` user work unit
    if (ctx == ctx::unbind) {
      trace.queue.unbind.wait(val);
      cv.unbind_timeout.wait_for(
          lock, std::chrono::milliseconds(WEBVIEW_UNBIND_TIMEOUT), [this, val] {
            return flags.get_terminating() ||
                   list.name_unres_promises[val].empty();
          });
      auto promises = list.name_unres_promises[val];
      for (auto &id : promises) {
        base->reject(id, frontend.err_message.reject_unbound(id, val));
      }
      list.name_unres_promises.erase(val);
      trace.queue.unbind.start(val);
      base->dispatch(fn);
      cv.unbind.wait(lock, [this] { return flags.done.unbind(); });
      trace.queue.unbind.done(flags.done.unbind(), val);
      flags.done.unbind(false);
    }

    // `eval` user work unit
    if (ctx == ctx::eval) {
      trace.queue.eval.start(val);
      base->dispatch(fn);
      cv.eval.wait(lock, [this] { return flags.done.eval(); });
      trace.queue.eval.done(flags.done.eval());
      flags.done.eval(false);
    }

    flags.update_queue_size();
    trace.queue.loop.end();
  }
}

void engine_queue::resolve_thread_constructor(
    str_arg_t name, str_arg_t id, str_arg_t msg,
    std::map<std::string, binding_ctx_t> *bindings) {

  auto is_unbound = bindings->count(name) == 0;
  if (is_unbound) {
    base->reject(id, frontend.err_message.reject_unbound(id, name));
    return;
  }
  auto args = json_parse(msg, "params", 0);
  auto &ctx = bindings->at(name);

  try {
    ctx.call(id, args);
  } catch (const std::exception &err) {
    base->reject(id, frontend.err_message.uncaught_exception(name, err.what()));
  } catch (...) {
    perror(frontend.err_message.webview_terminated(name).c_str());
  };
}

} // namespace detail

// Container: PRIVATE methods and values
namespace detail {

noresult engine_queue::enqueue_work(str_arg_t name_or_js, do_work_t fn,
                                    context_t fn_ctx) const {
  if (fn_ctx == ctx::bind) {
    list.pending_binds.push_back(name_or_js);
  }

  trace.queue.enqueue.added(fn_ctx, list.queue.size(), name_or_js);
  list.queue.push_back({fn_ctx, fn, name_or_js});
  cv.queue.notify_one();
  return {};
};

bool engine_queue::frontend_notification(str_arg_t id, str_arg_t method) {
  if (id != SYSTEM_NOTIFICATION_FLAG) {
    return false;
  };
  if (method == DOM_READY_M) {
    trace.queue.notify.on_message(method);
    flags.set_dom_ready();
  }
  if (method == BIND_DONE_M) {
    trace.queue.notify.on_message(method);
    flags.done.bind(true);
  }
  if (method == UNBIND_DONE_M) {
    trace.queue.notify.on_message(method);
    flags.done.unbind(true);
  }
  if (method == EVAL_READY_M) {
    trace.queue.notify.on_message(method);
    flags.done.eval(true);
  }
  return true;
}

void engine_queue::set_done(bool val, context_t fn_ctx) {
  if (fn_ctx == ctx::bind) {
    bind_done.store(val);
    if (val) {
      cv.bind.notify_one();
    }
  }
  if (fn_ctx == ctx::unbind) {
    unbind_done.store(val);
    if (val) {
      cv.unbind.notify_one();
    }
  }
  if (fn_ctx == ctx::eval) {
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

void engine_queue::promise_erase(str_arg_t id) const {
  auto name = list.promise_id_name[id];
  list.promise_id_name.erase(list.promise_id_name.at(id));
  list.name_unres_promises[name].remove(id);
  cv.unbind_timeout.notify_one();
  trace.queue.print_here("Promise erased: " + name + " | " + id);
};

engine_queue::lists_api_t engine_queue::list{{}, {}, {}, {}};
engine_queue::cv_api_t engine_queue::cv{};

} // namespace detail

// Container: PRIVATE flags API
namespace detail {

engine_queue::done_t::done_t(engine_queue *s) : api_base(s) {};
bool engine_queue::done_t::bind() { return s->bind_done.load(); }
void engine_queue::done_t::bind(bool val) { s->set_done(val, ctx::bind); }
bool engine_queue::done_t::unbind() { return s->unbind_done.load(); }
void engine_queue::done_t::unbind(bool val) { s->set_done(val, ctx::unbind); }
bool engine_queue::done_t::eval() { return s->eval_done.load(); }
void engine_queue::done_t::eval(bool val) { s->set_done(val, ctx::eval); }

engine_queue::flags_api_t::flags_api_t(engine_queue *s)
    : api_base(s), done{s} {};
bool engine_queue::flags_api_t::get_dom_ready() {
  return s->is_dom_ready.load();
};
void engine_queue::flags_api_t::set_dom_ready() {
  s->is_dom_ready.store(true);
  engine_queue::cv.queue.notify_one();
};
bool engine_queue::flags_api_t::get_terminating() {
  return s->is_terminating.load();
};
void engine_queue::flags_api_t::set_terminating() {
  s->is_terminating.store(true);
  s->queue_empty.store(false);
  s->is_dom_ready.store(true);
  s->flags.done.bind(true);
  s->flags.done.unbind(true);
  s->flags.done.eval(true);
  cv.queue.notify_one();
  cv.unbind_timeout.notify_one();
};
void engine_queue::flags_api_t::update_queue_size() {
  if (list.queue.size() > 1) {
    list.queue.pop_front();
  } else {
    list.queue.clear();
  }
  s->queue_empty.store(list.queue.empty());
}
bool engine_queue::flags_api_t::queue_empty() { return s->queue_empty.load(); };

} // namespace detail

} // namespace webview

#endif // defined(__cplusplus) && !defined(WEBVIEW_HEADER)
#endif // WEBVIEW_ENGINE_QUEUE_CC