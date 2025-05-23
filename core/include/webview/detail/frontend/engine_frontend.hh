/*
 * MIT License
 *
 * Copyright (c) 2017 Serge Zaitsev
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

#ifndef WEBVIEW_FRONTEND_ENGINE_FRONTEND_HH
#define WEBVIEW_FRONTEND_ENGINE_FRONTEND_HH

#if defined(__cplusplus) && !defined(WEBVIEW_HEADER)

#include "frontend_strings.hh"
#include <string>
#include <vector>

namespace webview {
namespace detail {
namespace frontend {
namespace _structs {

struct templates_t {
  templates_t() noexcept = default;
  std::string init = WEVBIEW_INIT_JS;
  std::string onbind = ON_BIND;
  std::string onunbind = ON_UNBIND;
  std::string onreply = ON_REPLY;
  std::string bind = BIND;
  std::string eval_wrapper = EVAL_WRAPPER;
};

struct error_messages_t {
  error_messages_t() noexcept = default;
  std::string reject_unbound = REJECT_UNBOUND_M;
  std::string uncaught_exp = UNCAUGHT_EXP_M;
  std::string terminated = WEBVIEW_TERMINATED_M;
};

struct sysops_t {
  sysops_t() noexcept = default;
  std::string dom_ready = FRONTEND_DOM_READY;
  std::string bind_done = FRONTEND_BIND_DONE;
  std::string unbind_done = FRONTEND_UNBIND_DONE;
  std::string js_eval_start = FRONTEND_EVAL_READY;
};

struct js_string_t {
  js_string_t(_structs::templates_t const js) : js(js){};

  /// Returns a tokenised JS function string for `unbind` which notifies that
  /// a binding was destroyed after the init script has already set things up.
  std::string onunbind(str_arg_t name) const;

  /// Returns a tokenised JS function string for `bind` which notifies that
  /// a binding was created after the init script has already set things up.
  std::string onbind(str_arg_t name) const;

  /// Returns a tokenised JS function string for a promise resolve/reject.
  std::string onreply(str_arg_t id, int status, str_arg_t result) const;

  /// Returns a tokenised JS string for the Webview frontend init function.
  std::string init(str_arg_t post_fn) const;

  /// Returns a tokenised JS string for the Webview frontend `bind` functions.
  std::string bind(std::vector<std::string> &bound_names) const;

  /// Wraps user JS to notify the native code when eval is ready.
  std::string eval_wrapper(str_arg_t user_js) const;

private:
  _structs::templates_t const js;
};

struct error_message_t {
  error_message_t(_structs::error_messages_t const error) : error(error){};

  /// Returns a tokenised error string for rejecting a promise if a callback binding was unbound.
  std::string reject_unbound(str_arg_t id, str_arg_t name) const;

  /// Returns a tokenised error string for rejecting a promise if a native callback has an uncaught exception.
  std::string uncaught_exception(str_arg_t name, str_arg_t what) const;

  /// Returns a tokenised error string for native callbacks in detached threads after webview terminates.
  std::string webview_terminated(str_arg_t name) const;

private:
  _structs::error_messages_t const error;
};

} // namespace _structs

namespace _classes {

// NOLINTBEGIN(cppcoreguidelines-non-private-member-variables-in-classes)
class front_end_t {
  _structs::templates_t const js_templates{};
  _structs::error_messages_t const error_messages{};

public:
  front_end_t() noexcept = default;

  /// Javascript string factory
  _structs::js_string_t js{js_templates};

  /// Error message string factory
  _structs::error_message_t err_message{error_messages};

  /// Frontend to backend system operation signals.
  _structs::sysops_t sysops{};
};

// NOLINTEND(cppcoreguidelines-non-private-member-variables-in-classes)
} // namespace _classes

/// API for HTML and JS related strings.
static const _classes::front_end_t front_end{};

} // namespace frontend
} // namespace detail
} // namespace webview

#endif // defined(__cplusplus) && !defined(WEBVIEW_HEADER)
#endif // WEBVIEW_FRONTEND_ENGINE_FRONTEND_HH
