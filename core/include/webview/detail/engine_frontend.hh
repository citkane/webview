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

#ifndef WEBVIEW_DETAIL_ENGINE_FRONTEND_HH
#define WEBVIEW_DETAIL_ENGINE_FRONTEND_HH

#if defined(__cplusplus) && !defined(WEBVIEW_HEADER)

#include "webview/detail/json.hh"
#include <string>
#include <vector>

// Containter: Template tokens
namespace templates {

#define NAME_TOKEN "_name_"
#define ID_TOKEN "_id_"
#define STATUS_TOKEN "_status_"
#define RESULT_TOKEN "_result_"
#define POST_FN_TOKEN "_post_fn_"
#define JS_NAMES_TOKEN "_js_names_"
#define USER_JS_TOKEN "_user_js_"
#define WHAT_TOKEN "_what_"

#define DOM_READY_M "dom_ready"
#define BIND_DONE_M "bind_done"
#define UNBIND_DONE_M "unbind_done"
#define EVAL_READY_M "js_eval_start"

#define SYSTEM_NOTIFICATION_FLAG "sysop"

} // namespace templates

// Containter: Template for Webview JS init script
namespace templates {

#define INIT_JS                                                                \
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
  "      return (" POST_FN_TOKEN ")(message);\n"                               \
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

} // namespace templates

// Container: Templates for various JS functions and wrappers
namespace templates {

#define ON_BIND                                                                \
  "if (window.__webview__) {\n"                                                \
  "  try {\n"                                                                  \
  "    window.__webview__.onBind(" NAME_TOKEN ");\n"                           \
  "    window.__webview__.sysop(\"" BIND_DONE_M "\");\n"                       \
  "  } catch(err) {\n"                                                         \
  "    window.__webview__.sysop(\"" BIND_DONE_M "\");\n"                       \
  "  }\n"                                                                      \
  "}"
#define ON_UNBIND                                                              \
  "if (window.__webview__) {\n"                                                \
  "  try {\n"                                                                  \
  "    window.__webview__.onUnbind(" NAME_TOKEN ");\n"                         \
  "    window.__webview__.sysop(\"" UNBIND_DONE_M "\");\n"                     \
  "  } catch(err) {\n"                                                         \
  "    window.__webview__.sysop(\"" UNBIND_DONE_M "\");\n"                     \
  "  }\n"                                                                      \
  "}"
#define ON_REPLY                                                               \
  "window.__webview__.onReply(" ID_TOKEN ", " STATUS_TOKEN ", " RESULT_TOKEN ")"

#define BIND                                                                   \
  "(function() {\n"                                                            \
  " 'use strict';\n"                                                           \
  " var methods = " JS_NAMES_TOKEN ";\n"                                       \
  " methods.forEach(function(name) {\n"                                        \
  "   window.__webview__.onBind(name);\n"                                      \
  " });\n"                                                                     \
  "})()"

#define EVAL_WRAPPER                                                           \
  "try {\n"                                                                    \
  "  setTimeout(() => {\n"                                                     \
  "    window.__webview__.sysop(\"" EVAL_READY_M "\");\n"                      \
  "  });\n" USER_JS_TOKEN "\n"                                                 \
  "} catch (err) {\n"                                                          \
  "  window.__webview__.sysop(\"" EVAL_READY_M "\");\n"                        \
  "  console.error(err);\n"                                                    \
  "}"

} // namespace templates

