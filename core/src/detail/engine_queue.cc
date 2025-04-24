#ifndef WEBVIEW_ENGINE_QUEUE_CC
#define WEBVIEW_ENGINE_QUEUE_CC

#include <cstdio>
#include <exception>
#if defined(__cplusplus) && !defined(WEBVIEW_HEADER)

#include "webview/detail/engine_base.hh"
#include "webview/detail/engine_queue.hh"

namespace webview {
namespace detail {

engine_queue::engine_queue(engine_base *engine) : engine(engine) {};

void engine_queue::notify(str_arg_t method) {
  if (method == "dom_ready") {
    debug.queue.notify.on_notify(method.c_str());
    set_dom_ready();
  }
  if (method == "on_bind") {
    debug.queue.notify.on_notify(method.c_str());
    bind.set_done(true);
  }
  if (method == "on_unbind") {
    debug.queue.notify.on_notify(method.c_str());
    unbind.set_done(true);
  }
}

void engine_queue::terminate() {
  std::mutex mtx;
  lock_t lock(mtx);
  bind.set_done(true);
  unbind.set_done(true);
  eval.set_done(true);
  flags.set_terminating();
  cv.queue.wait(lock, [this] { return queue_thread.joinable(); });
  queue_thread.join();
}
void engine_queue::set_dom_ready() { flags.set_dom_ready(); }

void engine_queue::register_unresolved_binds(str_arg_t js) {
  for (auto &bind : list.bind_promise_ids) {
    str_arg_t name = bind.first;
    if (list.contains(list.pending_promise_binds, name)) {
      return;
    }
    auto const &needle = "" + name + "(";
    auto found = js.find(needle) != std::string::npos;
    if (found) {
      list.add(list.pending_promise_binds, name);
    }
  }
};

noresult engine_queue::enqueue_work(str_arg_t name, do_work_t fn,
                                    scope_t scp) const {
  debug.queue.enqueue.added(name.c_str());
  if (scp == scope.bind) {
    list.pending_binds.push_back(name);
  }
  if (scp == scope.unbind) {
    list.pending_unbinds.push_back(name);
  }
  return enqueue_work(fn, scp);
};
noresult engine_queue::enqueue_work(do_work_t fn, scope_t scp) const {
  list.queue.emplace_back(scp, fn);
  debug.queue.enqueue.added(scp, list.queue.size());
  cv.queue.notify_one();
  return {};
};
void engine_queue::set_done(bool val, scope_t scp) {
  if (scp == scope.bind) {
    flags.done.bind.store(val);
    if (val) {
      cv.bind.notify_one();
    }
  }
  if (scp == scope.unbind) {
    flags.done.unbind.store(val);
    if (val) {
      cv.unbind.notify_one();
    }
  }
  if (scp == scope.eval) {
    flags.done.eval.store(val);
    if (val) {
      cv.eval.notify_one();
    }
  }
};

void engine_queue::resolve(binding_ctx_t *ctx, str_arg_t name, str_arg_t id,
                           str_arg_t msg) {
  std::thread resolver = std::thread(&engine_queue::resolve_thread_constructor,
                                     this, ctx, name, id, msg);
  resolver.detach();
}
void engine_queue::resolve_thread_constructor(binding_ctx_t *ctx,
                                              str_arg_t name, str_arg_t id,
                                              str_arg_t msg) {
  std::mutex mtx;
  lock_t lock(mtx, std::defer_lock);

  auto args = json_parse(msg, "params", 0);

  try {
    ctx->call(id, args);
  } catch (const std::exception &err) {
    auto e_message = "Uncaught exception from native user callback function `" +
                     name + "`:\n" + std::string(err.what());

    engine->dispatch([this, id, e_message] {
      engine->resolve(id, 1, json_escape(e_message));
    });

  } catch (...) {
    auto e_message =
        "\nNative user callback function `" + name +
        "` failed because Webview terminated before it could complete.\n\n";
    perror(e_message.c_str());
  };
}

void engine_queue::queue_thread_constructor() {
  std::mutex mtx;
  std::unique_lock<std::mutex> lock(mtx);

  auto const bind_fn = [this, &lock](do_work_t fn) {
    debug.queue.bind.start();
    try {
      engine->dispatch(fn);
      cv.bind.wait(lock, [this] { return bind.get_done(); });
    } catch (std::exception &e) {
      perror(e.what());
    }
    //list.pending_binds.pop_front();
    debug.queue.bind.done(bind.get_done());
  };

  auto const unbind_fn = [this, &lock](do_work_t fn) {
    debug.queue.unbind.start();
    try {
      engine->dispatch(fn);
      cv.unbind.wait(lock, [this] { return unbind.get_done(); });
    } catch (std::exception &e) {
      perror(e.what());
    }
    //list.pending_unbinds.pop_front();
    debug.queue.unbind.done(unbind.get_done());
  };
  auto const eval_fn = [this, &lock](do_work_t fn) {
    debug.queue.eval.start();
    try {
      engine->dispatch(fn);
      cv.eval.wait(lock, [this] { return eval.get_done(); });
    } catch (std::exception &e) {
      perror(e.what());
    }
    debug.queue.eval.done(eval.get_done());
  };

  while (true) {
    debug.queue.loop.wait(list.queue.size(), flags.get_queue_empty(),
                          flags.get_dom_ready());
    try {
      cv.queue.wait(lock, [this] {
        return flags.get_dom_ready() && !flags.get_queue_empty();
      });
      if (flags.get_terminating()) {
        break;
      }

      debug.queue.loop.start(list.queue.size());

      auto work = list.queue.front();
      auto current_fn = work.first;
      auto work_fn = work.second;

      if (current_fn == scope.bind) {
        bind_fn(work_fn);
        bind.set_done(false);
      }
      if (current_fn == scope.unbind) {
        debug.queue.unbind.wait();
        cv.queue.wait_for(lock, std::chrono::seconds(2),
                          [this] { return flags.get_terminating(); });
        unbind_fn(work_fn);
        unbind.set_done(false);
      }
      if (current_fn == scope.eval) {
        eval_fn(work_fn);
        eval.set_done(false);
      }

      flags.set_queue_empty();
      debug.queue.loop.end(list.queue.size());
    } catch (std::exception &e) {
      perror(e.what());
    }
  }
}

engine_queue::lists_api_t engine_queue::list{
    {}, {}, {}, {}, {}, {},
};
bool engine_queue::lists_api_t::contains(list_t<> &targ_list, str_arg_t value) {
  auto pos = std::find(targ_list.begin(), targ_list.end(), value);
  auto found = pos != targ_list.end();
  pos = targ_list.end();
  return found;
}
void engine_queue::lists_api_t::erase(list_t<> &targ_list, str_arg_t value) {
  auto found = std::find(targ_list.begin(), targ_list.end(), value);
  if (found != targ_list.end()) {
    targ_list.erase(found);
    found = targ_list.end();
  }
}
template <typename T>
void engine_queue::lists_api_t::add(list_t<T> &targ_list, const T &value) {
  targ_list.emplace_back(value);
}
void engine_queue::lists_api_t::add_unique(list_t<> &targ_list,
                                           str_arg_t value) {
  if (!list.contains(targ_list, value)) {
    targ_list.emplace_back(value);
  }
}

void engine_queue::promise_list_init(str_arg_t name) {
  list.bind_promise_ids[name] = {};
}
void engine_queue::promises_add(str_arg_t name, str_arg_t id) {
  list.promise_id_bind[id] = name;
  list.bind_promise_ids[name].push_back(id);
}
void engine_queue::promise_erase(str_arg_t id) {
  auto name = list.promise_id_bind[id];
  list.promise_id_bind.erase(id);
  auto id_list = list.bind_promise_ids[name];
  list.erase(id_list, id);
  if (id_list.empty()) {
    list.bind_promise_ids.erase(name);
    list.erase(list.pending_promise_binds, name);
    //cv.notify_one();
  }
}
void engine_queue::promise_flag_bind(str_arg_t name) {
  list.add_unique(list.pending_promise_binds, name);
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

engine_queue::cv_api_t engine_queue::cv{};

} // namespace detail
} // namespace webview

#endif // defined(__cplusplus) && !defined(WEBVIEW_HEADER)
#endif // WEBVIEW_ENGINE_QUEUE_CC