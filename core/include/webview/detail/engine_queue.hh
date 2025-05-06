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

#include "webview/types.hh"
#include "webview/utility/trace_log.hh"
#include <atomic>
#include <condition_variable>
#include <functional>
#include <list>
#include <thread>
#include <unordered_map>

namespace webview {
// Container: Global header declarations
namespace detail {

template <typename T> struct nested_api_t {
  T *self;
  nested_api_t(T *self) : self(self) {}
};

/// Shorthand for the commonly used `string` parameter to improve code readability.
using str_arg_t = const std::string &;
/// Shorthand type for callback lamda functions ie `bind`, `unbind` and `eval`.
using do_work_t = std::function<void()>;
class engine_base;

} // namespace detail

// Container: `engine_queue` class
namespace detail {
class engine_queue {
public:
  ~engine_queue() = default;
  engine_queue();

  /* ************************************************************************
   * Public API for `engine_queue`.
   *
   * Note:
   * This is a constrained public interface to private internal functionality
   * which is acceptable and idiomatic in C++ for API design.
   **************************************************************************/

  struct bind_api_t : nested_api_t<engine_queue> {
    ~bind_api_t() = default;
    bind_api_t(engine_queue *self) : nested_api_t(self) {}
    /// Puts a user `bind` work unit onto the queue.
    noresult enqueue(do_work_t fn, str_arg_t name) const;
  };
  struct unbind_api_t : nested_api_t<engine_queue> {
    ~unbind_api_t() = default;
    unbind_api_t(engine_queue *self) : nested_api_t(self) {}
    /// Puts a user `unbind` work unit onto the queue.
    noresult enqueue(do_work_t fn, str_arg_t name) const;
    /// Indicates to `unbind` if the name is queued to `bind`,
    /// in which case the future `unbind` is not an error.
    bool awaits_bind(str_arg_t name) const;
  };
  struct eval_api_t : nested_api_t<engine_queue> {
    ~eval_api_t() = default;
    eval_api_t(engine_queue *self) : nested_api_t(self) {}
    /// Puts a user `eval` work unit onto the queue.
    noresult enqueue(do_work_t fn, str_arg_t js) const;
  };
  struct promise_api_t : nested_api_t<engine_queue> {
    ~promise_api_t() = default;
    promise_api_t(engine_queue *self) : nested_api_t(self) {}
    /// Initialises an empty list for promise id's of a given bind name
    void list_init(str_arg_t name) const;
    /// Takes queue action for a resolved promise
    void resolved(str_arg_t id) const;
    /// Sends the native work unit of a promise to a concurrent thread.
    void resolve(str_arg_t name, str_arg_t id, str_arg_t args) const;
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
    void init(engine_base *wv);
    /// @brief Cleans up and shuts down the queue thread.
    ///
    /// This is the only instance where we lock the main / app thread.
    /// We do so to prevent segfault before the queue thread joins.
    void shutdown();
  };

  // NOLINTBEGIN(cppcoreguidelines-non-private-member-variables-in-classes)

  /// Public API for the engine_queue class instance.
  public_api_t user_queue;
  // NOLINTEND(cppcoreguidelines-non-private-member-variables-in-classes)

private:
  /// enum definitions for \ref ctx, ie.
  /// char 'b' = bind, 'u' = unbind, 'e' = eval
  enum context_t { bind_t = 'b', unbind_t = 'u', eval_t = 'e' };
  struct ctx_t {
    context_t bind = context_t::bind_t;
    context_t unbind = context_t::unbind_t;
    context_t eval = context_t::eval_t;
  };
  /// Container for user work operation tags, ie. `bind`, `unbind`, `eval`
  ctx_t const ctx{};

  /// @brief Constructs a thread to choreograph execution of `bind`, `unbind` or `eval` user work units.
  ///
  /// We do not want user work to stall the main / app thread.
  /// We need user work units to execute sequentially and atomically:
  /// - work must automatically queue until the DOM `__webview__` method is ready,
  /// - `unbind` must only execute after preceding `bind` work is completely registered,
  /// - JS promises must not be left indefinitely in a waiting state,
  /// - `eval` must wait for potentially included bound JS promises to register.
  /// - `unbind` must wait for resolution of relevant JS promises before rejecting them, and for this we set a sane timeout,
  void queue_thread_constructor();

  /// @brief Constructs a child thread for each native work unit of a bound JS promise.
  ///
  /// We want native promise work units to run concurrently.
  /// We do not want native promise work to stall execution of the main / app thread.
  /// @todo hardware concurrency limit queue.
  void resolve_thread_constructor(str_arg_t name, str_arg_t id, str_arg_t args);

