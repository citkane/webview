/*
 * MIT License
 *
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

#ifndef WEBVIEW_UTIL_TRACE_LOG_CC
#define WEBVIEW_UTIL_TRACE_LOG_CC

#if defined(__cplusplus) && !defined(WEBVIEW_HEADER)

#include "webview/utility/trace_log.hh"

namespace webview {
namespace utility {
namespace {

inline std::string ansi(std::initializer_list<int> codes) {
  std::string ansi_string;
  for (size_t i = 0; i < codes.size(); ++i) {
    auto code = std::to_string(*(codes.begin() + i));
    if (i == 0 && codes.size() == 1) {
      ansi_string += "\033[" + code;
    } else if (i == 0) {
      ansi_string += "\033[" + code + ";";
    } else if (i == codes.size() - 1) {
      ansi_string += code;
    } else {
      ansi_string += code + ";";
    }
  };
  ansi_string += "m";
  return ansi_string;
}
inline std::string get_ctx(char scp) {
  if (scp == 'b') {
    return "bind";
  }
  if (scp == 'u') {
    return "unbind";
  }
  return "eval";
}

inline time_point_t get_now() { return std::chrono::steady_clock::now(); }
inline long elapsed_ms(time_point_t start, time_point_t end) {
  return std::chrono::duration_cast<std::chrono::milliseconds>(end - start)
      .count();
};

inline std::string bool_s(bool flag) { return flag ? "true" : "false"; }
inline std::string escape_s(std::string text) { return "\"" + text + "\""; }
inline std::string num_s(size_t val) { return std::to_string(val); }

inline std::string bold(std::string this_col, std::string text) {
  return ansi({1}) + text + col::default_c() + this_col;
}
inline std::string dim(std::string this_col, std::string text) {
  return ansi({2}) + text + col::default_c() + this_col;
}

inline void printf_col(std::string this_col, std::string message) {
  printf("%s%s%s\n", this_col.c_str(), message.c_str(),
         col::default_c().c_str());
};

} // namespace
const std::string &col::yellow() {
  static std::string col = ansi({33});
  return col;
};
const std::string &col::yellow_dim() {
  static std::string col = ansi({33, 2});
  return col;
};
const std::string &col::green() {
  static std::string col = ansi({92});
  return col;
};
const std::string &col::red() {
  static std::string col = ansi({91});
  return col;
};
const std::string &col::blue() {
  static std::string col = ansi({94});
  return col;
};
const std::string &col::blue_dark() {
  static std::string col = ansi({34});
  return col;
};
const std::string &col::magenta() {
  static std::string col = ansi({95});
  return col;
};
const std::string &col::default_c() {
  static std::string col = ansi({0});
  return col;
};

print_here_t::print_here_t(std::string prefix, std::string postfix)
    : prefix(prefix), postfix(postfix) {};
void print_here_t::print_here(std::string message) const {
  auto this_c = col::magenta();
  auto here_m = bold(this_c, "here") + ": ";
  message = col::default_c() + message;
  printf_col(this_c, prefix + postfix + here_m + message);
};

namespace queue_api {

queue_bind_t::queue_bind_t(std::string prefix, std::string postfix)
    : print_here_t{prefix, postfix}, prefix(prefix), postfix(postfix) {}
void queue_bind_t::wait(std::string name) const {
  queue_t::print_wait(prefix, postfix, name);
};
void queue_bind_t::start(std::string name) const {
  queue_t::print_start(prefix, postfix, name);
};
void queue_bind_t::done(bool done, std::string name) const {
  queue_t::print_done(prefix, postfix, done, name);
}

queue_unbind_t::queue_unbind_t(std::string prefix, std::string postfix)
    : print_here_t{prefix, postfix}, prefix(prefix), postfix(postfix) {};
void queue_unbind_t::wait(std::string name) const {
  queue_t::print_wait(prefix, postfix, name);
};
void queue_unbind_t::start(std::string name) const {
  queue_t::print_start(prefix, postfix, name);
};
void queue_unbind_t::done(bool done, std::string name) const {
  queue_t::print_done(prefix, postfix, done, name);
}

queue_eval_t::queue_eval_t(std::string prefix, std::string postfix)
    : print_here_t{prefix, postfix}, prefix(prefix), postfix(postfix) {};
void queue_eval_t::start(std::string js) const {
  auto this_c = col::blue();
  auto postfix_m = bold(this_c, postfix) + ": ";
  auto start_m = bold(this_c, "START") + " js ...\n";
  auto js_m = dim(this_c, js);
  printf_col(this_c, prefix + postfix_m + start_m + js_m);
};
void queue_eval_t::done(bool done) const {
  auto this_c = col::blue();
  auto postfix_m = bold(this_c, postfix) + ": ";
  auto done_m = bold(this_c, "DONE: ") + bool_s(done);
  printf_col(this_c, prefix + postfix_m + done_m);
}

queue_loop_t::queue_loop_t(std::string prefix, std::string postfix)
    : print_here_t{prefix, postfix},
      process_ts(get_now()),
      loop_wait_ts(get_now()),
      loop_start_ts(get_now()),
      loop_end_ts(get_now()),
      prefix(prefix),
      postfix(postfix) {};
void queue_loop_t::wait(size_t size, bool empty, bool dom_ready) const {
  set_loop_wait_ts();
  auto this_col = col::green();
  auto waiting = bold(this_col, "WAITING ");
  auto size_s = "| queue size: " + std::to_string(size) + " ";
  auto empty_s = "| queue_empty: " + bool_s(empty) + " ";
  auto dom_s = "| dom_ready: " + bool_s(dom_ready) + " ";
  auto time_m = "| total process time: " + total_elapsed() + " ";

  printf_col(this_col,
             prefix + postfix + waiting + time_m + size_s + empty_s + dom_s);
};
void queue_loop_t::start(size_t size) const {
  set_loop_start_ts();
  auto this_col = col::green();
  auto start = bold(this_col, "START ");
  auto time_m = "| waited for: " + wait_elapsed() + " ";
  auto size_m = "| queue size: " + std::to_string(size);

  printf_col(this_col, prefix + postfix + start + time_m + size_m + "\n");
};
void queue_loop_t::end() const {
  set_loop_end_ts();
  auto this_col = col::green();
  auto end = bold(this_col, "END ");
  auto time_m = "| loop process time: " + loop_elapsed();
  printf_col(this_col, "\n" + prefix + postfix + end + time_m);
}
void queue_loop_t::set_loop_wait_ts() const { loop_wait_ts = get_now(); }
void queue_loop_t::set_loop_start_ts() const { loop_start_ts = get_now(); }
void queue_loop_t::set_loop_end_ts() const { loop_end_ts = get_now(); }

std::string queue_loop_t::total_elapsed() const {
  auto elapsed = elapsed_ms(process_ts, loop_wait_ts);
  return std::to_string(elapsed) + "ms";
}
std::string queue_loop_t::wait_elapsed() const {
  auto elapsed = elapsed_ms(loop_wait_ts, loop_start_ts);
  return std::to_string(elapsed) + "ms";
}
std::string queue_loop_t::loop_elapsed() const {
  auto elapsed = elapsed_ms(loop_start_ts, loop_end_ts);
  return std::to_string(elapsed) + "ms";
}

queue_notify_t::queue_notify_t(std::string prefix, std::string postfix)
    : print_here_t{prefix, postfix}, prefix(prefix), postfix(postfix) {}
void queue_notify_t::on_message(std::string method) const {
  auto this_c = col::yellow_dim();
  auto mess_m = bold(this_c, "on_message") + ": ";
  auto method_m = escape_s(method);
  printf_col(this_c, prefix + postfix + mess_m + method_m);
}

queue_enqueue_t::queue_enqueue_t(std::string prefix, std::string postfix)
    : print_here_t{prefix, postfix}, prefix(prefix), postfix(postfix) {}
void queue_enqueue_t::added(char scp, size_t size,
                            std::string name_or_js) const {
  auto this_c = col::default_c();
  auto ctx_m = get_ctx(scp);
  auto size_m = "queue size: " + num_s(size) + " | ";
  auto queued_m = bold(this_c, "queueing " + ctx_m) + " | ";
  auto name_or_js_m = scp == 'e' ? "got js ...\n" + dim(this_c, name_or_js)
                                 : escape_s(name_or_js);
  printf_col(this_c, prefix + postfix + queued_m + size_m + name_or_js_m);
}
void queue_enqueue_t::added(char scp, size_t size) const {
  auto this_c = col::default_c();
  auto ctx_m = get_ctx(scp);
  auto size_m = "queue size: " + num_s(size) + " | ";
  auto queued_m = bold(this_c, "queueing " + ctx_m);
  printf_col(this_c, prefix + postfix + size_m + queued_m);
}

queue_t::queue_t(std::string prefix, std::string postfix)
    : queue(this, prefix, postfix) {};
queue_t::queue_s::queue_s(queue_t *self, std::string prefix,
                          std::string postfix)
    : api_base(self),
      print_here_t{prefix, postfix},
      bind{prefix + postfix},
      unbind{prefix + postfix},
      eval{prefix + postfix},
      loop{prefix + postfix},
      notify{prefix + postfix},
      enqueue{prefix + postfix} {};
void queue_t::print_wait(std::string prefix, std::string postfix,
                         std::string name) {
  auto this_c = col::yellow_dim();
  auto postfix_m = bold(this_c, postfix) + ": ";
  auto wait_m = bold(this_c, "WAIT: ") + escape_s(name);
  printf_col(this_c, prefix + postfix_m + wait_m);
}
void queue_t::print_start(std::string prefix, std::string postfix,
                          std::string name) {
  auto this_c = col::blue();
  auto postfix_m = bold(this_c, postfix) + ": ";
  auto start_m = bold(this_c, "START: ") + escape_s(name);
  printf_col(this_c, prefix + postfix_m + start_m);
}
void queue_t::print_done(std::string prefix, std::string postfix, bool done,
                         std::string name) {
  auto this_c = col::blue();
  auto postfix_m = bold(this_c, postfix) + ": ";
  auto done_m = bold(this_c, "work_done: ") + bool_s(done) + ": ";
  auto name_m = escape_s(name);
  printf_col(this_c, prefix + postfix_m + done_m + name_m);
}

} // namespace queue_api

namespace base_api {

base_bind_t::base_bind_t(std::string prefix, std::string postfix)
    : print_here_t{prefix, postfix}, prefix(prefix), postfix(postfix) {};
void base_bind_t::start(std::string name) const {
  base_t::print_start(prefix + postfix, name);
};
void base_bind_t::work(std::string name) const {
  base_t::print_work(prefix + postfix, name);
};
void base_bind_t::done(std::string name) const {
  base_t::print_done(prefix + postfix, name);
}

base_unbind_t::base_unbind_t(std::string prefix, std::string postfix)
    : print_here_t{prefix, postfix}, prefix(prefix), postfix(postfix) {};
void base_unbind_t::start(std::string name) const {
  base_t::print_start(prefix + postfix, name);
};
void base_unbind_t::work(std::string name) const {
  base_t::print_work(prefix + postfix, name);
};
void base_unbind_t::done(std::string name) const {
  base_t::print_done(prefix + postfix, name);
}

base_eval_t::base_eval_t(std::string prefix, std::string postfix)
    : print_here_t{prefix, postfix}, prefix(prefix), postfix(postfix) {};
void base_eval_t::start(std::string js, bool skip_queue) const {
  auto this_c = col::default_c();
  auto skip_m = "skip queue: " + bool_s(skip_queue) + " | ";
  auto received_m = bold(this_c, "received js") + " ...\n";
  auto m = prefix + postfix + skip_m + received_m + dim(this_c, js);
  printf_col(this_c, m);
};
void base_eval_t::work(std::string js) const {
  auto this_c = col::blue_dark();
  auto work_m = bold(this_c, "do_work") + " js...\n";
  auto m = prefix + postfix + work_m + dim(this_c, js);
  printf_col(this_c, m);
};
void base_eval_t::done(bool done, std::string js) const {
  auto this_c = col::blue();
  auto done_m = bold(this_c, "work done") + ": " + bool_s(done) + " | js ...\n";
  auto js_m = dim(this_c, js);
  printf_col(this_c, prefix + postfix + done_m + js_m);
}

void base_t::print_start(std::string prefix, std::string name) {
  auto this_c = col::default_c();
  auto name_m = escape_s(name);
  auto got_m = bold(this_c, "got") + ": ";
  printf_col(this_c, prefix + got_m + name_m);
}
void base_t::print_work(std::string prefix, std::string name) {
  auto this_c = col::blue_dark();
  auto work_m = bold(this_c, "do_work: ");
  auto name_m = escape_s(name);
  printf_col(this_c, prefix + work_m + name_m);
}
void base_t::print_done(std::string prefix, std::string name) {
  auto this_c = col::blue();
  auto done_m = bold(this_c, "work done") + ": ";
  auto name_m = escape_s(name);
  printf_col(this_c, prefix + done_m + name_m);
}
base_t::base_t(std::string prefix, std::string postfix)
    : base{this, prefix, postfix} {};
base_t::base_s::base_s(base_t *self, std::string prefix, std::string postfix)
    : api_base(self),
      print_here_t{prefix, postfix},
      bind{prefix + postfix},
      unbind{prefix + postfix},
      eval{prefix + postfix} {};

} // namespace base_api

trace_t::trace_t(std::string prefix, std::string postfix)
    : base_t(prefix), queue_t(prefix), print_here_t(prefix, postfix) {};

} // namespace utility
} // namespace webview
#endif // defined(__cplusplus) && !defined(WEBVIEW_HEADER)
#endif // WEBVIEW_UTIL_TRACE_LOG_CC
