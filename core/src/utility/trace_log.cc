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

inline long elapsed_ms(time_point_t start, time_point_t end) {
  return std::chrono::duration_cast<std::chrono::milliseconds>(end - start)
      .count();
};

inline std::string bool_s(bool flag) { return flag ? "true" : "false"; }
inline std::string escape_s(string_t text) { return "\"" + text + "\""; }
inline std::string num_s(size_t val) { return std::to_string(val); }

inline std::string bold(string_t this_col, string_t text) {
  return ansi({1}) + text + col::default_c() + this_col;
}
inline std::string dim(string_t this_col, string_t text) {
  return ansi({2}) + text + col::default_c() + this_col;
}

inline void printf_col(string_t this_col, string_t message) {
  printf("%s%s%s\n", this_col.c_str(), message.c_str(),
         col::default_c().c_str());
};

} // namespace

string_t col::yellow() {
  static string_t col = ansi({33});
  return col;
};
string_t col::yellow_dim() {
  static string_t col = ansi({33, 2});
  return col;
};
string_t col::green() {
  static string_t col = ansi({92});
  return col;
};
string_t col::red() {
  static string_t col = ansi({91});
  return col;
};
string_t col::blue() {
  static string_t col = ansi({94});
  return col;
};
string_t col::blue_dark() {
  static string_t col = ansi({34});
  return col;
};
string_t col::magenta() {
  static string_t col = ansi({95});
  return col;
};
string_t col::default_c() {
  static string_t col = ansi({0});
  return col;
};

print_here_t::print_here_t(string_t prefix, string_t postfix)
    : prefix(prefix), postfix(postfix) {}
void print_here_t::print_here(string_t message) const {
  auto this_c = col::magenta();
  auto here_m = bold(this_c, "here") + ": ";
  auto message_ = col::default_c() + message;
  printf_col(this_c, prefix + postfix + here_m + message_);
};

