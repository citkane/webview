#ifndef WEBVIEW_ENGINE_QUEUE_HH
#define WEBVIEW_ENGINE_QUEUE_HH

#if defined(__cplusplus) && !defined(WEBVIEW_HEADER)
#ifndef WEBVIEW_UNBIND_TIMEOUT
#define WEBVIEW_UNBIND_TIMEOUT 20
#endif

#include "webview/detail/engine_base.hh"
#include <condition_variable>
#include <mutex>
#include <thread>
#include <unordered_map>

namespace webview {
namespace detail {

class engine_queue {
  template <typename T> struct api_base {
    T *s;
    api_base(T *self) : s(self) {}
  };

  struct promise_api_t : api_base<engine_queue> {
    promise_api_t(engine_queue *s) : api_base(s) {};
    void list_init(str_arg_t name) const { s->promise_list_init(name); };
    void add(str_arg_t name, str_arg_t id) const { s->promises_add(name, id); };
    void erase(str_arg_t id) const { s->promise_erase(id); };
    void flag_bind(str_arg_t name) const { s->promise_flag_bind(name); };
  };

  struct bind_api_t : api_base<engine_queue> {
    bind_api_t(engine_queue *s) : api_base(s) {};
    noresult enqueue(do_work_t fn, str_arg_t name) const {
      return s->enqueue_work(name, fn, s->scope.bind);
    };
    void set_done(bool val) const { s->set_done(val, s->scope.bind); };
    bool get_done() const { return s->flags.done.bind.load(); }
  };

  struct unbind_api_t : api_base<engine_queue> {
    unbind_api_t(engine_queue *s) : api_base(s) {};
    noresult enqueue(do_work_t fn, str_arg_t name) const {
      return s->enqueue_work(name, fn, s->scope.unbind);
    };
    void set_done(bool val) const { s->set_done(val, s->scope.unbind); };
    bool get_done() const { return s->flags.done.unbind.load(); };
    bool awaits_bind(str_arg_t name) const {
      return s->unbind_awaiting_bind(name);
    };
  };

  struct eval_api_t : api_base<engine_queue> {
    eval_api_t(engine_queue *s) : api_base(s) {};
    noresult enqueue(do_work_t fn) const {
      return s->enqueue_work(fn, s->scope.eval);
    };
    void set_done(bool val) const { s->set_done(val, s->scope.eval); };
    bool get_done() const { return s->flags.done.eval.load(); }
  };

public:
  explicit engine_queue(engine_base *engine);
  ~engine_queue() = default;

  // NOLINTBEGIN(cppcoreguidelines-non-private-member-variables-in-classes)
  //
  // These are constrained public interfaces to internal functionality,
  // thus acceptable and idiomatic in C++ for API design

  /// `engine_queue` promises api.
  promise_api_t const promises{this};
  /// `engine_queue` bind api.
  bind_api_t const bind{this};
  /// `engine_queue` unbind api.
  unbind_api_t const unbind{this};
  /// `engine_queue` eval api.
  eval_api_t const eval{this};

  // NOLINTEND(cppcoreguidelines-non-private-member-variables-in-classes)

  void notify(str_arg_t method);
  void resolve(binding_ctx_t *ctx, str_arg_t name, str_arg_t id, str_arg_t msg);
  void terminate();
  void set_dom_ready();
  void register_unresolved_binds(str_arg_t js);

private:
  template <typename T = std::string> using list_t = std::vector<T>;
  enum scope_t { bind_t = 'b', unbind_t = 'u', eval_t = 'e' };
  struct scope_api_t {
    scope_t bind = bind_t;
    scope_t unbind = unbind_t;
    scope_t eval = eval_t;
  };
  /// A private nested type for managing scopes
  scope_api_t scope{};

