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

#define TEST_VALUE_WRAPPER_JS                                                  \
  "window.__webview__.post(\n"                                                 \
  "  JSON.stringify({\n"                                                       \
  "    id: \"" TEST_NOTIFICATION_FLAG "\",\n"                                  \
  "    method: " TOKEN_VALUE "\n"                                              \
  "  })\n"                                                                     \
  ");"

#define TEST_MAKE_CALL_JS                                                      \
  "try {\n"                                                                    \
  "  window.increment()\n"                                                     \
  "  .then(r => window.test(" TOKEN_VALUE "))\n"                               \
  "  .catch(() => window.test(" TOKEN_VALUE ",1))\n"                           \
  "} catch {\n"                                                                \
  "  window.test(" TOKEN_VALUE ",1);\n"                                        \
  "}"

#define TEST_INIT_JS                                                           \
  "window.x = 42;\n"                                                           \
  "window.onload = () => {\n"                                                  \
  " " TOKEN_POST_FN "\n"                                                       \
  "};"

#define TEST_BIND_UNBIND_HTML                                                  \
  "<html><body>\n"                                                             \
  "  <div>Test synchronous binding and unbinding</div>\n"                      \
  "  <script>\n"                                                               \
  "    window.test(0);\n"                                                      \
  "  </script>\n"                                                              \
  "</body></html>"

#define TEST_STRING_RETURNS_HTML                                               \
  "<html><body>\n"                                                             \
  "  <div>" TOKEN_VALUE "</div>\n"                                             \
  "  <script>\n"                                                               \
  "    try {\n"                                                                \
  "      window.loadData()\n"                                                  \
  "        .then(() => window.endTest(0))\n"                                   \
  "        .catch(() => window.endTest(1));\n"                                 \
  "    } catch {\n"                                                            \
  "      window.endTest(2);\n"                                                 \
  "    }\n"                                                                    \
  "  </script>\n"                                                              \
  "</body></html>"

#include "webview/detail/frontend/frontend_strings.hh"
#include "webview/log/trace_log.hh"
#include "webview/types/types.hh"
#include <atomic>
#include <chrono>
#include <condition_variable>
#include <string>

using namespace webview::strings;
using namespace webview::log;
namespace webview {
namespace detail {
namespace backend {
class engine_base;
}
} // namespace detail
namespace test {
namespace _structs {

struct js_t {
  js_t() noexcept = default;

  std::string init(str_arg_t init_value) const;

  /// Wraps a string value in evaluable JS
  std::string post_value(str_arg_t value) const;

  std::string make_call_js(unsigned int result) const;
};

struct html_t {
  html_t() noexcept = default;

  std::string bind_unbind = TEST_BIND_UNBIND_HTML;
  std::string string_returns(str_arg_t title) const;

  std::string navigate_encoded() const;
};

} // namespace _structs
using namespace webview::detail::backend;

/// Test utilities class
class tester_t {
  engine_base *wv;

public:
  tester_t(engine_base *wv) : wv(wv){};

  static const _structs::js_t js;
  static const _structs::html_t html;
  static std::condition_variable &cv() {
    static std::condition_variable cv_;
    return cv_;
  }
  static bool resolve_on_main_thread();
  static void resolve_on_main_thread(bool val);

  // NOLINTBEGIN(cppcoreguidelines-non-private-member-variables-in-classes)

  struct public_api_t {
    public_api_t(engine_base *wv) : wv(wv){};

    /// Sets the returned test value.
    void set_value(str_arg_t val);

    /// Sets the expected returned test value.
    void expect_value(str_arg_t value);

    /// Gets the returned test value.
    std::string get_value() const;

    /// Flag for evaluating if the returned value matches the expected value.
    bool values_match() const;

    /// dispatch -> wv->eval of a tokenised `window.__webview__.post` string with a test value
    void ping_value(str_arg_t value);

    /// Wraps wv->terminate in dispatch
    void terminate();

    /// Returns the given timespan in std::chrono::seconds
    std::chrono::seconds seconds(int seconds) const;

  private:
    engine_base *wv;

  } tester{wv};

  // NOLINTEND(cppcoreguidelines-non-private-member-variables-in-classes)

private:
  static std::string &string_value() {
    static std::string val;
    return val;
  }
  static std::string &string_expected_value() {
    static std::string val;
    return val;
  }
  static std::atomic_bool &worker_proceed() {
    static std::atomic_bool val{};
    return val;
  }
  static std::atomic_bool &resolve_on_main_thread_() {
    static std::atomic_bool val{};
    return val;
  }
  static void compare_values() {
    worker_proceed().store(string_expected_value() == string_value());
    cv().notify_one();
  }
};

const _structs::js_t tester_t::js{};
const _structs::html_t tester_t::html{};

} // namespace test
} // namespace webview

#endif // defined(__cplusplus) && !defined(WEBVIEW_HEADER)
#endif // WEBVIEW_TEST_HELPER_HH
