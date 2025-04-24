#ifndef WEBVIEW_ENGINE_DEBUG_HH
#define WEBVIEW_ENGINE_DEBUG_HH

#include <string>
#if defined(__cplusplus) && !defined(WEBVIEW_HEADER)

static const char *bool_str(bool val) { return val ? "true" : "false"; }
//col_t col() { return col_t{}; };
struct col_t {
  const char *dim = "\033[2m";
  const char *yellow = "\033[33m";
  const char *green = "\033[32m";
  const char *reset = "\033[0m";
};
struct queue_t {
  queue_t(std::string prefix)
      : notify(prefix + postfix),
        enqueue(prefix + postfix),
        bind(prefix + postfix),
        unbind(prefix + postfix),
        eval(prefix + postfix),
        loop(prefix + postfix) {}
  std::string postfix = "queue: ";

  struct notify_t {
    notify_t(std::string prefix) : prefix_(prefix + "notify: ") {}
    std::string prefix_;
    const char *prefix = prefix_.c_str();
    void on_notify(const char *method) const {
      printf("%son_message: %s\n", prefix, method);
    }
  } notify;

  struct enqueue_t {
    enqueue_t(std::string prefix) : prefix_(prefix + "enqueue: ") {}
    std::string prefix_;
    const char *prefix = prefix_.c_str();
    void added(const char *name) const {
      printf("%sadded: %s\n", prefix, name);
    }
    void added(char scp, size_t size) const {
      printf("%sadded: %c | %zu\n", prefix, scp, size);
    }
  } enqueue;

  struct bind_t {
    bind_t(std::string prefix) : prefix_(prefix + "bind: ") {};
    std::string prefix_;
    const char *prefix = prefix_.c_str();
    void start() const { printf("%sstart\n", prefix); };
    void done(bool done) const {
      printf("%sdone: %s\n", prefix, bool_str(done));
    }
  } bind;
  struct unbind_t {
    unbind_t(std::string prefix) : prefix_(prefix + "unbind: ") {};
    std::string prefix_;
    const char *prefix = prefix_.c_str();
    void wait() const { printf("%swait\n", prefix); };
    void start() const { printf("%sstart\n", prefix); };
    void done(bool done) const {
      printf("%sdone: %s\n", prefix, bool_str(done));
    }
  } unbind;
  struct eval_t {
    eval_t(std::string prefix) : prefix_(prefix + "eval: ") {};
    std::string prefix_;
    const char *prefix = prefix_.c_str();
    void start() const { printf("%sstart\n", prefix); };
    void done(bool done) const {
      printf("%sdone: %s\n", prefix, bool_str(done));
    }
  } eval;
  struct loop_t {
    loop_t(std::string prefix) : prefix_(prefix + "loop: ") {};
    std::string prefix_;
    col_t col;
    const char *prefix = prefix_.c_str();
    void wait(size_t size, bool empty, bool dom_ready) const {
      printf("\n%s%swaiting: %zu | queue_empty: %s | dom_ready: "
             "%s%s\n",
             col.dim, prefix, size, bool_str(empty), bool_str(dom_ready),
             col.reset);
    };
    void start(size_t size) const {
      printf("\n%s%sstart: %zu%s\n", col.green, prefix, size, col.reset);
    };
    void end(size_t size) const {
      printf("%s%send: %zu%s\n", col.green, prefix, size, col.reset);
    }
  } loop;
};
struct base_t {
  base_t(std::string prefix)
      : bind(prefix + postfix),
        unbind(prefix + postfix),
        eval(prefix + postfix) {};
  std::string postfix = "base: ";
  struct bind_t {
    bind_t(std::string prefix) : prefix_(prefix + "bind: ") {};
    std::string prefix_;
    col_t col;
    const char *prefix = prefix_.c_str();
    void start(const char *name) const {
      printf("%s%sregister: %s%s\n", col.dim, prefix, name, col.reset);
    };
    void work(const char *name) const {
      printf("%s%swork: %s%s\n", col.yellow, prefix, name, col.reset);
    };
    void done(const char *name) const {
      printf("%s%sdone: %s%s\n", col.green, prefix, name, col.reset);
    }
  } bind;
  struct unbind_t {
    unbind_t(std::string prefix) : prefix_(prefix + "unbind: ") {};
    std::string prefix_;
    col_t col;
    const char *prefix = prefix_.c_str();
    void start(const char *name) const {
      printf("%s%sregister: %s%s\n", col.dim, prefix, name, col.reset);
    };
    void work(const char *name) const {
      printf("%s%swork: %s%s\n", col.yellow, prefix, name, col.reset);
    };
    void done(const char *name) const {
      printf("%s%sdone: %s%s\n", col.green, prefix, name, col.reset);
    }
  } unbind;
  struct eval_t {
    eval_t(std::string prefix) : prefix_(prefix + "eval: ") {};
    std::string prefix_;
    col_t col;
    const char *prefix = prefix_.c_str();
    void start(const char *js, bool skip_queue) const {
      printf("\n%s%sregister: skip: %s\n%s%s\n\n", col.dim, prefix,
             bool_str(skip_queue), js, col.reset);
    };
    void work(const char *js) const {
      printf("\n%s%swork\n%s%s\n\n", col.yellow, prefix, js, col.reset);
    };
    void done(bool done) const {
      printf("%s%sdone: %s%s\n", col.green, prefix, bool_str(done), col.reset);
    }
  } eval;
};
struct debug_t {
  debug_t(std::string prefix) : queue(prefix), base(prefix) {};
  queue_t queue;
  base_t base;
  col_t col = col_t{};
};

#endif // defined(__cplusplus) && !defined(WEBVIEW_HEADER)
#endif // WEBVIEW_ENGINE_DEBUG_HH