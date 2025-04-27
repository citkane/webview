#ifndef WEBVIEW_ENGINE_TRACE_HH
#define WEBVIEW_ENGINE_TRACE_HH

#if defined(__cplusplus) && !defined(WEBVIEW_HEADER)

#include <chrono>
#include <string>

namespace webview {
namespace detail {
namespace utility {
using string_t = std::string;
using namespace std::chrono;
using time_point_t = time_point<steady_clock>;

static long elapsed_ms(time_point_t start, time_point_t end) {
  return duration_cast<milliseconds>(end - start).count();
};
static string_t bool_s(bool flag) { return flag ? "true" : "false"; }
static string_t escape_s(string_t text) { return "\"" + text + "\""; }
static string_t num_s(size_t val) { return std::to_string(val); }
static string_t get_ctx(char scp) {
  if (scp == 'b') {
    return "bind";
  }
  if (scp == 'u') {
    return "unbind";
  }
  return "eval";
}

static string_t ansi(int code) { return "\033[" + std::to_string(code) + "m"; }
static string_t reset() { return ansi(0); }
static string_t bold(string_t col, string_t text) {
  return ansi(1) + text + reset() + col;
}
static string_t dim(string_t col, string_t text) {
  return ansi(2) + text + reset() + col;
}

struct col_t {
  string_t yellow = ansi(33);
  string_t yellow_dim = ansi(33) + ansi(2);
  string_t green = ansi(92);
  string_t red = ansi(91);
  string_t blue = ansi(94);
  string_t blue_dark = ansi(34);
  string_t magenta = ansi(95);
  string_t default_c = reset();
};

void printf_col(string_t colour_string, string_t message) {
  printf("%s%s%s\n", colour_string.c_str(), message.c_str(), reset().c_str());
};
} // namespace utility

using namespace utility;
// NOLINTBEGIN(cppcoreguidelines-non-private-member-variables-in-classes)
class trace_t;

class queue_t {
  string_t postfix = "QUEUE: ";
  class bind_t {
    string_t postfix = "BIND";

  public:
    ~bind_t() = default;
    bind_t(string_t prefix, queue_t *queue) : prefix(prefix), queue(queue) {};

    void wait(string_t name) const {
      queue->print_wait(prefix, postfix, name);
    };
    void start(string_t name) const {
      queue->print_start(prefix, postfix, name);
    };
    void done(bool done, string_t name) const {
      queue->print_done(prefix, postfix, done, name);
    }

  private:
    string_t prefix;
    queue_t *queue;
  };
  class unbind_t {
    string_t postfix = "UNBIND";

  public:
    ~unbind_t() = default;
    unbind_t(string_t prefix, queue_t *queue) : prefix(prefix), queue(queue) {};

    void wait(string_t name) const {
      queue->print_wait(prefix, postfix, name);
    };
    void start(string_t name) const {
      queue->print_start(prefix, postfix, name);
    };
    void done(bool done, string_t name) const {
      queue->print_done(prefix, postfix, done, name);
    }

  private:
    string_t prefix;
    queue_t *queue;
  };
  class eval_t {
    string_t postfix = "EVAL";

  public:
    ~eval_t() = default;
    eval_t(string_t prefix) : prefix(prefix) {};

    void start(string_t js) const {
      auto this_c = col.blue;
      auto postfix_m = bold(this_c, postfix) + ": ";
      auto start_m = bold(this_c, "START") + " js ...\n";
      auto js_m = dim(this_c, js);
      printf_col(this_c, prefix + postfix_m + start_m + js_m);
    };
    void done(bool done) const {
      auto this_c = col.blue;
      auto postfix_m = bold(this_c, postfix) + ": ";
      auto done_m = bold(this_c, "DONE: ") + bool_s(done);
      printf_col(this_c, prefix + postfix_m + done_m);
    }

  private:
    string_t prefix;
    col_t col{};
  };
  class loop_t {
    string_t postfix = "loop: ";

  public:
    ~loop_t() = default;
    loop_t(string_t prefix)
        : process_ts(steady_clock::now()),
          loop_wait_ts(steady_clock::now()),
          loop_start_ts(steady_clock::now()),
          loop_end_ts(steady_clock::now()),
          prefix(prefix + postfix) {};

