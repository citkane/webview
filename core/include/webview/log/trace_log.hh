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

#ifndef WEBVIEW_LOG_TRACE_LOG_HH
#define WEBVIEW_LOG_TRACE_LOG_HH

#if defined(__cplusplus) && !defined(WEBVIEW_HEADER)
#include "ansi_colours.hh"
#include <chrono>
#include <mutex>
#include <string>

#if defined(_WIN32)
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif // WIN32_LEAN_AND_MEAN
#ifdef _MSC_VER
#ifndef _WIN32_WINNT
#define _WIN32_WINNT 0x0501 //Win XP
#endif                      // _WIN32_WINNT
#endif                      // _MSC_VER
#include <io.h>
#include <windows.h>
#endif

namespace webview {
namespace types {
using str_arg_t = str_arg_t;
using time_point_t = std::chrono::time_point<std::chrono::steady_clock>;
} // namespace types
namespace log {
using namespace webview::types;
namespace _structs_classes {

struct trace_tools_t : _classes::ansi_t {
protected:
  time_point_t get_now() const;
  std::string get_ctx(char scp) const;
  long elapsed_ms(time_point_t start, time_point_t end) const;
  std::string bool_s(bool flag) const;
  std::string escape_s(str_arg_t text) const;
  std::string num_s(size_t val) const;
  std::string bold(str_arg_t this_col, str_arg_t text) const;
  std::string dim(str_arg_t this_col, str_arg_t text) const;
  void print_ansi(str_arg_t this_col, str_arg_t message) const;
};

class print_here_t : protected trace_tools_t {
public:
  ~print_here_t() = default;
  print_here_t(str_arg_t prefix, str_arg_t postfix)
      : prefix(prefix), postfix(postfix) {}
  void print_here(str_arg_t message) const;

private:
  std::string prefix;
  std::string postfix;
};

struct queue_print_t : public print_here_t {
public:
  queue_print_t(str_arg_t prefix, str_arg_t postfix)
      : print_here_t{prefix, postfix}, prefix{prefix}, postfix(postfix) {}
  void start(str_arg_t name) const;
  void wait(str_arg_t name) const;
  void done(bool done, str_arg_t name) const;

private:
  std::string prefix;
  std::string postfix;
};

class queue_bind_t {
public:
  ~queue_bind_t() = default;
  queue_bind_t(str_arg_t prefix, str_arg_t postfix = "BIND")
      : bind{prefix, postfix} {};

  struct wrapper_t : public queue_print_t {
    ~wrapper_t() = default;
    wrapper_t(str_arg_t prefix, str_arg_t postfix)
        : queue_print_t{prefix, postfix} {};
  } bind;
};

class queue_unbind_t {
public:
  ~queue_unbind_t() = default;
  queue_unbind_t(str_arg_t prefix, str_arg_t postfix = "UNBIND")
      : unbind{prefix, postfix} {};

  struct wrapper_t : public queue_print_t {
    ~wrapper_t() = default;
    wrapper_t(str_arg_t prefix, str_arg_t postfix)
        : queue_print_t{prefix, postfix} {};
  } unbind;
};

class queue_eval_t {
public:
  ~queue_eval_t() = default;
  queue_eval_t(str_arg_t prefix, str_arg_t postfix = "EVAL")
      : eval{prefix, postfix} {};

  struct wrapper_t : public print_here_t {
    ~wrapper_t() = default;
    wrapper_t(str_arg_t prefix, str_arg_t postfix)
        : print_here_t(prefix, postfix), prefix(prefix), postfix(postfix) {}
    void start(str_arg_t js) const;
    void done(bool done) const;

  private:
    std::string prefix;
    std::string postfix;
  } eval;
};

class queue_loop_t {
public:
  ~queue_loop_t() = default;
  queue_loop_t(str_arg_t prefix, str_arg_t postfix = "loop: ")
      : loop{prefix, postfix} {};

  struct wrapper_t : public print_here_t {
    ~wrapper_t() = default;
    wrapper_t(str_arg_t prefix, str_arg_t postfix)
        : print_here_t(prefix, postfix),
          prefix(prefix),
          postfix(postfix),
          process_ts(get_now()) {}
    void wait(size_t size, bool empty, bool dom_ready) const;
    void start(size_t size) const;
    void end() const;

  private:
    std::string prefix;
    std::string postfix;
    time_point_t process_ts;
    time_point_t mutable loop_wait_ts;
    time_point_t mutable loop_start_ts;
    time_point_t mutable loop_end_ts;

    void set_loop_wait_ts() const;
    void set_loop_start_ts() const;
    void set_loop_end_ts() const;
    std::string total_elapsed() const;
    std::string wait_elapsed() const;
    std::string loop_elapsed() const;

  } loop;
};

class queue_notify_t {
public:
  ~queue_notify_t() = default;
  queue_notify_t(str_arg_t prefix, str_arg_t postfix = "notify: ")
      : notify{prefix, postfix} {};

