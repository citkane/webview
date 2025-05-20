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

#define FRONTEND_DOM_READY "_dom_ready"
#define FRONTEND_BIND_DONE "_bind_done"
#define FRONTEND_UNBIND_DONE "_unbind_done"
#define FRONTEND_EVAL_READY "_frontend_eval_ready"

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
  "  const domReadyInterval = setInterval(()=>{\n"                             \
  "    const ready = document.readyState;\n"                                   \
  "    if (ready === 'interactive' || ready === 'complete') {\n"               \
  "      clearInterval(domReadyInterval);\n"                                   \
  "      window.__webview__.sysop(\"" FRONTEND_DOM_READY "\");\n"              \
  "    }\n"                                                                    \
  "  })\n"                                                                     \
  "})()"

#define ON_BIND                                                                \
  "if (window.__webview__) {\n"                                                \
  "  try {\n"                                                                  \
  "    window.__webview__.onBind(" TOKEN_NAME ");\n"                           \
  "  } catch(err) {\n"                                                         \
  "    console.error(err);\n"                                                  \
  "  } finally {\n"                                                            \
  "    setTimeout(() => {\n"                                                   \
  "      window.__webview__.sysop(\"" FRONTEND_BIND_DONE "\");\n"              \
  "    });\n"                                                                  \
  "  }\n"                                                                      \
  "}"
#define ON_UNBIND                                                              \
  "if (window.__webview__) {\n"                                                \
  "  try {\n"                                                                  \
  "    window.__webview__.onUnbind(" TOKEN_NAME ");\n"                         \
  "  } catch (err) {\n"                                                        \
  "    console.error(err);\n"                                                  \
  "  } finally {\n"                                                            \
  "    setTimeout(() => {\n"                                                   \
  "      window.__webview__.sysop(\"" FRONTEND_UNBIND_DONE "\");\n"            \
  "    });\n"                                                                  \
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
  "  " TOKEN_USER_JS "\n"                                                      \
  "} catch (err) {\n"                                                          \
  "  console.error(err);\n"                                                    \
  "} finally {\n"                                                              \
  "  setTimeout(() => {\n"                                                     \
  "    window.__webview__.sysop(\"" FRONTEND_EVAL_READY "\");\n"               \
  "  });\n"                                                                    \
  "}"

#define REJECT_UNBOUND_M                                                       \
  "Promise id " TOKEN_ID " was rejected because function \"" TOKEN_NAME        \
  "\" was unbound."

#define UNCAUGHT_EXP_M                                                         \
  "Uncaught exception from native user callback function \"" TOKEN_NAME        \
  "\":\n" TOKEN_WHAT

#define WEBVIEW_TERMINATED_M                                                   \
  "\nNative user callback function \"" TOKEN_NAME                              \
  "\" failed because Webview terminated before it could complete.\n\n"

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