    void wait(size_t size, bool empty, bool dom_ready) const {
      set_loop_wait_ts();
      auto this_col = col.green;
      auto waiting = bold(this_col, "WAITING ");
      auto size_s = "| queue size: " + std::to_string(size) + " ";
      auto empty_s = "| queue_empty: " + bool_s(empty) + " ";
      auto dom_s = "| dom_ready: " + bool_s(dom_ready) + " ";
      auto time_m = "| total process time: " + total_elapsed() + " ";
      if (is_init) {
        printf("\n");
      }
      printf_col(this_col,
                 prefix + waiting + time_m + size_s + empty_s + dom_s);
      if (is_init) {
        printf("\n");
      }
      is_init = false;
    };
    void start(size_t size) const {
      set_loop_start_ts();
      auto this_col = col.green;
      auto start = bold(this_col, "START ");
      auto time_m = "| waited for: " + wait_elapsed() + " ";
      auto size_m = "| queue size: " + std::to_string(size);

      printf_col(this_col, prefix + start + time_m + size_m + "\n");
    };
    void end() const {
      set_loop_end_ts();
      auto this_col = col.green;
      auto end = bold(this_col, "END ");
      auto time_m = "| loop process time: " + loop_elapsed();
      printf_col(this_col, "\n" + prefix + end + time_m);
    }

  private:
    time_point_t process_ts;
    time_point_t mutable loop_wait_ts;
    time_point_t mutable loop_start_ts;
    time_point_t mutable loop_end_ts;
    string_t prefix;
    col_t col{};
    bool mutable is_init{true};

    void set_loop_wait_ts() const { loop_wait_ts = steady_clock::now(); }
    void set_loop_start_ts() const { loop_start_ts = steady_clock::now(); }
    void set_loop_end_ts() const { loop_end_ts = steady_clock::now(); }
    string_t total_elapsed() const {
      auto elapsed = elapsed_ms(process_ts, loop_wait_ts);
      return std::to_string(elapsed) + "ms";
    }
    string_t wait_elapsed() const {
      auto elapsed = elapsed_ms(loop_wait_ts, loop_start_ts);
      return std::to_string(elapsed) + "ms";
    }
    string_t loop_elapsed() const {
      auto elapsed = elapsed_ms(loop_start_ts, loop_end_ts);
      return std::to_string(elapsed) + "ms";
    }
  };
  class notify_t {
  public:
    ~notify_t() = default;
    notify_t(string_t prefix) : prefix(prefix + "notify: ") {}

    void on_message(string_t method) const {
      auto this_c = col.yellow_dim;
      auto mess_m = bold(this_c, "on_message") + ": ";
      auto method_m = escape_s(method);
      printf_col(this_c, prefix + mess_m + method_m);
    }

  private:
    string_t prefix;
    col_t col{};
  };
  class enqueue_t {
  public:
    ~enqueue_t() = default;
    enqueue_t(string_t prefix) : prefix(prefix + "enqueue: ") {}

    void added(char scp, size_t size, string_t name_or_js) const {
      auto this_c = col.default_c;
      auto ctx_m = get_ctx(scp);
      auto size_m = "queue size: " + num_s(size) + " | ";
      auto queued_m = bold(this_c, "queueing " + ctx_m) + " | ";
      auto name_or_js_m = scp == 'e' ? "got js ...\n" + dim(this_c, name_or_js)
                                     : escape_s(name_or_js);
      printf_col(this_c, prefix + queued_m + size_m + name_or_js_m);
    }
    void added(char scp, size_t size) const {
      auto this_c = col.default_c;
      auto ctx_m = get_ctx(scp);
      auto size_m = "queue size: " + num_s(size) + " | ";
      auto queued_m = bold(this_c, "queueing " + ctx_m);
      printf_col(this_c, prefix + size_m + queued_m);
    }

  private:
    string_t prefix;
    col_t col{};
  };

public:
  ~queue_t() = default;
  queue_t(string_t prefix)
      : bind(prefix + postfix, this),
        unbind(prefix + postfix, this),
        eval(prefix + postfix),
        loop(prefix + postfix),
        notify(prefix + postfix),
        enqueue(prefix + postfix),
        prefix(prefix + postfix) {}

  bind_t bind;
  unbind_t unbind;
  eval_t eval;
  loop_t loop;
  notify_t notify;
  enqueue_t enqueue;
  /// Flexible print tool to highlight a point in code,
  void print_here(string_t message) const {
    auto this_c = col.magenta;
    auto here_m = bold(this_c, "here") + ": ";
    message = col.default_c + message;
    printf_col(this_c, prefix + here_m + message);
  };

private:
  string_t prefix;
  col_t col{};