  /// Adds `bind`, `unbind` or `eval` user work unit to the queue.
  noresult queue_work(str_arg_t name_or_js, do_work_t fn, context_t fn_ctx);
  /// Sends a native promise work unit to a concurrent detached thread.
  void resolve_work(engine_base *wv, str_arg_t msg, str_arg_t id);

  struct atomic_dom_ready_t : nested_api_t<engine_queue> {
    atomic_dom_ready_t(engine_queue *self) : nested_api_t(self) {}
    /// Query if the backend is ready to do work.
    bool ready() const;
    /// Notify the queue that the backend is ready to receive work.
    void ready(bool flag) const;
  };
  /*
  struct atomic_terminate_t : nested_api_t<engine_queue> {
    atomic_terminate_t(engine_queue *self) : nested_api_t(self) {}
    /// Query if Webview is in the process of terminating.
    bool terminating() const;
    /// Signal the queue to destroy itself.
    //void init() const;
  };
  */
  struct atomic_queue_t : nested_api_t<engine_queue> {
    atomic_queue_t(engine_queue *self) : nested_api_t(self) {}
    /// Decrements the queue list and flags empty state.
    void update() const;
    /// Query if the queue is empty.
    bool empty() const;
    /// Set the queue_empty flag.
    void empty(bool val) const;
  };
  struct atomic_done_t : nested_api_t<engine_queue> {
    atomic_done_t(engine_queue *self) : nested_api_t(self) {}
    /// Gets the bind flag state
    bool bind();
    /// Sets the bind flag state
    void bind(bool val);
    /// Gets the unbind flag state
    bool unbind();
    /// Sets the unbind flag state
    void unbind(bool val);
    /// Gets the eval flag state
    bool eval();
    /// Sets the eval flag state
    void eval(bool val);
  };
  struct atomic_bindings_t : nested_api_t<engine_queue> {
    atomic_bindings_t(engine_queue *self) : nested_api_t(self) {}
    /// Gets the bindings map locked state
    bool locked() const;
    /// Sets the bindings map locked state
    void locked(bool val) const;
  };
  struct atomic_lists_t : nested_api_t<engine_queue> {
    atomic_lists_t(engine_queue *self) : nested_api_t(self) {}
    /// Gets the lists locked state
    bool locked() const;
    /// Sets the lists locked state
    void locked(bool val) const;
  };
  struct atomic_api_t : nested_api_t<engine_queue> {
    ~atomic_api_t() = default;
    atomic_api_t(engine_queue *self) : nested_api_t(self) {}
    atomic_dom_ready_t dom{this->self};
    //atomic_terminate_t terminate{this->self};
    atomic_queue_t queue{this->self};
    atomic_done_t done{this->self};
    atomic_bindings_t bindings{this->self};
    atomic_lists_t lists{this->self};
    bool terminating() const;
    bool and_(std::initializer_list<bool> flags) const;
  };
  /// API to query and set various flags atomically
  atomic_api_t atomic;

  /// Structure for the \ref list.queue value
  struct action_t {
    context_t ctx;
    do_work_t fn;
    std::string val;
  };
  struct lists_api_t {
    /// A map keyed by bound function name containing a list it's unresolved promise id's
    std::unordered_map<std::string, std::list<std::string>> name_unres_promises;
    /// A map keyed by promise id referencing it's corresponding `bind` name
    std::unordered_map<std::string, std::string> promise_id_name;
    /// The ordered list of user work units to process
    std::list<action_t> queue;
    /// A list of queued `bind` names;
    std::list<std::string> pending_binds;
  };
  /// Grouping of list-like values
  lists_api_t lists;

  struct cv_api_t {
    std::condition_variable mutable queue;
    std::condition_variable mutable bind;
    std::condition_variable mutable eval;
    std::condition_variable mutable unbind;
    std::condition_variable mutable unbind_timeout;
    std::condition_variable mutable bindings;
    std::condition_variable mutable lists;
    std::condition_variable *all[7] = {
        &queue, &bind, &eval, &unbind, &unbind_timeout, &bindings, &lists};
  };
  /// Grouping of condition variables
  cv_api_t const cv{};

  std::atomic_bool is_dom_ready{};
  std::atomic_bool queue_empty{};
  std::atomic_bool unbind_done{};
  std::atomic_bool unbind_can_proceed{};
  std::atomic_bool bind_done{};
  std::atomic_bool eval_done{};
  std::atomic_bool bindings_locked{};
  std::atomic_bool lists_locked{};

  /// A thread to concurrently choreograph user work queueing.
  std::thread queue_thread;

  engine_base *wv;

  /// Temporary debug tracing utility
  /// @todo remove before merge
  utility::trace_t trace = {"Webview::"};
};

} // namespace detail
} // namespace webview
#endif // defined(__cplusplus) && !defined(WEBVIEW_HEADER)
#endif // WEBVIEW_ENGINE_QUEUE_HH
