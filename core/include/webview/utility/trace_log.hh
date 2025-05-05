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

#ifndef WEBVIEW_UTILITY_TRACE_LOG_HH
#define WEBVIEW_UTILITY_TRACE_LOG_HH

#if defined(__cplusplus) && !defined(WEBVIEW_HEADER)

#include <chrono>
#include <string>

namespace webview {
namespace utility {
using time_point_t = std::chrono::time_point<std::chrono::steady_clock>;
using string_t = const std::string &;

inline time_point_t get_now() { return std::chrono::steady_clock::now(); }

class col {
public:
  static const std::string &yellow();
  static const std::string &yellow_dim();
  static const std::string &green();
  static const std::string &red();
  static const std::string &blue();
  static const std::string &blue_dark();
  static const std::string &magenta();
  static const std::string &default_c();
};

class print_here_t {
public:
  ~print_here_t() = default;
  print_here_t(string_t prefix, string_t postfix);
  void print_here(string_t message) const;

private:
  std::string prefix;
  std::string postfix;
};

// NOLINTBEGIN(cppcoreguidelines-non-private-member-variables-in-classes)
namespace queue_api {

struct queue_print_t : public print_here_t {
public:
  queue_print_t(string_t prefix, string_t postfix)
      : print_here_t{prefix, postfix}, prefix{prefix}, postfix(postfix) {};
  void start(string_t name) const;
  void wait(string_t name) const;
  void done(bool done, string_t name) const;

private:
  std::string prefix;
  std::string postfix;
};

class queue_bind_t {
public:
  ~queue_bind_t() = default;
  queue_bind_t(string_t prefix, string_t postfix = "BIND")
      : bind{prefix, postfix} {};
  struct wrapper_t : public queue_print_t {
    ~wrapper_t() = default;
    wrapper_t(string_t prefix, string_t postfix)
        : queue_print_t{prefix, postfix} {};
  } bind;
};

class queue_unbind_t {
public:
  ~queue_unbind_t() = default;
  queue_unbind_t(string_t prefix, string_t postfix = "UNBIND")
      : unbind{prefix, postfix} {};
  struct wrapper_t : public queue_print_t {
    ~wrapper_t() = default;
    wrapper_t(string_t prefix, string_t postfix)
        : queue_print_t{prefix, postfix} {};
  } unbind;
};

class queue_eval_t {
public:
  ~queue_eval_t() = default;
  queue_eval_t(string_t prefix, string_t postfix = "EVAL")
      : eval{prefix, postfix} {};
  struct wrapper_t : public print_here_t {
    ~wrapper_t() = default;
    wrapper_t(string_t prefix, string_t postfix)
        : print_here_t(prefix, postfix), prefix(prefix), postfix(postfix) {};
    void start(string_t js) const;
    void done(bool done) const;

  private:
    std::string prefix;
    std::string postfix;
  } eval;
};

class queue_loop_t {
public:
  ~queue_loop_t() = default;
  queue_loop_t(string_t prefix, string_t postfix = "loop: ")
      : loop{prefix, postfix} {};
  struct wrapper_t : public print_here_t {
    ~wrapper_t() = default;
    wrapper_t(string_t prefix, string_t postfix)
        : print_here_t(prefix, postfix),
          prefix(prefix),
          postfix(postfix),
          process_ts(get_now()) {};
    void wait(size_t size, bool empty, bool dom_ready) const;
    void start(size_t size) const;
    void end() const;

  private:
    std::string prefix;
    std::string postfix;
    time_point_t process_ts;

    void set_loop_wait_ts() const;
    void set_loop_start_ts() const;
    void set_loop_end_ts() const;
    std::string total_elapsed() const;
    std::string wait_elapsed() const;
    std::string loop_elapsed() const;

    time_point_t mutable loop_wait_ts;
    time_point_t mutable loop_start_ts;
    time_point_t mutable loop_end_ts;

  } loop;
};

class queue_notify_t {
public:
  ~queue_notify_t() = default;
  queue_notify_t(string_t prefix, string_t postfix = "notify: ")
      : notify{prefix, postfix} {};
  struct wrapper_t : public print_here_t {
    ~wrapper_t() = default;
    wrapper_t(string_t prefix, string_t postfix)
        : print_here_t(prefix, postfix), prefix(prefix), postfix(postfix) {};
    void on_message(string_t method) const;

