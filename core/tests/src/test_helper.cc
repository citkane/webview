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
#include "../include/test_helper.hh"
#include "webview/detail/engine_base.hh"

using namespace webview::test;
using namespace webview::strings;
using namespace webview::detail::backend;
using namespace webview::test::_structs;

std::string js_t::init(str_arg_t init_value) const {
  auto init_value_js = post_value(init_value);
  return tokenise(TEST_INIT_JS, tokens.post_fn, init_value_js);
}
std::string js_t::post_value(str_arg_t value) const {
  return tokenise(TEST_VALUE_WRAPPER_JS, strings::tokens.value, value);
}
std::string js_t::make_call_js(unsigned int result) const {
  return tokenise(TEST_MAKE_CALL_JS, tokens.value, std::to_string(result));
}

std::string html_t::string_returns(str_arg_t title) const {
  return tokenise(TEST_STRING_RETURNS_HTML, tokens.value, title);
}
std::string html_t::navigate_encoded() const {
  std::string encoding = "data:text/html,";
  std::string html =
      "%3Chtml%3EEnsure%20that%20JS%20code%20can%20call%20native%"
      "20code%20and%"
      "20vice%20versa%3C%2Fhtml%3E";
  return encoding + html;
}

bool tester::resolve_on_main_thread() {
  std::lock_guard<std::mutex> lock(mtx());
  return resolve_on_main_thread_().load();
}
void tester::resolve_on_main_thread(bool val) {
  std::lock_guard<std::mutex> lock(mtx());
  resolve_on_main_thread_().store(val);
}

void tester::set_value(str_arg_t val) {
  std::lock_guard<std::mutex> lock(mtx());
  string_value() = val;
  eval_values();
  if (worker_proceed().load()) {
    cv().notify_all();
  }
}

void tester::expect_value(str_arg_t value) {
  std::lock_guard<std::mutex> lock(mtx());
  string_expected_value() = value;
  eval_values();
}

bool tester::values_match() { return worker_proceed().load(); }

std::string tester::get_value() {
  std::lock_guard<std::mutex> lock(mtx());
  return string_value();
}

void tester::ping_value(str_arg_t value, engine_base &wv) {
  wv.dispatch([&, value] { wv.eval(js.post_value(value)); });
}

std::chrono::seconds tester::seconds(int seconds) {
  return std::chrono::seconds(seconds);
}

#endif // defined(__cplusplus) && !defined(WEBVIEW_HEADER)
#endif // WEBVIEW_TEST_HELPER_CC