  void print_wait(string_t prefix, string_t postfix, string_t name) const {
    col_t col;
    auto this_c = col.yellow_dim;
    auto postfix_m = bold(this_c, postfix) + ": ";
    auto wait_m = bold(this_c, "WAIT: ") + escape_s(name);
    printf_col(this_c, prefix + postfix_m + wait_m);
  }
  void print_start(string_t prefix, string_t postfix, string_t name) const {
    col_t col;
    auto this_c = col.blue;
    auto postfix_m = bold(this_c, postfix) + ": ";
    auto start_m = bold(this_c, "START: ") + escape_s(name);
    printf_col(this_c, prefix + postfix_m + start_m);
  }
  void print_done(string_t prefix, string_t postfix, bool done,
                  string_t name) const {
    col_t col;
    auto this_c = col.blue;
    auto postfix_m = bold(this_c, postfix) + ": ";
    auto done_m = bold(this_c, "work_done: ") + bool_s(done) + ": ";
    auto name_m = escape_s(name);
    printf_col(this_c, prefix + postfix_m + done_m + name_m);
  }
};
class base_t {
  string_t postfix = "BASE: ";

  class bind_t {
    string_t postfix = "bind: ";

  public:
    ~bind_t() = default;
    bind_t(string_t prefix, base_t *base)
        : prefix(prefix + postfix), base(base) {};

    void start(string_t name) const { base->print_start(prefix, name); };
    void work(string_t name) const { base->print_work(prefix, name); };
    void done(string_t name) const { base->print_done(prefix, name); }

  private:
    string_t prefix;
    base_t *base;
  };
  class unbind_t {
    string_t postfix = "unbind: ";

  public:
    ~unbind_t() = default;
    unbind_t(string_t prefix, base_t *base)
        : prefix(prefix + postfix), base(base) {};

    void start(string_t name) const { base->print_start(prefix, name); };
    void work(string_t name) const { base->print_work(prefix, name); };
    void done(string_t name) const { base->print_done(prefix, name); }

  private:
    string_t prefix;
    base_t *base;
  };
  class eval_t {
    string_t postfix = "eval: ";

  public:
    eval_t(string_t prefix) : prefix(prefix + postfix) {};
    ~eval_t() = default;

    void start(string_t js, bool skip_queue) const {
      auto this_c = col.default_c;
      auto skip_m = "skip queue: " + bool_s(skip_queue) + " | ";
      auto received_m = bold(this_c, "received js") + " ...\n";
      auto m = prefix + skip_m + received_m + dim(this_c, js);
      printf_col(this_c, m);
    };
    void work(string_t js) const {
      auto this_c = col.blue_dark;
      auto work_m = bold(this_c, "do_work") + " js...\n";
      auto m = prefix + work_m + dim(this_c, js);
      printf_col(this_c, m);
    };
    void done(bool done, string_t js) const {
      auto this_c = col.blue;
      auto done_m =
          bold(this_c, "work done") + ": " + bool_s(done) + " | js ...\n";
      auto js_m = dim(this_c, js);
      printf_col(this_c, prefix + done_m + js_m);
    }

  private:
    string_t prefix;
    col_t col{};
  };

public:
  base_t(string_t prefix)
      : bind(prefix + postfix, this),
        unbind(prefix + postfix, this),
        eval(prefix + postfix),
        prefix(prefix + postfix) {};
  ~base_t() = default;

  bind_t bind;
  unbind_t unbind;
  eval_t eval;
  /// Flexible print tool to highlight a point in code,
  void print_here(string_t message) const {
    auto this_c = col.magenta;
    auto here_m = bold(this_c, "here") + ": ";
    message = col.default_c + message;
    printf_col(this_c, prefix + here_m + message);
  };

private:
  string_t prefix;
  col_t col{};

  void print_start(string_t prefix, string_t name) const {
    auto this_c = col.default_c;
    auto name_m = escape_s(name);
    auto got_m = bold(this_c, "got") + ": ";
    printf_col(this_c, prefix + got_m + name_m);
  }
  void print_work(string_t prefix, string_t name) const {
    auto this_c = col.blue_dark;
    auto work_m = bold(this_c, "do_work: ");
    auto name_m = escape_s(name);
    printf_col(this_c, prefix + work_m + name_m);
  }
  void print_done(string_t prefix, string_t name) const {
    auto this_c = col.blue;
    auto done_m = bold(this_c, "work done") + ": ";
    auto name_m = escape_s(name);
    printf_col(this_c, prefix + done_m + name_m);
  }
};

class trace_t {
public:
  trace_t(string_t prefix) : queue(prefix), base("--- " + prefix) {};
  ~trace_t() = default;

  queue_t queue;
  base_t base;
};
// NOLINTEND(cppcoreguidelines-non-private-member-variables-in-classes)

} // namespace detail
} // namespace webview

#endif // defined(__cplusplus) && !defined(WEBVIEW_HEADER)
#endif // WEBVIEW_ENGINE_TRACE_HH