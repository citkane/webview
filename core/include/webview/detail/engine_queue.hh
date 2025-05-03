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
#include <atomic>
#include <condition_variable>
#include <functional>
#include <list>
#include <thread>
#include <unordered_map>

namespace webview {
// Container: Global header declarations
namespace detail {

template <typename T> struct api_base {
  T *self;
  api_base(T *self) : self(self) {}
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
   * This is a constrained public interface to private internal functionality,
   * thus acceptable and idiomatic in C++ for API design.
   **************************************************************************/
  struct public_api_t : api_base<engine_queue> {
    public_api_t(engine_queue *self);

    struct bind_api_t : api_base<engine_queue> {
      bind_api_t(engine_queue *self) : api_base(self) {}
      /// Puts a user `bind` work unit onto the queue.
      noresult enqueue(do_work_t fn, str_arg_t name) const;
    } bind;

    struct unbind_api_t : api_base<engine_queue> {
      unbind_api_t(engine_queue *self) : api_base(self) {}
      /// Puts a user `unbind` work unit onto the queue.
      noresult enqueue(do_work_t fn, str_arg_t name) const;
      /// Indicates to `unbind` if the name is queued to `bind`,
      /// in which case the future `unbind` is not an error.
      bool awaits_bind(str_arg_t name) const;
    } unbind;

    struct eval_api_t : api_base<engine_queue> {
      eval_api_t(engine_queue *self) : api_base(self) {}
      /// Puts a user `eval` work unit onto the queue.
      noresult enqueue(do_work_t fn, str_arg_t js) const;
    } eval;

    struct promise_api_t : api_base<engine_queue> {
      promise_api_t(engine_queue *self) : api_base(self) {}
      /// Initialises an empty list for promise id's of a given bind name
      void list_init(str_arg_t name) const;
      /// Takes queue action for a resolved promise
      void resolved(str_arg_t id) const;
      /// Sends the native work unit of a promise to a concurrent thread.
      void resolve(engine_base *wv, str_arg_t name, str_arg_t id,
                   str_arg_t args) const;
      /// Relays notifications from the frontend to relevant queue methods.
      bool is_system_message(str_arg_t id, str_arg_t method);
    } promises;

    void init_queue(engine_base *wv);
    /// @brief Cleans up and shuts down the queue thread.
    ///
    /// This is the only instance where we lock the main / app thread.
    /// We do so to prevent segfault before the queue thread joins.
    void shutdown_queue();
  };
  // NOLINTBEGIN(cppcoreguidelines-non-private-member-variables-in-classes)

  /// Public API for the engine_queue class instance.
  public_api_t queue;

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
  ctx_t ctx{};

  /// @brief Constructs a thread to choreograph execution of `bind`, `unbind` or `eval` user work units.
  /// We do not want user work to stall the main / app thread.
  ///
  /// We need user work units to execute sequentially and atomically:
  /// - work must automatically queue until the DOM `__webview__` method is ready,
  /// - `unbind` must only execute after preceding `bind` work is completely registered,
  /// - JS promises must not be left to indefinitely hang in a waiting state,
  /// - `eval` must wait for potentially included bound JS promises to register.
  /// - `unbind` must wait for resolution of relevant JS promises before rejecting them,
  ///    and for this we set a sane timeout,
  void queue_thread_constructor(engine_base *wv);

  /// Constructs a child thread for each native work unit of a bound JS promise.
  /// We want native promise work units to run concurrently.
  /// We do not want native promise work to stall execution of the main / app thread.
  void resolve_thread_constructor(engine_base *wv, str_arg_t name, str_arg_t id,
                                  str_arg_t args);
  /// Adds `bind`, `unbind` or `eval` user work unit to the queue.
  noresult queue_work(str_arg_t name_or_js, do_work_t fn, context_t fn_ctx);
  /// Sends a native promise work unit to a concurrent thread.
  void resolve_work(engine_base *wv, str_arg_t msg, str_arg_t id);
  /// Flags a `bind`, `unbind` or `eval` user work unit as  done.
  void set_done(bool val, context_t fn_ctx);
  /// Checks if `unbind` is referencing an earlier `bind` function that has not yet completed.
  bool unbind_awaiting_bind(str_arg_t name);
  /// Initialises an empty list of potential promises for a bound function name.
  void promise_list_init(str_arg_t name);
  /// Removes a resolved promise from a bound function's register
  void promise_erase(str_arg_t id);

  /// \ref flags_api_t.done nested API type
  struct done_t : api_base<engine_queue> {
    done_t(engine_queue *self) : api_base(self) {}
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
  /// API type to query and set various flags \ref flags
  struct flags_api_t : api_base<engine_queue> {
    ~flags_api_t() = default;
    flags_api_t(engine_queue *self) : api_base(self), done{self} {};
    /// Query if the backend is ready to do work.
    bool get_dom_ready() const;
    /// Notify the queue that the backend is ready to receive work.
    void set_dom_ready() const;
    /// Query if Webview is in the process of terminating.
    bool is_terminating() const;
    /// Signal the queue to do it's destruction.
    void init_termination() const;
    /// Decrements the queue list and flags empty state.
    void update_queue_size() const;
    /// Query if the queue is empty.
    bool queue_empty() const;
    /// Set the queue_empty flag.
    void queue_empty(bool val) const;
    /// API to get and set contextual queue operation flags.
    done_t done;
  };
  /// API to query and set various flags
  flags_api_t flags;

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
  lists_api_t list;

  struct cv_api_t {
    std::condition_variable mutable queue;
    std::condition_variable mutable bind;
    std::condition_variable mutable eval;
    std::condition_variable mutable unbind;
    std::condition_variable mutable unbind_timeout;
  };
  /// Grouping of condition variables
  cv_api_t const cv{};

  std::atomic_bool is_dom_ready{};
  std::atomic_bool is_terminating{};
  std::atomic_bool queue_empty{};
  std::atomic_bool unbind_done{};
  std::atomic_bool unbind_can_proceed{};
  std::atomic_bool bind_done{};
  std::atomic_bool eval_done{};

  /// A thread to concurrently choreograph user work queueing.
  std::thread queue_thread;
};

} // namespace detail
} // namespace webview
#endif // defined(__cplusplus) && !defined(WEBVIEW_HEADER)
#endif // WEBVIEW_ENGINE_QUEUE_HH