  struct wrapper_t : public print_here_t {
    ~wrapper_t() = default;
    wrapper_t(str_arg_t prefix, str_arg_t postfix)
        : print_here_t(prefix, postfix), prefix(prefix), postfix(postfix) {}
    void on_message(str_arg_t method) const;

  private:
    std::string prefix;
    std::string postfix;
  } notify;
};

class queue_enqueue_t {
public:
  ~queue_enqueue_t() = default;
  queue_enqueue_t(str_arg_t prefix, str_arg_t postfix = "enqueue: ")
      : enqueue{prefix, postfix} {};

  struct wrapper_t : public print_here_t {
    ~wrapper_t() = default;
    wrapper_t(str_arg_t prefix, str_arg_t postfix)
        : print_here_t(prefix, postfix), prefix(prefix), postfix(postfix) {}
    void added(char scp, size_t size, str_arg_t name_or_js) const;
    void added(char scp, size_t size) const;

  private:
    std::string prefix;
    std::string postfix;
  } enqueue;
};

class queue_trace_t : public queue_bind_t,
                      public queue_unbind_t,
                      public queue_eval_t,
                      public queue_loop_t,
                      public queue_notify_t,
                      public queue_enqueue_t,
                      public print_here_t {
public:
  ~queue_trace_t() = default;
  queue_trace_t(str_arg_t prefix, str_arg_t postfix = "QUEUE: ")
      : queue_bind_t{prefix + postfix},
        queue_unbind_t{prefix + postfix},
        queue_eval_t{prefix + postfix},
        queue_loop_t{prefix + postfix},
        queue_notify_t{prefix + postfix},
        queue_enqueue_t{prefix + postfix},
        print_here_t{prefix, postfix} {};
};

struct base_print_t : public print_here_t {
public:
  base_print_t(str_arg_t prefix, str_arg_t postfix)
      : print_here_t{prefix, postfix}, prefix{prefix}, postfix(postfix) {}
  void start(str_arg_t name) const;
  void work(str_arg_t name) const;
  void done(str_arg_t name) const;

private:
  std::string prefix;
  std::string postfix;
};

class base_bind_t {
public:
  ~base_bind_t() = default;
  base_bind_t(str_arg_t prefix, str_arg_t postfix = "bind: ")
      : bind{prefix, postfix} {};

  struct wrapper_t : public base_print_t {
    ~wrapper_t() = default;
    wrapper_t(str_arg_t prefix, str_arg_t postfix)
        : base_print_t{prefix, postfix} {};
  } bind;
};

class base_unbind_t {
public:
  ~base_unbind_t() = default;
  base_unbind_t(str_arg_t prefix, str_arg_t postfix = "unbind: ")
      : unbind{prefix, postfix} {};

  struct wrapper_t : public base_print_t {
    ~wrapper_t() = default;
    wrapper_t(str_arg_t prefix, str_arg_t postfix)
        : base_print_t{prefix, postfix} {};
  } unbind;
};

class base_eval_t {
public:
  ~base_eval_t() = default;
  base_eval_t(str_arg_t prefix, str_arg_t postfix = "eval: ")
      : eval{prefix, postfix} {};

  struct wrapper_t : public print_here_t {
    ~wrapper_t() = default;
    wrapper_t(str_arg_t prefix, str_arg_t postfix)
        : print_here_t{prefix, postfix}, prefix(prefix), postfix(postfix) {}
    void start(str_arg_t js, bool skip_queue) const;
    void work(str_arg_t js) const;
    void done(bool done, str_arg_t js) const;

  private:
    std::string prefix;
    std::string postfix;
  } eval;
};

class base_trace_t : public base_bind_t,
                     public base_unbind_t,
                     public base_eval_t,
                     public print_here_t {
public:
  ~base_trace_t() = default;
  base_trace_t(str_arg_t prefix, str_arg_t postfix = "BASE: ")
      : base_bind_t(prefix + postfix),
        base_unbind_t(prefix + postfix),
        base_eval_t(prefix + postfix),
        print_here_t(prefix, postfix) {}
};

} // namespace _structs_classes

using namespace _structs_classes;
class trace {
  ~trace() = default;
#if defined(_WIN32)
  trace() {
    AttachConsole(ATTACH_PARENT_PROCESS);
    static_cast<void>(freopen("CONOUT$", "w", stdout));
  };
#else
  trace() = default;
#endif
public:
  static const base_trace_t &base;
  static const queue_trace_t &queue;

private:
  static str_arg_t prefix() {
    static const std::string prefix_instance = "WEBVIEW: ";
    return prefix_instance;
  };
  static const base_trace_t &get_base() noexcept {
    static const base_trace_t base_instance = {prefix()};
    return base_instance;
  }
  static const queue_trace_t &get_queue() noexcept {
    static const queue_trace_t queue_instance = {prefix()};
    return queue_instance;
  }
};

} // namespace log
} // namespace webview

#endif // defined(__cplusplus) && !defined(WEBVIEW_HEADER)
#endif // WEBVIEW_LOG_TRACE_LOG_HH