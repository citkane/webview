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
#include "webview/utility/frontend_strings.hh"
#include <atomic>
#include <chrono>
#include <condition_variable>

namespace webview {
using namespace webview::detail;
namespace test {
namespace templates {

#define TEST_POST                                                              \
  "window.__webview__.post(\n"                                                 \
  "  JSON.stringify({\n"                                                       \
  "    id: \"" TEST_NOTIFICATION_FLAG "\",\n"                                  \
  "    method: " TOKEN_VALUE "\n"                                              \
  "  })\n"                                                                     \
  ");"

#define TEST_B_UB_CALL                                                         \
  "try {\n"                                                                    \
  "  window.increment()"                                                       \
  "  .then(r => window.test(" TOKEN_VALUE "))\n"                               \
  "  .catch(() => window.test(" TOKEN_VALUE ",1))\n"                           \
  "} catch {\n"                                                                \
  "  window.test(" TOKEN_VALUE ",1)"                                           \
  "}"

} // namespace templates

/// Static test utilities class
class tester {
public:
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
  static bool values_match() { return worker_proceed().load(); };

  /// Tokenises a `window.__webview__.post` string with a test value
  static std::string post_value(const std::string &value) {
    return utility::tokeniser(TEST_POST, TOKEN_VALUE, value);
  }

  /// Dispatches -> wv.eval of a tokenised `window.__webview__.post` string with a test value
  static void post_value(engine_base *wv, const std::string &value) {
    auto js = post_value(value);
    auto do_work = [=] { wv->eval(js); };
    wv->dispatch(do_work);
  }

  /// Flags if the worker thread has initialised
  static bool worker_thread_ready() { return main_proceed().load(); }
  /// Sets if the worker thread has initialised
  static void worker_thread_ready(bool flag) {
    main_proceed().store(flag);
    main_cv().notify_one();
  }

  /// Dispatches -> wv.terminate
  static void terminate(engine_base *wv) {
    wv->dispatch([wv] { wv->terminate(); });
  }

  static std::condition_variable &worker_cv() {
    static std::condition_variable cv_;
    return cv_;
  };
  static std::condition_variable &main_cv() {
    static std::condition_variable cv_;
    return cv_;
  };

  /// Returns the given timespan in std::chrono::seconds
  static std::chrono::seconds timeout_sec(int seconds = 1) {
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
  static std::atomic_bool &main_proceed() {
    static std::atomic_bool val{};
    return val;
  }

  static void compare_values() {
    worker_proceed().store(string_expected_value() == string_value());
    worker_cv().notify_one();
  }
};
} // namespace test
} // namespace webview

#endif // defined(__cplusplus) && !defined(WEBVIEW_HEADER)
#endif // WEBVIEW_TEST_HELPER_HH
