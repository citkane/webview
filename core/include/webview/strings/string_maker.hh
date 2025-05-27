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

#ifndef WEBVIEW_STRINGS_STRING_FACTORY_HH
#define WEBVIEW_STRINGS_STRING_FACTORY_HH

#if defined(__cplusplus) && !defined(WEBVIEW_HEADER)
#include "webview/strings/json.hh"
#include <vector>

namespace webview {
namespace strings {
namespace _lib {
namespace frontend {

/// Tokenised JS function strings
struct js_string_t {
  js_string_t() noexcept = default;

  /// Returns a tokenised JS function string for `unbind` which notifies that
  /// a binding was destroyed after the init script has already set things up.
  std::string onunbind(const_str_ref name) const;

  /// Returns a tokenised JS function string for `bind` which notifies that
  /// a binding was created after the init script has already set things up.
  std::string onbind(const_str_ref name) const;

  /// Returns a tokenised JS function string for a promise resolve/reject.
  std::string onreply(const_str_ref id, int status, const_str_ref result) const;

  /// Returns a tokenised JS string for the Webview frontend init function.
  std::string init(const_str_ref post_fn) const;

  /// Returns a tokenised JS string for the Webview frontend `bind` functions.
  std::string bind(std::vector<std::string> &bound_names) const;

  /// Wraps user JS to notify the native code when eval is ready.
  std::string eval_wrapper(const_str_ref user_js) const;
};

/// Tokenised error message strings
struct error_message_t {
  error_message_t() noexcept = default;

  /// Returns a tokenised error string for rejecting a promise if a callback binding was unbound.
  std::string reject_unbound(const_str_ref id, const_str_ref name) const;

  /// Returns a tokenised error string for rejecting a promise if a native callback has an uncaught exception.
  std::string uncaught_exception(const_str_ref name, const_str_ref what) const;

  /// Returns a tokenised error string for native callbacks in detached threads after webview terminates.
  std::string webview_terminated(const_str_ref name) const;
};

} // namespace frontend

namespace tests {

struct test_js_t {
  test_js_t() noexcept = default;

  std::string init(const_str_ref init_value, bool escaped = false) const;

  /// Wraps a string value in evaluable JS
  std::string post_value(const_str_ref value, bool escaped = false) const;

  std::string make_call_js(unsigned int result) const;
};

struct test_html_t {
  test_html_t() noexcept = default;

  std::string string_returns(const_str_ref title) const;

  std::string navigate_encoded() const;

  std::string bind_unbind() const;
};

} // namespace tests
} // namespace _lib
} // namespace strings
} // namespace webview

#endif // defined(__cplusplus) && !defined(WEBVIEW_HEADER)
#endif // WEBVIEW_STRINGS_STRING_FACTORY_HH
