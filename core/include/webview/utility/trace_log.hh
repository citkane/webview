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

#ifndef WEBVIEW_UTIL_TRACE_LOG_HH
#define WEBVIEW_UTIL_TRACE_LOG_HH

#if defined(__cplusplus) && !defined(WEBVIEW_HEADER)

#include <chrono>
#include <string>

namespace webview {
namespace utility {

template <typename T> struct api_base {
  T *self;
  api_base(T *self) : self(self) {}
};
using time_point_t = std::chrono::time_point<std::chrono::steady_clock>;

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
  print_here_t(std::string prefix, std::string postfix);
  void print_here(std::string message) const;

private:
  std::string prefix;
  std::string postfix;
};

// NOLINTBEGIN(cppcoreguidelines-non-private-member-variables-in-classes)
namespace queue_api {

class queue_bind_t : public print_here_t {
public:
  ~queue_bind_t() = default;
  queue_bind_t(std::string prefix, std::string postfix = "BIND");

  void wait(std::string name) const;
  void start(std::string name) const;
  void done(bool done, std::string name) const;

private:
  std::string prefix;
  std::string postfix;
};
class queue_unbind_t : public print_here_t {
public:
  ~queue_unbind_t() = default;
  queue_unbind_t(std::string prefix, std::string postfix = "UNBIND");

  void wait(std::string name) const;
  void start(std::string name) const;
  void done(bool done, std::string name) const;

private:
  std::string prefix;
  std::string postfix;
};
class queue_eval_t : public print_here_t {
public:
  ~queue_eval_t() = default;
  queue_eval_t(std::string prefix, std::string postfix = "EVAL");

  void start(std::string js) const;
  void done(bool done) const;

private:
  std::string prefix;
  std::string postfix;
};
class queue_loop_t : public print_here_t {
public:
  ~queue_loop_t() = default;
  queue_loop_t(std::string prefix, std::string postfix = "loop: ");

  void wait(size_t size, bool empty, bool dom_ready) const;
  void start(size_t size) const;
  void end() const;

private:
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
  std::string prefix;
  std::string postfix;
};
class queue_notify_t : public print_here_t {
public:
  ~queue_notify_t() = default;
  queue_notify_t(std::string prefix, std::string postfix = "notify: ");

  void on_message(std::string method) const;

private:
  std::string prefix;
  std::string postfix;
};
class queue_enqueue_t : public print_here_t {
public:
  ~queue_enqueue_t() = default;
  queue_enqueue_t(std::string prefix, std::string postfix = "enqueue: ");

  void added(char scp, size_t size, std::string name_or_js) const;
  void added(char scp, size_t size) const;

private:
  std::string prefix;
  std::string postfix;
};
class queue_t {
public:
  ~queue_t() = default;
  queue_t(std::string prefix, std::string postfix = "QUEUE: ");

  struct queue_s : api_base<queue_t>, public print_here_t {
    queue_s(queue_t *self, std::string prefix, std::string postfix);
    queue_bind_t bind;
    queue_unbind_t unbind;
    queue_eval_t eval;
    queue_loop_t loop;
    queue_notify_t notify;
    queue_enqueue_t enqueue;
  } queue;

protected:
  friend class trace_t;
  friend class queue_bind_t;
  friend class queue_unbind_t;
  static void print_wait(std::string prefix, std::string postfix,
                         std::string name);
  static void print_start(std::string prefix, std::string postfix,
                          std::string name);
  static void print_done(std::string prefix, std::string postfix, bool done,
                         std::string name);
};

} // namespace queue_api

namespace base_api {

class base_bind_t : public print_here_t {
public:
  ~base_bind_t() = default;
  base_bind_t(std::string prefix, std::string postfix = "bind: ");

  void start(std::string name) const;
  void work(std::string name) const;
  void done(std::string name) const;

private:
  std::string prefix;
  std::string postfix;
};
class base_unbind_t : public print_here_t {
public:
  ~base_unbind_t() = default;
  base_unbind_t(std::string prefix, std::string postfix = "unbind: ");

  void start(std::string name) const;
  void work(std::string name) const;
  void done(std::string name) const;

private:
  std::string prefix;
  std::string postfix;
};

class base_eval_t : public print_here_t {
public:
  ~base_eval_t() = default;
  base_eval_t(std::string prefix, std::string postfix = "eval: ");

  void start(std::string js, bool skip_queue) const;
  void work(std::string js) const;
  void done(bool done, std::string js) const;

private:
  std::string prefix;
  std::string postfix;
};

class base_t {
public:
  base_t(std::string prefix, std::string postfix = "BASE: ");
  struct base_s : api_base<base_t>, public print_here_t {
    base_s(base_t *self, std::string prefix, std::string postfix);
    base_bind_t bind;
    base_unbind_t unbind;
    base_eval_t eval;
  } base;

protected:
  friend class trace_t;
  friend class base_bind_t;
  friend class base_unbind_t;
  friend class base_eval_t;
  static void print_start(std::string prefix, std::string name);
  static void print_work(std::string prefix, std::string name);
  static void print_done(std::string prefix, std::string name);
};

} // namespace base_api

using namespace queue_api;
using namespace base_api;

class trace_t : public base_t, public queue_t, public print_here_t {
public:
  virtual ~trace_t() = default;
  trace_t(std::string prefix, std::string postfix = "root: ");
};

} // namespace utility
} // namespace webview
// NOLINTEND(cppcoreguidelines-non-private-member-variables-in-classes)

#endif // defined(__cplusplus) && !defined(WEBVIEW_HEADER)
#endif // WEBVIEW_UTIL_TRACE_LOG_HH