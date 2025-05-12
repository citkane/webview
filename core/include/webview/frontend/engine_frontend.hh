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
#include "webview/detail/json.hh"
#include <string>
#include <vector>

namespace webview {
namespace utility {
namespace signal_strings {

#define DOM_READY_M "dom_ready"
#define BIND_DONE_M "bind_done"
#define UNBIND_DONE_M "unbind_done"
#define EVAL_READY_M "js_eval_start"

} // namespace signal_strings

namespace init_js {

#define WEVBIEW_INIT_JS                                                        \
  "(function() {\n"                                                            \
  "  'use strict';\n"                                                          \
  "  function generateId() {\n"                                                \
  "    var crypto = window.crypto || window.msCrypto;\n"                       \
  "    var bytes = new Uint8Array(16);\n"                                      \
  "    crypto.getRandomValues(bytes);\n"                                       \
  "    return Array.prototype.slice.call(bytes).map(function(n) {\n"           \
  "      var s = n.toString(16);\n"                                            \
  "      return ((s.length % 2) == 1 ? '0' : '') + s;\n"                       \
  "    }).join('');\n"                                                         \
  "  }\n"                                                                      \
  "  var Webview = (function() {\n"                                            \
  "    var _promises = {};\n"                                                  \
  "    function Webview_() {}\n"                                               \
  "    Webview_.prototype.post = function(message) {\n"                        \
  "      return (" TOKEN_POST_FN ")(message);\n"                               \
  "    };\n"                                                                   \
  "    Webview_.prototype.sysop = function(command) {\n"                       \
  "      this.post(JSON.stringify({\n"                                         \
  "        id: \"" SYSTEM_NOTIFICATION_FLAG "\",\n"                            \
  "        method: command,\n"                                                 \
  "        params: []\n"                                                       \
  "      }));\n"                                                               \
  "    };\n"                                                                   \
  "    Webview_.prototype.call = function(method) {\n"                         \
  "      var _id = generateId();\n"                                            \
  "      var _params = Array.prototype.slice.call(arguments, 1);\n "           \
  "      var promise = new Promise(function(resolve, reject) {\n "             \
  "        _promises[_id] = { resolve, reject };\n"                            \
  "      });\n"                                                                \
  "      this.post(JSON.stringify({\n"                                         \
  "        id: _id,\n"                                                         \
  "        method: method,\n"                                                  \
  "        params: _params\n"                                                  \
  "      }));\n"                                                               \
  "      return promise;\n"                                                    \
  "    };\n"                                                                   \
  "    Webview_.prototype.onReply = function(id, status, result) {\n"          \
  "      var promise = _promises[id];\n"                                       \
  "      if (result !== undefined) {\n"                                        \
  "        try {\n"                                                            \
  "          result = JSON.parse(result);\n"                                   \
  "        } catch (e) {\n"                                                    \
  "          promise.reject(new Error(\"Failed to parse binding result as "    \
  "JSON\"));\n"                                                                \
  "          return;\n"                                                        \
  "        }\n"                                                                \
  "      }\n"                                                                  \
  "      if (status === 0) {\n"                                                \
  "        promise.resolve(result);\n"                                         \
  "      } else {\n"                                                           \
  "        promise.reject(result);\n"                                          \
  "      }\n"                                                                  \
  "    };\n"                                                                   \
  "    Webview_.prototype.onBind = function(name) {\n"                         \
  "      if (window.hasOwnProperty(name)) {\n"                                 \
  "        throw new Error('Property \"' + name + '\" already exists');\n"     \
  "      }\n"                                                                  \
  "      window[name] = (function() {\n"                                       \
  "        var params = "                                                      \
  "[name].concat(Array.prototype.slice.call(arguments));\n"                    \
  "        return Webview_.prototype.call.apply(this, params);\n"              \
  "      }).bind(this);\n"                                                     \
  "    };\n"                                                                   \
  "    Webview_.prototype.onUnbind = function(name) {\n"                       \
  "      if (!window.hasOwnProperty(name)) {\n"                                \
  "        throw new Error('Property \"' + name + '\" does not exist');\n"     \
  "      }\n"                                                                  \
  "      delete window[name];\n"                                               \
  "    };\n"                                                                   \
  "    return Webview_;\n"                                                     \
  "  })();\n"                                                                  \
  "  window.__webview__ = new Webview();\n"                                    \
  "  window.__webview__.sysop(\"" DOM_READY_M "\");\n"                         \
  "})()"

} // namespace init_js

namespace js_functions {

#define ON_BIND                                                                \
  "if (window.__webview__) {\n"                                                \
  "  try {\n"                                                                  \
  "    window.__webview__.onBind(" TOKEN_NAME ");\n"                           \
  "    window.__webview__.sysop(\"" BIND_DONE_M "\");\n"                       \
  "  } catch(err) {\n"                                                         \
  "    window.__webview__.sysop(\"" BIND_DONE_M "\");\n"                       \
  "  }\n"                                                                      \
  "}"
#define ON_UNBIND                                                              \
  "if (window.__webview__) {\n"                                                \
  "  try {\n"                                                                  \
  "    window.__webview__.onUnbind(" TOKEN_NAME ");\n"                         \
  "    window.__webview__.sysop(\"" UNBIND_DONE_M "\");\n"                     \
  "  } catch(err) {\n"                                                         \
  "    window.__webview__.sysop(\"" UNBIND_DONE_M "\");\n"                     \
  "  }\n"                                                                      \
  "}"
#define ON_REPLY                                                               \
  "window.__webview__.onReply(" TOKEN_ID ", " TOKEN_STATUS ", " TOKEN_RESULT ")"

#define BIND                                                                   \
  "(function() {\n"                                                            \
  " 'use strict';\n"                                                           \
  " var methods = " TOKEN_JS_NAMES ";\n"                                       \
  " methods.forEach(function(name) {\n"                                        \
  "   window.__webview__.onBind(name);\n"                                      \
  " });\n"                                                                     \
  "})()"

#define EVAL_WRAPPER                                                           \
  "try {\n"                                                                    \
  "  setTimeout(() => {\n"                                                     \
  "    window.__webview__.sysop(\"" EVAL_READY_M "\");\n"                      \
  "  });\n" TOKEN_USER_JS "\n"                                                 \
  "} catch (err) {\n"                                                          \
  "  window.__webview__.sysop(\"" EVAL_READY_M "\");\n"                        \
  "  console.error(err);\n"                                                    \
  "}"

} // namespace js_functions

// Container: Templates for JS error messages
namespace error_messages {

#define REJECT_UNBOUND_M                                                       \
  "Promise id " TOKEN_ID " was rejected because function \"" TOKEN_NAME        \
  "\" was unbound."

#define UNCAUGHT_EXP_M                                                         \
  "Uncaught exception from native user callback function \"" TOKEN_NAME        \
  "\":\n" TOKEN_WHAT

#define WEBVIEW_TERMINATED_M                                                   \
  "\nNative user callback function \"" TOKEN_NAME                              \
  "\" failed because Webview terminated before it could complete.\n\n"

} // namespace error_messages

struct front_end_t {
  ~front_end_t() = default;
  front_end_t() = default;