namespace queue_api {
void queue_print_t::start(string_t name) const {
  auto this_c = col::blue();
  auto postfix_m = bold(this_c, postfix) + ": ";
  auto start_m = bold(this_c, "START: ") + escape_s(name);
  printf_col(this_c, prefix + postfix_m + start_m);
}
void queue_print_t::wait(string_t name) const {
  auto this_c = col::yellow_dim();
  auto postfix_m = bold(this_c, postfix) + ": ";
  auto wait_m = bold(this_c, "WAIT: ") + escape_s(name);
  printf_col(this_c, prefix + postfix_m + wait_m);
}
void queue_print_t::done(bool done, string_t name) const {
  auto this_c = col::blue();
  auto postfix_m = bold(this_c, postfix) + ": ";
  auto done_m = bold(this_c, "work_done: ") + bool_s(done) + ": ";
  auto name_m = escape_s(name);
  printf_col(this_c, prefix + postfix_m + done_m + name_m);
}

void queue_eval_t::wrapper_t::start(string_t js) const {
  auto this_c = col::blue();
  auto postfix_m = bold(this_c, postfix) + ": ";
  auto start_m = bold(this_c, "START") + " js ...\n";
  auto js_m = dim(this_c, js);
  printf_col(this_c, prefix + postfix_m + start_m + js_m);
};
void queue_eval_t::wrapper_t::done(bool done) const {
  auto this_c = col::blue();
  auto postfix_m = bold(this_c, postfix) + ": ";
  auto done_m = bold(this_c, "DONE: ") + bool_s(done);
  printf_col(this_c, prefix + postfix_m + done_m);
}

void queue_loop_t::wrapper_t::wait(size_t size, bool empty,
                                   bool dom_ready) const {
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
void queue_loop_t::wrapper_t::start(size_t size) const {
  set_loop_start_ts();
  auto this_col = col::green();
  auto start = bold(this_col, "START ");
  auto time_m = "| waited for: " + wait_elapsed() + " ";
  auto size_m = "| queue size: " + std::to_string(size);

  printf_col(this_col, prefix + postfix + start + time_m + size_m + "\n");
};
void queue_loop_t::wrapper_t::end() const {
  set_loop_end_ts();
  auto this_col = col::green();
  auto end = bold(this_col, "END ");
  auto time_m = "| loop process time: " + loop_elapsed();
  printf_col(this_col, "\n" + prefix + postfix + end + time_m);
}
void queue_loop_t::wrapper_t::set_loop_wait_ts() const {
  loop_wait_ts = get_now();
}
void queue_loop_t::wrapper_t::set_loop_start_ts() const {
  loop_start_ts = get_now();
}
void queue_loop_t::wrapper_t::set_loop_end_ts() const {
  loop_end_ts = get_now();
}
std::string queue_loop_t::wrapper_t::total_elapsed() const {
  auto elapsed = elapsed_ms(process_ts, loop_wait_ts);
  return std::to_string(elapsed) + "ms";
}
std::string queue_loop_t::wrapper_t::wait_elapsed() const {
  auto elapsed = elapsed_ms(loop_wait_ts, loop_start_ts);
  return std::to_string(elapsed) + "ms";
}
std::string queue_loop_t::wrapper_t::loop_elapsed() const {
  auto elapsed = elapsed_ms(loop_start_ts, loop_end_ts);
  return std::to_string(elapsed) + "ms";
}

void queue_notify_t::wrapper_t::on_message(string_t method) const {
  auto this_c = col::yellow_dim();
  auto mess_m = bold(this_c, "on_message") + ": ";
  auto method_m = escape_s(method);
  printf_col(this_c, prefix + postfix + mess_m + method_m);
}

void queue_enqueue_t::wrapper_t::added(char scp, size_t size,
                                       string_t name_or_js) const {
  auto this_c = col::default_c();
  auto ctx_m = get_ctx(scp);
  auto size_m = "queue size: " + num_s(size) + " | ";
  auto queued_m = bold(this_c, "queueing " + ctx_m) + " | ";
  auto name_or_js_m = scp == 'e' ? "got js ...\n" + dim(this_c, name_or_js)
                                 : escape_s(name_or_js);
  printf_col(this_c, prefix + postfix + queued_m + size_m + name_or_js_m);
}
void queue_enqueue_t::wrapper_t::added(char scp, size_t size) const {
  auto this_c = col::default_c();
  auto ctx_m = get_ctx(scp);
  auto size_m = "queue size: " + num_s(size) + " | ";
  auto queued_m = bold(this_c, "queueing " + ctx_m);
  printf_col(this_c, prefix + postfix + size_m + queued_m);
}

} // namespace queue_api

namespace base_api {

void base_print_t::start(string_t name) const {
  auto this_c = col::default_c();
  auto name_m = escape_s(name);
  auto got_m = bold(this_c, "got") + ": ";
  printf_col(this_c, prefix + postfix + got_m + name_m);
}
void base_print_t::work(string_t name) const {
  auto this_c = col::blue_dark();
  auto work_m = bold(this_c, "do_work: ");
  auto name_m = escape_s(name);
  printf_col(this_c, prefix + postfix + work_m + name_m);
}
void base_print_t::done(string_t name) const {
  auto this_c = col::blue();
  auto done_m = bold(this_c, "work done") + ": ";
  auto name_m = escape_s(name);
  printf_col(this_c, prefix + postfix + done_m + name_m);
}

void base_eval_t::wrapper_t::start(string_t js, bool skip_queue) const {
  auto this_c = col::default_c();
  auto skip_m = "skip queue: " + bool_s(skip_queue) + " | ";
  auto received_m = bold(this_c, "received js") + " ...\n";
  auto m = prefix + postfix + skip_m + received_m + dim(this_c, js);
  printf_col(this_c, m);
};
void base_eval_t::wrapper_t::work(string_t js) const {
  auto this_c = col::blue_dark();
  auto work_m = bold(this_c, "do_work") + " js...\n";
  auto m = prefix + postfix + work_m + dim(this_c, js);
  printf_col(this_c, m);
};
void base_eval_t::wrapper_t::done(bool done, string_t js) const {
  auto this_c = col::blue();
  auto done_m = bold(this_c, "work done") + ": " + bool_s(done) + " | js ...\n";
  auto js_m = dim(this_c, js);
  printf_col(this_c, prefix + postfix + done_m + js_m);
}

} // namespace base_api

} // namespace utility
} // namespace webview
#endif // defined(__cplusplus) && !defined(WEBVIEW_HEADER)
#endif // WEBVIEW_UTIL_TRACE_LOG_CC
