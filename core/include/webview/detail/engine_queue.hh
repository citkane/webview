#ifndef WEBVIEW_ENGINE_QUEUE_HH
#define WEBVIEW_ENGINE_QUEUE_HH

#if defined(__cplusplus) && !defined(WEBVIEW_HEADER)
#ifndef WEBVIEW_UNBIND_TIMEOUT
#define WEBVIEW_UNBIND_TIMEOUT 20
#endif

#include "webview/detail/engine_base.hh"
#include <condition_variable>
#include <thread>
#include <unordered_map>

namespace webview {

// Container: Global header declarations
namespace detail {

/// enum definitions for \ref ctx, ie.
/// char 'b' = bind, 'u' = unbind, 'e' = eval
enum context_t { bind_t = 'b', unbind_t = 'u', eval_t = 'e' };
inline namespace ctx {
constexpr context_t bind = context_t::bind_t;
constexpr context_t unbind = context_t::unbind_t;
constexpr context_t eval = context_t::eval_t;
}; // namespace ctx

} // namespace detail

// Container: `engine_queue` class
namespace detail {

class engine_queue {
private:
  /* ************************************************************************
   * BEGIN private structures for the `engine_queue` public API
   **************************************************************************/

  template <typename T> struct api_base {
    T *s;
    api_base(T *self) : s(self) {}
  };

  struct promise_api_t : api_base<engine_queue> {
    promise_api_t(engine_queue *s);
    /// Initialises an empty list for promise id's of a given bind name
    void list_init(str_arg_t name) const;
    /// Takes queue action for a resolved promise
    void resolved(str_arg_t id) const;
  };

  struct bind_api_t : api_base<engine_queue> {
    bind_api_t(engine_queue *s);
    /// Puts a user `bind` work unit onto the queue.
    noresult enqueue(do_work_t fn, str_arg_t name) const;
  };

  struct unbind_api_t : api_base<engine_queue> {
    unbind_api_t(engine_queue *s);
    /// Puts a user `unbind` work unit onto the queue.
    noresult enqueue(do_work_t fn, str_arg_t name) const;
    /// Indicates to `unbind` if the name is queued to `bind`,
    /// in which case the future `unbind` is not an error.
    bool awaits_bind(str_arg_t name) const;
  };

  struct eval_api_t : api_base<engine_queue> {
    eval_api_t(engine_queue *s);
    /// Puts a user `eval` work unit onto the queue.
    noresult enqueue(do_work_t fn, str_arg_t js) const;
  };

  /* ************************************************************************
   * END private structures for the `engine_queue` public API
   **************************************************************************/
public:
  ~engine_queue() = default;
  explicit engine_queue(engine_base *base);

  /* ************************************************************************
   * Public API for `engine_queue`.
   *
   * Note:
   * These are constrained public interfaces to private internal functionality,
   * thus acceptable and idiomatic in C++ for API design.
   **************************************************************************/
  // NOLINTBEGIN(cppcoreguidelines-non-private-member-variables-in-classes)

  /// `engine_queue` promises api.
  promise_api_t const promises;
  /// `engine_queue` bind api.
  bind_api_t const bind;
  /// `engine_queue` unbind api.
  unbind_api_t const unbind;
  /// `engine_queue` eval api.
  eval_api_t const eval;

  // NOLINTEND(cppcoreguidelines-non-private-member-variables-in-classes)

  /// Sends the native work unit of a promise to a concurrent thread.
  void resolve(str_arg_t msg, std::map<std::string, binding_ctx_t> *bindings);
  /// @brief Cleans up and shuts down the queue thread.
  ///
  /// This is the only instance where we lock the main / app thread.
  /// We do so to firstly join the queue thread otherwise Webview will segfault.
  void terminate();

private:
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
  void queue_thread_constructor();
  /// A thread to concurrently choreograph user work queueing.
  std::thread queue_thread;
  /// Constructs a child thread for each native work unit of a bound JS promise.
  /// We want native promise work units to run concurrently.
  /// We do not want native promise work to stall execution of the main / app thread.
  void
  resolve_thread_constructor(str_arg_t name, str_arg_t id, str_arg_t msg,
                             std::map<std::string, binding_ctx_t> *bindings);
  /// Adds `bind`, `unbind` or `eval` user work unit to the queue.
  noresult enqueue_work(str_arg_t name_or_js, do_work_t fn,
                        context_t fn_ctx) const;
  /// Relays notifications from the frontend to relevant queue methods.
  bool frontend_notification(str_arg_t id, str_arg_t method);
  /// Flags a `bind`, `unbind` or `eval` user work unit as  done.
  void set_done(bool val, context_t fn_ctx);
  /// Checks if `unbind` is referencing an earlier `bind` function that has not yet completed.
  bool unbind_awaiting_bind(str_arg_t name);
  /// Initialises an empty list of potential promises for a bound function name.
  void promise_list_init(str_arg_t name);
  /// Removes a resolved promise from a bound function's register
  void promise_erase(str_arg_t id) const;

  std::atomic_bool is_terminating{};
  std::atomic_bool queue_empty{};
  std::atomic_bool is_dom_ready{};
  std::atomic_bool unbind_done{};
  std::atomic_bool unbind_can_proceed{};
  std::atomic_bool bind_done{};
  std::atomic_bool eval_done{};
  /// \ref flags_api_t.done
  struct done_t : api_base<engine_queue> {
    done_t(engine_queue *s);
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
  /// API to query and set various flags
  struct flags_api_t : api_base<engine_queue> {
    flags_api_t(engine_queue *s);
    /// Query if the backend is ready to do work.
    bool get_dom_ready();
    /// Notify the queue that the backend is ready to receive work.
    void set_dom_ready();
    /// Query if Webview is in the process of terminating.
    bool get_terminating();
    /// Signal the queue to do it's destruction.
    void set_terminating();
    /// Decrements the queue list and flags empty state.
    void update_queue_size();
    /// Query if the queue is empty.
    bool queue_empty();
    /// API to get and set contextual queue operation flags.
    done_t done;
  };
  /// API for working with flags
  flags_api_t flags;
  struct cv_api_t {
    std::condition_variable queue;
    std::condition_variable bind;
    std::condition_variable eval;
    std::condition_variable unbind;
    std::condition_variable unbind_timeout;
  };
  /// Grouping of condition variables
  static cv_api_t cv;
  /// Structure for the \ref list.queue value
  struct action_t {
    context_t ctx;
    do_work_t fn;
    std::string val;
  };
  struct lists_api_t {
    /// The ordered list of user work units to process
    std::list<action_t> queue;
    /// A map keyed by bound function name containing a list it's unresolved promise id's
    std::unordered_map<std::string, std::list<std::string>> name_unres_promises;
    /// A map keyed by promise id referencing it's corresponding `bind` name
    std::unordered_map<std::string, std::string> promise_id_name;
    /// A list of queued `bind` names;
    std::list<std::string> pending_binds;
  };
  /// Grouping of list-like values
  static lists_api_t list;
  /// Reference to the `engine_base` class instance.
  engine_base *base;

  /// Temporary debug tracing utility
  /// @todo remove before merge
  trace_t trace = trace_t("Webview::");
}; // namespace detail

} // namespace detail
} // namespace webview
#endif // defined(__cplusplus) && !defined(WEBVIEW_HEADER)
#endif // WEBVIEW_ENGINE_QUEUE_HH