  std::atomic_bool is_terminating{};
  std::atomic_bool queue_empty{};
  std::atomic_bool is_dom_ready{};
  struct flags_api_t : api_base<engine_queue> {
    flags_api_t(engine_queue *s) : api_base(s) {};
    bool get_dom_ready() { return s->is_dom_ready.load(); };
    void set_dom_ready() {
      s->is_dom_ready.store(true);
      engine_queue::cv.queue.notify_one();
    };
    bool get_terminating() { return s->is_terminating.load(); };
    void set_terminating() {
      s->is_terminating.store(true);
      s->queue_empty.store(false);
      s->is_dom_ready.store(true);
      s->bind.set_done(true);
      s->unbind.set_done(true);
      s->eval.set_done(true);
      engine_queue::cv.queue.notify_one();
      //engine_queue::cv.bind.notify_one();
      //engine_queue::cv.eval.notify_one();
      //engine_queue::cv.unbind.notify_one();
      //engine_queue::cv.unbind_timeout.notify_one();
    };
    void set_queue_empty() {
      if (list.queue.size() > 1) {
        list.queue.pop_front();
      } else {
        list.queue.clear();
      }
      s->queue_empty.store(list.queue.empty());
      //cv.queue.notify_all();
    }
    bool get_queue_empty() { return s->queue_empty.load(); };
    struct done_t {
      std::atomic_bool unbind{};
      std::atomic_bool bind{};
      std::atomic_bool eval{};
    } done{};
  };
  /// A private internal API for working with flags
  flags_api_t flags{this};

  struct cv_api_t {
    std::condition_variable queue;
    std::condition_variable bind;
    std::condition_variable eval;
    std::condition_variable unbind;
    std::condition_variable unbind_timeout;
  };
  /// A private internal API for managing condition variables
  static cv_api_t cv;

  typedef std::pair<scope_t, do_work_t> action_t;
  struct lists_api_t {
    /// A map of bound function name / promise id's
    std::unordered_map<std::string, list_t<>> bind_promise_ids;
    /// A map of promise id / bound function name
    std::unordered_map<std::string, std::string> promise_id_bind;
    /// The ordered list of user actions to process
    std::list<action_t> queue;
    /// The ordered list of queued bind names;
    std::list<std::string> pending_binds;
    /// The ordered list of queued unbind names;
    std::list<std::string> pending_unbinds;
    /// A list of bound function names that have promises pending
    list_t<> pending_promise_binds;
    /// Checks if a list contains a value
    bool contains(list_t<> &targ_list, str_arg_t value);
    /// Erases a value from the given list
    void erase(list_t<> &targ_list, str_arg_t value);
    /// Adds a unique value to the given list
    void add_unique(list_t<> &targ_list, str_arg_t value);
    /// Adds a value to the given list
    template <typename T = std::string>
    void add(list_t<T> &targ_list, const T &value);
  };
  /// A private internal API for managing lists
  static lists_api_t list;

  typedef std::unique_lock<std::mutex> lock_t;

  std::thread queue_thread =
      std::thread(&engine_queue::queue_thread_constructor, this);

  engine_base *engine;

  noresult enqueue_work(str_arg_t name, do_work_t fn, scope_t scope) const;
  noresult enqueue_work(do_work_t fn, scope_t scope) const;
  void set_done(bool val, scope_t scope);

  /// Creates a child thread for each native work unit of a bound JS promise.
  /// We want native promise work units to run concurrently.
  /// We do not want native promise work units to stall execution of the main/app thread.
  void resolve_thread_constructor(binding_ctx_t *ctx, str_arg_t name,
                                  str_arg_t id, str_arg_t msg);

  /// Creates a thread to manage the the coordinated execution of user work units
  /// We want this to happen in the correct order, eg:
  /// - `unbind` will first give unresolved promises opportunity to resolve.
  /// - `eval` will give `bind` chance to register before evaluating
  /// We do not want ordering to stall execution of the main/app thread.
  void queue_thread_constructor();

  bool unbind_awaiting_bind(str_arg_t name);

  void promise_list_init(str_arg_t name);
  void promises_add(str_arg_t name, str_arg_t id);
  void promise_erase(str_arg_t id);
  void promise_flag_bind(str_arg_t name);

  debug_t debug = debug_t("Webview: ");
};

} // namespace detail
} // namespace webview
#endif // defined(__cplusplus) && !defined(WEBVIEW_HEADER)
#endif // WEBVIEW_ENGINE_QUEUE_HH