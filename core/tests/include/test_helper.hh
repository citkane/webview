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

#ifndef WEBVIEW_TEST_HELPER_HH
#define WEBVIEW_TEST_HELPER_HH

#if defined(__cplusplus) && !defined(WEBVIEW_HEADER)
#include "webview/detail/engine_base.hh"
#include "webview/detail/frontend/frontend_strings.hh"
#include <atomic>
#include <chrono>
#include <condition_variable>

namespace webview {
using namespace webview::detail;

namespace test {
namespace _templates {

#define TEST_VALUE_WRAPPER                                                     \
  "window.__webview__.post(\n"                                                 \
  "  JSON.stringify({\n"                                                       \
  "    id: \"" TEST_NOTIFICATION_FLAG "\",\n"                                  \
  "    method: " TOKEN_VALUE "\n"                                              \
  "  })\n"                                                                     \
  ");"

} // namespace _templates

/// Static test utilities class
class tester {
public:
  static std::condition_variable &cv() {
    static std::condition_variable cv_;
    return cv_;
  }

  /// Sets the returned test value.
  static void set_value(const std::string &val) {
    string_value() = val;
    compare_values();
  };

  /// Sets the expected returned test value.
  static void expect_value(const std::string &value) {
    string_expected_value() = value;
  };

  /// Flag for evaluating if the returned value matches the expected value.
  static bool values_match() {
    compare_values();
    return worker_proceed().load();
  };

  /// Wraps a string value in evaluable JS
  static std::string get_value_js(const std::string &value) {
    return str::tokenise(TEST_VALUE_WRAPPER, str::token.value, value);
  }

  /// dispatch -> wv.eval of a tokenised `window.__webview__.post` string with a test value
  static void post_value(const std::string &value, engine_base *wv) {
    auto js = get_value_js(value);
    auto do_work = [=] { wv->eval(js); };
    wv->dispatch(do_work);
  }

  /// Wraps wv.terminate in dispatch
  static void terminate(engine_base *wv) {
    wv->dispatch([=] { wv->terminate(); });
  }

  /// Returns the given timespan in std::chrono::seconds
  static std::chrono::seconds seconds(int seconds = 1) {
    return std::chrono::seconds(seconds);
  };

private:
  static std::string &string_value() {
    static std::string val;
    return val;
  };
  static std::string &string_expected_value() {
    static std::string val;
    return val;
  };
  static std::atomic_bool &worker_proceed() {
    static std::atomic_bool val{};
    return val;
  }

  static void compare_values() {
    worker_proceed().store(string_expected_value() == string_value());
    cv().notify_one();
  }
};
} // namespace test
} // namespace webview

#endif // defined(__cplusplus) && !defined(WEBVIEW_HEADER)
#endif // WEBVIEW_TEST_HELPER_HH
