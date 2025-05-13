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

#ifndef WEBVIEW_LOG_TRACE_LOG_CC
#define WEBVIEW_LOG_TRACE_LOG_CC

#if defined(__cplusplus) && !defined(WEBVIEW_HEADER)
#include "webview/log/trace_log.hh"

namespace webview {
namespace log {
namespace _structs {

std::mutex trace_tools_t::trace_mtx;

time_point_t trace_tools_t::get_now() const {
  return std::chrono::steady_clock::now();
}
std::string trace_tools_t::get_ctx(char scp) const {
  if (scp == 'b') {
    return "bind";
  }
  if (scp == 'u') {
    return "unbind";
  }
  return "eval";
}
long trace_tools_t::elapsed_ms(time_point_t start, time_point_t end) const {
  return static_cast<long int>(
      std::chrono::duration_cast<std::chrono::milliseconds>(end - start)
          .count());
};
std::string trace_tools_t::bool_s(bool flag) const {
  return flag ? "true" : "false";
}
std::string trace_tools_t::escape_s(string_t text) const {
  return "\"" + text + "\"";
}
std::string trace_tools_t::num_s(size_t val) const {
  return std::to_string(val);
}
std::string trace_tools_t::bold(string_t this_col, string_t text) const {
  return ansi.bold + text + ansi.default_c + this_col;
}
std::string trace_tools_t::dim(string_t this_col, string_t text) const {
  return ansi.dim + text + ansi.default_c + this_col;
}
void trace_tools_t::print_ansi(string_t this_col, string_t message) const {
  std::lock_guard<std::mutex> lock(trace_mtx);
  printf("%s%s%s\n", this_col.c_str(), message.c_str(), ansi.default_c.c_str());
};

void print_here_t::print_here(string_t message) const {
  auto this_c = ansi.magenta;
  auto here_m = bold(this_c, "here") + ": ";
  auto message_ = ansi.default_c + message;
  print_ansi(this_c, prefix + postfix + here_m + message_);
};

#if WEBVIEW_LOG_TRACE
void queue_print_t::start(string_t name) const {

  auto this_c = ansi.blue;
  auto postfix_m = bold(this_c, postfix) + ": ";
  auto start_m = bold(this_c, "START: ") + escape_s(name);
  print_ansi(this_c, prefix + postfix_m + start_m);
}
#else
void queue_print_t::start(string_t /**/) const {}
#endif
#if WEBVIEW_LOG_TRACE
void queue_print_t::wait(string_t name) const {
  auto this_c = ansi.yellow_dim;
  auto postfix_m = bold(this_c, postfix) + ": ";
  auto wait_m = bold(this_c, "WAIT: ") + escape_s(name);
  print_ansi(this_c, prefix + postfix_m + wait_m);
}
#else
void queue_print_t::wait(string_t /**/) const {}
#endif
#if WEBVIEW_LOG_TRACE
void queue_print_t::done(bool done, string_t name) const {
  auto this_c = ansi.blue;
  auto postfix_m = bold(this_c, postfix) + ": ";
  auto done_m = bold(this_c, "work_done: ") + bool_s(done) + ": ";
  auto name_m = escape_s(name);
  print_ansi(this_c, prefix + postfix_m + done_m + name_m);
}
#else
void queue_print_t::done(bool /**/, string_t /**/) const {}
#endif

#if WEBVIEW_LOG_TRACE
void queue_eval_t::wrapper_t::start(string_t js) const {
  auto this_c = ansi.blue;
  auto postfix_m = bold(this_c, postfix) + ": ";
  auto start_m = bold(this_c, "START") + " js ...\n";
  auto js_m = dim(this_c, js);
  print_ansi(this_c, prefix + postfix_m + start_m + js_m);
};
#else
void queue_eval_t::wrapper_t::start(string_t /**/) const {}
#endif
#if WEBVIEW_LOG_TRACE
void queue_eval_t::wrapper_t::done(bool done) const {
  auto this_c = ansi.blue;
  auto postfix_m = bold(this_c, postfix) + ": ";
  auto done_m = bold(this_c, "DONE: ") + bool_s(done);
  print_ansi(this_c, prefix + postfix_m + done_m);
}
#else
void queue_eval_t::wrapper_t::done(bool /**/) const {}
#endif
#if WEBVIEW_LOG_TRACE
void queue_loop_t::wrapper_t::wait(size_t size, bool empty,
                                   bool dom_ready) const {
  set_loop_wait_ts();
  auto this_col = ansi.green;
  auto waiting = bold(this_col, "WAITING ");
  auto size_s = "| queue size: " + std::to_string(size) + " ";
  auto empty_s = "| queue_empty: " + bool_s(empty) + " ";
  auto dom_s = "| dom_ready: " + bool_s(dom_ready) + " ";
  auto time_m = "| total process time: " + total_elapsed() + " ";

  print_ansi(this_col,
             prefix + postfix + waiting + time_m + size_s + empty_s + dom_s);
};
#else
void queue_loop_t::wrapper_t::wait(size_t /**/, bool /**/, bool /**/) const {}
#endif

#if WEBVIEW_LOG_TRACE
void queue_loop_t::wrapper_t::start(size_t size) const {
  set_loop_start_ts();
  auto this_col = ansi.green;
  auto start = bold(this_col, "START ");
  auto time_m = "| waited for: " + wait_elapsed() + " ";
  auto size_m = "| queue size: " + std::to_string(size);

  print_ansi(this_col, prefix + postfix + start + time_m + size_m + "\n");
};
#else
void queue_loop_t::wrapper_t::start(size_t /**/) const {}
#endif
void queue_loop_t::wrapper_t::end() const {
#if WEBVIEW_LOG_TRACE
  set_loop_end_ts();
  auto this_col = ansi.green;
  auto end = bold(this_col, "END ");
  auto time_m = "| loop process time: " + loop_elapsed();
  print_ansi(this_col, "\n" + prefix + postfix + end + time_m);
#endif
}
void queue_loop_t::wrapper_t::set_loop_wait_ts() const {
#if WEBVIEW_LOG_TRACE
  loop_wait_ts = get_now();
#endif
}
void queue_loop_t::wrapper_t::set_loop_start_ts() const {
#if WEBVIEW_LOG_TRACE
  loop_start_ts = get_now();
#endif
}
void queue_loop_t::wrapper_t::set_loop_end_ts() const {
#if WEBVIEW_LOG_TRACE
  loop_end_ts = get_now();
#endif
}
std::string queue_loop_t::wrapper_t::total_elapsed() const {
#if WEBVIEW_LOG_TRACE
  auto elapsed = elapsed_ms(process_ts, loop_wait_ts);
  return std::to_string(elapsed) + "ms";
#else
  return "";
#endif
}
std::string queue_loop_t::wrapper_t::wait_elapsed() const {
#if WEBVIEW_LOG_TRACE
  auto elapsed = elapsed_ms(loop_wait_ts, loop_start_ts);
  return std::to_string(elapsed) + "ms";
#else
  return "";
#endif
}
std::string queue_loop_t::wrapper_t::loop_elapsed() const {
#if WEBVIEW_LOG_TRACE
  auto elapsed = elapsed_ms(loop_start_ts, loop_end_ts);
  return std::to_string(elapsed) + "ms";
#else
  return "";
#endif
}

#if WEBVIEW_LOG_TRACE
void queue_notify_t::wrapper_t::on_message(string_t method) const {
  auto this_c = ansi.yellow_dim;
  auto mess_m = bold(this_c, "on_message") + ": ";
  auto method_m = escape_s(method);
  print_ansi(this_c, prefix + postfix + mess_m + method_m);
}
#else
void queue_notify_t::wrapper_t::on_message(string_t /**/) const {}
#endif

#if WEBVIEW_LOG_TRACE
void queue_enqueue_t::wrapper_t::added(char scp, size_t size,
                                       string_t name_or_js) const {
  auto this_c = ansi.default_c;
  auto ctx_m = get_ctx(scp);
  auto size_m = "queue size: " + num_s(size) + " | ";
  auto queued_m = bold(this_c, "queueing " + ctx_m) + " | ";
  auto name_or_js_m = scp == 'e' ? "got js ...\n" + dim(this_c, name_or_js)
                                 : escape_s(name_or_js);
  print_ansi(this_c, prefix + postfix + queued_m + size_m + name_or_js_m);
}
#else
void queue_enqueue_t::wrapper_t::added(char /**/, size_t /**/,
                                       string_t /**/) const {}
#endif
#if WEBVIEW_LOG_TRACE
void queue_enqueue_t::wrapper_t::added(char scp, size_t size) const {
  auto this_c = ansi.default_c;
  auto ctx_m = get_ctx(scp);
  auto size_m = "queue size: " + num_s(size) + " | ";
  auto queued_m = bold(this_c, "queueing " + ctx_m);
  print_ansi(this_c, prefix + postfix + size_m + queued_m);
}
#else
void queue_enqueue_t::wrapper_t::added(char /**/, size_t /**/) const {}
#endif

#if WEBVIEW_LOG_TRACE
void base_print_t::start(string_t name) const {
  auto this_c = ansi.default_c;
  auto name_m = escape_s(name);
  auto got_m = bold(this_c, "got") + ": ";
  print_ansi(this_c, prefix + postfix + got_m + name_m);
}
#else
void base_print_t::start(string_t /**/) const {}
#endif
#if WEBVIEW_LOG_TRACE
void base_print_t::work(string_t name) const {
  auto this_c = ansi.blue_dark;
  auto work_m = bold(this_c, "do_work: ");
  auto name_m = escape_s(name);
  print_ansi(this_c, prefix + postfix + work_m + name_m);
}
#else
void base_print_t::work(string_t /**/) const {}
#endif
#if WEBVIEW_LOG_TRACE
void base_print_t::done(string_t name) const {
  auto this_c = ansi.blue;
  auto done_m = bold(this_c, "work done") + ": ";
  auto name_m = escape_s(name);
  print_ansi(this_c, prefix + postfix + done_m + name_m);
}
#else
void base_print_t::done(string_t /**/) const {}
#endif

#if WEBVIEW_LOG_TRACE
void base_eval_t::wrapper_t::start(string_t js, bool skip_queue) const {
  auto this_c = ansi.default_c;
  auto skip_m = "skip queue: " + bool_s(skip_queue) + " | ";
  auto received_m = bold(this_c, "received js") + " ...\n";
  auto m = prefix + postfix + skip_m + received_m + dim(this_c, js);
  print_ansi(this_c, m);
};
#else
void base_eval_t::wrapper_t::start(string_t /**/, bool /**/) const {}
#endif
#if WEBVIEW_LOG_TRACE
void base_eval_t::wrapper_t::work(string_t js) const {
  auto this_c = ansi.blue_dark;
  auto work_m = bold(this_c, "do_work") + " js...\n";
  auto m = prefix + postfix + work_m + dim(this_c, js);
  print_ansi(this_c, m);
};
#else
void base_eval_t::wrapper_t::work(string_t /**/) const {}
#endif
#if WEBVIEW_LOG_TRACE
void base_eval_t::wrapper_t::done(bool done, string_t js) const {
  auto this_c = ansi.blue;
  auto done_m = bold(this_c, "work done") + ": " + bool_s(done) + " | js ...\n";
  auto js_m = dim(this_c, js);
  print_ansi(this_c, prefix + postfix + done_m + js_m);
}
#else
void base_eval_t::wrapper_t::done(bool /**/, string_t /**/) const {}
#endif

} // namespace _structs

const _structs::base_t &trace::base = get_base();
const _structs::queue_t &trace::queue = get_queue();

} // namespace log
} // namespace webview
#endif // defined(__cplusplus) && !defined(WEBVIEW_HEADER)
#endif // WEBVIEW_LOG_TRACE_LOG_CC
