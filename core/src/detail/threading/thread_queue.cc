/*
 * MIT License
 *
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

#ifndef WEBVIEW_ENGINE_THREAD_QUEUE_CC
#define WEBVIEW_ENGINE_THREAD_QUEUE_CC

#if defined(__cplusplus) && !defined(WEBVIEW_HEADER)
#include "webview/detail/engine_base.hh"
#include "webview/detail/engine_queue.hh"
#include "webview/detail/frontend/engine_frontend.hh"
#include <mutex>

namespace webview {
namespace detail {

void engine_queue::queue_thread_constructor(engine_base *wv_instance) {
  std::mutex queue_thread_mtx;
  std::unique_lock<std::mutex> lock(queue_thread_mtx);
  while (true) {
    log::trace::queue.loop.wait(list.queue.size(), list.queue.empty(),
                                atomic.dom.ready());
    cv.queue.wait(lock, [this] {
      return atomic.AND({atomic.dom.ready(), !list.queue.empty()});
    });
    if (atomic.terminating()) {
      break;
    }
    log::trace::queue.loop.start(list.queue.size());
    auto action = list.queue.front();
    context_t work_ctx = action.ctx;
    std::string name = action.name_or_js;
    std::string js = action.name_or_js;
    auto work_fn = action.work_fn;

    // `bind` user work unit
    if (work_ctx == ctx.bind) {
      log::trace::queue.bind.start(name);
      wv_instance->dispatch(work_fn);
      log::trace::queue.bind.wait(name);
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
      cv.unbind_timeout.wait_for(lock, timeout, [this, name] {
        return atomic.AND({list.unresolved_promises.empty(name)});
      });
      if (atomic.terminating()) {
        break;
      }
      log::trace::queue.unbind.start(name);
      auto promises = list.unresolved_promises.get_copy(name);
      for (auto &id : promises) {
        auto err = frontend.err_message.reject_unbound(id, name);
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

    log::trace::queue.loop.end();
    list.queue.pop_front();
  }
}

} // namespace detail
} // namespace webview

#endif // defined(__cplusplus) && !defined(WEBVIEW_HEADER)
#endif // WEBVIEW_ENGINE_THREAD_QUEUE_CC
