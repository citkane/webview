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

#ifndef WEBVIEW_TEST_HELPER_CC
#define WEBVIEW_TEST_HELPER_CC

#if defined(__cplusplus) && !defined(WEBVIEW_HEADER)
#include "webview/tests/test_helper.hh"
#include "webview/detail/engine_base.hh"
#include "webview/strings/string_api.hh"

using namespace webview::strings;

bool tester::resolve_on_main_thread() {
  return resolve_on_main_thread_().load();
}
void tester::resolve_on_main_thread(bool val) {
  resolve_on_main_thread_().store(val);
}

void tester::set_value(const_str_ref val) {
  std::lock_guard<std::mutex> lock(mtx());
  string_value() = val;
  eval_values();
  if (worker_proceed().load()) {
    cv().notify_all();
  }
}

void tester::expect_value(const_str_ref value) {
  std::lock_guard<std::mutex> lock(mtx());
  string_expected_value() = value;
  eval_values();
}

bool tester::values_match() { return worker_proceed().load(); }

std::string tester::get_value() {
  std::lock_guard<std::mutex> lock(mtx());
  return string_value();
}

void tester::ping_value(const_str_ref escaped_value, engine_base &wv,
                        bool escaped) {
  std::lock_guard<std::mutex> lock(mtx());
  wv.dispatch([&, escaped_value, escaped] {
    wv.eval(string::tests::js.post_value(escaped_value, escaped));
  });
}

std::chrono::seconds tester::seconds(int seconds) {
  return std::chrono::seconds(seconds);
}

#endif // defined(__cplusplus) && !defined(WEBVIEW_HEADER)
#endif // WEBVIEW_TEST_HELPER_CC