  private:
    std::string prefix;
    std::string postfix;
  } notify;
};

class queue_enqueue_t {
public:
  ~queue_enqueue_t() = default;
  queue_enqueue_t(string_t prefix, string_t postfix = "enqueue: ")
      : enqueue{prefix, postfix} {};
  struct wrapper_t : public print_here_t {
    ~wrapper_t() = default;
    wrapper_t(string_t prefix, string_t postfix)
        : print_here_t(prefix, postfix), prefix(prefix), postfix(postfix) {};
    void added(char scp, size_t size, string_t name_or_js) const;
    void added(char scp, size_t size) const;

  private:
    std::string prefix;
    std::string postfix;
  } enqueue;
};

} // namespace queue_api

namespace base_api {

struct base_print_t : public print_here_t {
public:
  base_print_t(string_t prefix, string_t postfix)
      : print_here_t{prefix, postfix}, prefix{prefix}, postfix(postfix) {};
  void start(string_t name) const;
  void work(string_t name) const;
  void done(string_t name) const;

private:
  std::string prefix;
  std::string postfix;
};

class base_bind_t {
public:
  ~base_bind_t() = default;
  base_bind_t(string_t prefix, string_t postfix = "bind: ")
      : bind{prefix, postfix} {};
  struct wrapper_t : public base_print_t {
    ~wrapper_t() = default;
    wrapper_t(string_t prefix, string_t postfix)
        : base_print_t{prefix, postfix} {};
  } bind;
};

class base_unbind_t {
public:
  ~base_unbind_t() = default;
  base_unbind_t(string_t prefix, string_t postfix = "unbind: ")
      : unbind{prefix, postfix} {};
  struct wrapper_t : public base_print_t {
    ~wrapper_t() = default;
    wrapper_t(string_t prefix, string_t postfix)
        : base_print_t{prefix, postfix} {};
  } unbind;
};

class base_eval_t {
public:
  ~base_eval_t() = default;
  base_eval_t(string_t prefix, string_t postfix = "eval: ")
      : eval{prefix, postfix} {};
  struct wrapper_t : public print_here_t {
    ~wrapper_t() = default;
    wrapper_t(string_t prefix, string_t postfix)
        : print_here_t{prefix, postfix}, prefix(prefix), postfix(postfix) {};
    void start(string_t js, bool skip_queue) const;
    void work(string_t js) const;
    void done(bool done, string_t js) const;

  private:
    std::string prefix;
    std::string postfix;
  } eval;
};

} // namespace base_api

using namespace queue_api;
using namespace base_api;

class base_t : public print_here_t {
public:
  ~base_t() = default;
  base_t(string_t prefix, string_t postfix = "BASE: ")
      : print_here_t{prefix, "root: "}, base{prefix, postfix} {};

  struct wrapper_t : public base_bind_t,
                     public base_unbind_t,
                     public base_eval_t,
                     public print_here_t {
    ~wrapper_t() = default;
    wrapper_t(string_t prefix, string_t postfix)
        : base_bind_t(prefix + postfix),
          base_unbind_t(prefix + postfix),
          base_eval_t(prefix + postfix),
          print_here_t(prefix, postfix) {};
  } base;
};

class queue_t : public print_here_t {
public:
  ~queue_t() = default;
  queue_t(string_t prefix, string_t postfix = "QUEUE: ")
      : print_here_t(prefix, "root: "), queue{prefix, postfix} {};

  struct wrapper_t : public queue_bind_t,
                     public queue_unbind_t,
                     public queue_eval_t,
                     public queue_loop_t,
                     public queue_notify_t,
                     public queue_enqueue_t,
                     public print_here_t {
    ~wrapper_t() = default;
    wrapper_t(string_t prefix, string_t postfix)
        : queue_bind_t{prefix + postfix},
          queue_unbind_t{prefix + postfix},
          queue_eval_t{prefix + postfix},
          queue_loop_t{prefix + postfix},
          queue_notify_t{prefix + postfix},
          queue_enqueue_t{prefix + postfix},
          print_here_t{prefix, postfix} {};
  } queue;
};

class trace_t : public base_t, public queue_t {
public:
  ~trace_t() = default;
  trace_t(string_t prefix) : base_t("........ " + prefix), queue_t(prefix) {};
};

} // namespace utility
} // namespace webview
// NOLINTEND(cppcoreguidelines-non-private-member-variables-in-classes)

#endif // defined(__cplusplus) && !defined(WEBVIEW_HEADER)
#endif // WEBVIEW_UTILITY_TRACE_LOG_HH