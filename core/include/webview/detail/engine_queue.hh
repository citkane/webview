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

#ifndef WEBVIEW_ENGINE_QUEUE_HH
#define WEBVIEW_ENGINE_QUEUE_HH

#if defined(__cplusplus) && !defined(WEBVIEW_HEADER)
#ifndef WEBVIEW_UNBIND_TIMEOUT
#define WEBVIEW_UNBIND_TIMEOUT 20
#endif
#include "threading/threadsafe_lists.hh"
#include "webview/detail/threading/atomic_api.hh"
#include "webview/types/types.hh"
#include <atomic>
#include <thread>

namespace webview {
namespace detail {
namespace backend {

class engine_queue : protected engine_lists_t {
public:
  ~engine_queue() = default;
  engine_queue();

  /* ************************************************************************
   * API for `engine_queue`.
   *
   * Note:
   * This is a constrained public interface to private internal functionality,
   * which is acceptable and idiomatic in C++ for API design.
   **************************************************************************/

  struct bind_api_t : nested_api_t<engine_queue> {
    ~bind_api_t() = default;
    bind_api_t(engine_queue *self) : nested_api_t(self) {}
    /// Puts a user `bind` work unit onto the queue.
    noresult enqueue(dispatch_fn_t fn, str_arg_t name) const;
    /// Indicates if adding a `bind` to the queue is an error, eg. duplicate name.
    bool is_duplicate(str_arg_t name) const;
  };
  struct unbind_api_t : nested_api_t<engine_queue> {
    ~unbind_api_t() = default;
    unbind_api_t(engine_queue *self) : nested_api_t(self) {}
    /// Puts a user `unbind` work unit onto the queue.
    noresult enqueue(dispatch_fn_t fn, str_arg_t name) const;
    /// Indicates if adding an `unbind` to the queue is an error, eg. bind doesn't exist.
    bool not_found(str_arg_t name) const;
  };
  struct eval_api_t : nested_api_t<engine_queue> {
    ~eval_api_t() = default;
    eval_api_t(engine_queue *self) : nested_api_t(self) {}
    /// Puts a user `eval` work unit onto the queue.
    noresult enqueue(dispatch_fn_t fn, str_arg_t js) const;
  };
  struct promise_api_t : nested_api_t<engine_queue> {
    ~promise_api_t() = default;
    promise_api_t(engine_queue *self) : nested_api_t(self) {}

    /// Takes queue action for a resolved promise
    void resolved(str_arg_t id) const;
    /// Sends the native work unit of a promise to a concurrent thread.
    void resolve(str_arg_t name, str_arg_t id, str_arg_t args,
                 engine_base *wv) const;
    /// Relays notifications from the frontend to relevant queue methods.
    bool is_system_message(str_arg_t id, str_arg_t method);
  };
  struct bindings_api_t : nested_api_t<engine_queue> {
    bindings_api_t(engine_queue *self) : nested_api_t(self) {}
    /// Sets the bindings map locked state
    void locked(bool val) const;
  };
  struct public_api_t : nested_api_t<engine_queue> {
    ~public_api_t() = default;
    public_api_t(engine_queue *self) : nested_api_t{self} {};
    bind_api_t bind{this->self};
    unbind_api_t unbind{this->self};
    eval_api_t eval{this->self};
    promise_api_t promises{this->self};
    bindings_api_t bindings{this->self};

    /// Initialises the user work unit queue thread;
    void init(engine_base *wv) const;
    /// Releases and shuts down the queue thread.
    void shutdown() const;
    bool shutting_down() const;
  };

  // NOLINTBEGIN(cppcoreguidelines-non-private-member-variables-in-classes)

  /// Public API for the engine_queue class instance.
  public_api_t queue;

  // NOLINTEND(cppcoreguidelines-non-private-member-variables-in-classes)

private:
  friend struct _structs::atomic_dom_ready_t;
  friend struct _structs::atomic_done_t;
  friend struct _structs::atomic_api_t;

  /// Container for user work operation tags, ie. `bind`, `unbind`, `eval`
  action_ctx_t const ctx{};

  /// @brief Constructs a thread to choreograph execution of `bind`, `unbind` or `eval` user work units.
  ///
  /// We do not want user work to stall the main / app thread.
  /// We need user work units to execute sequentially and atomically:
  /// - work must automatically queue until the DOM `__webview__` method is ready,
  /// - `unbind` must only execute after preceding `bind` work is completely registered,
  /// - JS promises must not be left indefinitely in a waiting state,
  /// - `eval` must wait for potentially included bound JS promises to register.
  /// - `unbind` must wait for resolution of relevant JS promises before rejecting them, and for this we set a sane timeout,
  void queue_thread_constructor(engine_base *wv_instance);

  /// @brief Constructs a child thread for each native work unit of a bound JS promise.
  ///
  /// We want native promise work units to run concurrently.
  /// We do not want native promise work to stall execution of the main / app thread.
  /// @todo hardware concurrency limit queue.
  void resolve_thread_constructor(str_arg_t name, str_arg_t id, str_arg_t args,
                                  engine_base *wv);

  /// Adds `bind`, `unbind` or `eval` user work unit to the queue.
  noresult queue_work(str_arg_t name_or_js, dispatch_fn_t fn, context_t fn_ctx);

  /// Sends a native promise work unit to a concurrent detached thread.
  void resolve_work(engine_base *wv, str_arg_t msg, str_arg_t id);

  /// API to query and set various flags atomically
  _structs::atomic_api_t atomic;

  std::atomic_bool is_dom_ready{};
  std::atomic_bool queue_empty{};
  std::atomic_size_t queue_size{};
  std::atomic_bool unbind_done{};
  std::atomic_bool unbind_can_proceed{};
  std::atomic_bool bind_done{};
  std::atomic_bool eval_done{};
  std::atomic_bool is_terminating{};

  /// A thread to concurrently choreograph user work queueing.
  std::thread queue_thread;
};

} // namespace backend
} // namespace detail
} // namespace webview
#endif // defined(__cplusplus) && !defined(WEBVIEW_HEADER)
#endif // WEBVIEW_ENGINE_QUEUE_HH