// Container: Templates for JS error messages
namespace templates {

#define REJECT_UNBOUND_M                                                       \
  "Promise id " ID_TOKEN " was rejected because function \"" NAME_TOKEN        \
  "\" was unbound."

#define UNCAUGHT_EXP_M                                                         \
  "Uncaught exception from native user callback function \"" NAME_TOKEN        \
  "\":\n" WHAT_TOKEN

#define WEBVIEW_TERMINATED_M                                                   \
  "\nNative user callback function \"" NAME_TOKEN                              \
  "\" failed because Webview terminated before it could complete.\n\n"

} // namespace templates
namespace webview {
namespace detail {

// Container: Utility functions
namespace utility {

/// Performs string replacement for tokens.
/// @todo REGEX is probably going to be optimal for performance
std::string tokeniser(const std::string &template_string,
                      const std::string &token,
                      const std::string &replacement) {
  if (token.empty()) {
    return template_string;
  }
  std::string tokenised_string = template_string;
  size_t start_pos = 0;
  while ((start_pos = tokenised_string.find(token, start_pos)) !=
         std::string::npos) {
    tokenised_string.replace(start_pos, token.length(), replacement);
    start_pos += replacement.length();
  }

  return tokenised_string;
}

/// Parses a vector into a JSON array string.
std::string json_list(std::vector<std::string> &binding_names) {
  std::string json = "[";
  bool first = true;
  for (const auto &name : binding_names) {
    if (first) {
      first = false;
    } else {
      json += ",";
    }
    json += webview::detail::json_escape(name);
  }
  json += "]";

  return json;
}

} // namespace utility
} // namespace detail

// Container: Class API
namespace detail {
using namespace utility;

struct front_end_t {
  front_end_t() = default;

  struct js_t {
    /// Returns a tokenised JS function string for `unbind` which notifies that
    /// a binding was destroyed after the init script has already set things up.
    std::string onunbind(const std::string &name) const {
      return utility::tokeniser(ON_UNBIND, NAME_TOKEN, json_escape(name));
    }

    /// Returns a tokenised JS function string for `bind` which notifies that
    /// a binding was created after the init script has already set things up.
    std::string onbind(const std::string &name) const {
      return utility::tokeniser(ON_BIND, NAME_TOKEN, json_escape(name));
    }

    /// Returns a tokenised JS function string for a promise resolve/reject.
    std::string onreply(std::string id, int status,
                        std::string escaped_result) const {
      std::string js = utility::tokeniser(ON_REPLY, ID_TOKEN, json_escape(id));
      js = utility::tokeniser(js, STATUS_TOKEN, std::to_string(status));
      js = utility::tokeniser(js, RESULT_TOKEN, escaped_result);
      return js;
    }

    /// Returns a tokenised JS function string for the Webview backend init function.
    std::string init(const std::string &post_fn) const {
      return utility::tokeniser(INIT_JS, POST_FN_TOKEN, post_fn);
    }

    /// Returns a tokenised JS function string for the Webview backend `bind` functions.
    std::string bind(std::vector<std::string> &bound_names) const {
      auto names = utility::json_list(bound_names);
      auto js = utility::tokeniser(BIND, JS_NAMES_TOKEN, names);
      return js;
    }

    /// Wraps user JS to notify the native code when eval is ready.
    std::string eval_wrapper(std::string user_js) const {
      return utility::tokeniser(EVAL_WRAPPER, USER_JS_TOKEN, user_js);
    }
  } js{};
  struct error_message_t {
    /// Returns a tokenised error message string for rejecting a promise if a callback binding was unbound.
    std::string reject_unbound(std::string id, std::string name) const {
      auto message = utility::tokeniser(REJECT_UNBOUND_M, ID_TOKEN, id);
      message = utility::tokeniser(message, NAME_TOKEN, name);
      return message;
    }

    std::string uncaught_exception(std::string name, std::string what) const {
      auto message = utility::tokeniser(UNCAUGHT_EXP_M, NAME_TOKEN, name);
      message = utility::tokeniser(message, WHAT_TOKEN, what);
      return message;
    }

    std::string webview_terminated(std::string name) const {
      return utility::tokeniser(WEBVIEW_TERMINATED_M, NAME_TOKEN, name);
    }

  } err_message{};
};
/// API for generating HTML and JS related strings.
static const front_end_t frontend{};
} // namespace detail
} // namespace webview

#endif // defined(__cplusplus) && !defined(WEBVIEW_HEADER)
#endif // WEBVIEW_DETAIL_ENGINE_FRONTEND_HH