  struct js_string_t {
    /// Returns a tokenised JS function string for `unbind` which notifies that
    /// a binding was destroyed after the init script has already set things up.
    std::string onunbind(const std::string &name) const {
      return tokeniser(ON_UNBIND, TOKEN_NAME, detail::json_escape(name));
    }

    /// Returns a tokenised JS function string for `bind` which notifies that
    /// a binding was created after the init script has already set things up.
    std::string onbind(const std::string &name) const {
      return tokeniser(ON_BIND, TOKEN_NAME, detail::json_escape(name));
    }

    /// Returns a tokenised JS function string for a promise resolve/reject.
    std::string onreply(std::string id, int status,
                        std::string escaped_result) const {
      std::string js_string =
          tokeniser(ON_REPLY, TOKEN_ID, detail::json_escape(id));
      js_string = tokeniser(js_string, TOKEN_STATUS, std::to_string(status));
      js_string = tokeniser(js_string, TOKEN_RESULT, escaped_result);
      return js_string;
    }

    /// Returns a tokenised JS function string for the Webview backend init function.
    std::string init(const std::string &post_fn) const {
      return tokeniser(WEVBIEW_INIT_JS, TOKEN_POST_FN, post_fn);
    }

    /// Returns a tokenised JS function string for the Webview backend `bind` functions.
    std::string bind(std::vector<std::string> &bound_names) const {
      auto names = json_list(bound_names);
      auto js_string = tokeniser(BIND, TOKEN_JS_NAMES, names);
      return js_string;
    }

    /// Wraps user JS to notify the native code when eval is ready.
    std::string eval_wrapper(std::string user_js) const {
      return tokeniser(EVAL_WRAPPER, TOKEN_USER_JS, user_js);
    }
  } js{};

  struct error_message_t {
    /// Returns a tokenised error string for rejecting a promise if a callback binding was unbound.
    std::string reject_unbound(std::string id, std::string name) const {
      auto message = tokeniser(REJECT_UNBOUND_M, TOKEN_ID, id);
      message = tokeniser(message, TOKEN_NAME, name);
      return message;
    }

    /// Returns a tokenised error string for rejecting a promise if a native callback has an uncaught exception.
    std::string uncaught_exception(std::string name, std::string what) const {
      auto message = tokeniser(UNCAUGHT_EXP_M, TOKEN_NAME, name);
      message = tokeniser(message, TOKEN_WHAT, what);
      return message;
    }

    /// Returns a tokenised error string for native callbacks in detached threads after webview terminates.
    std::string webview_terminated(std::string name) const {
      return tokeniser(WEBVIEW_TERMINATED_M, TOKEN_NAME, name);
    }

  } err_message{};
};

/// API for generating HTML and JS related strings.
static const front_end_t frontend{};

} // namespace utility
} // namespace webview

#endif // defined(__cplusplus) && !defined(WEBVIEW_HEADER)
#endif // WEBVIEW_FRONTEND_ENGINE_FRONTEND_HH
