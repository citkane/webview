/*
 * MIT License
 *
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

#ifndef WEBVIEW_UTIL_FRONTEND_STRINGS_HH
#define WEBVIEW_UTIL_FRONTEND_STRINGS_HH

#if defined(__cplusplus) && !defined(WEBVIEW_HEADER)
#define SYSTEM_NOTIFICATION_FLAG "_sysop_"
#define TEST_NOTIFICATION_FLAG "_tests_"
#define TOKEN_NAME "_name_"
#define TOKEN_ID "_id_"
#define TOKEN_STATUS "_status_"
#define TOKEN_RESULT "_result_"
#define TOKEN_POST_FN "_post_fn_"
#define TOKEN_JS_NAMES "_js_names_"
#define TOKEN_USER_JS "_user_js_"
#define TOKEN_WHAT "_what_"
#define TOKEN_VALUE "_value_"

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

#include "webview/lib/json.hh"
#include "webview/types/types.hh"
#include <initializer_list>
#include <string>
#include <vector>

using namespace webview::types;

namespace webview {
namespace strings {
namespace _structs {

struct sys_flags_t {
  sys_flags_t() noexcept = default;
  std::string sysop = SYSTEM_NOTIFICATION_FLAG;
  std::string testop = TEST_NOTIFICATION_FLAG;
};

struct tokens_t {
  tokens_t() noexcept = default;
  std::string name = TOKEN_NAME;
  std::string id = TOKEN_ID;
  std::string status = TOKEN_STATUS;
  std::string result = TOKEN_RESULT;
  std::string post_fn = TOKEN_POST_FN;
  std::string js_names = TOKEN_JS_NAMES;
  std::string user_js = TOKEN_USER_JS;
  std::string what = TOKEN_WHAT;
  std::string value = TOKEN_VALUE;
};

struct tokenise_data_t {
  std::string token;
  std::string tkn_replcmnt;
};

} // namespace _structs

using replacements_t = std::initializer_list<_structs::tokenise_data_t>;
_structs::sys_flags_t const sys_flag{};
_structs::tokens_t const tokens{};

/// Performs string replacement for tokens.
/// @todo REGEX is probably going to be optimal for performance
std::string tokenise(str_arg_t tmplate, str_arg_t token,
                     str_arg_t tkn_replcmnt) {
  if (token.empty()) {
    return tmplate;
  }
  std::string tokenised_string = tmplate;
  size_t start_pos = 0;
  while ((start_pos = tokenised_string.find(token, start_pos)) !=
         std::string::npos) {
    tokenised_string.replace(start_pos, token.length(), tkn_replcmnt);
    start_pos += tkn_replcmnt.length();
  }
  return tokenised_string;
}
std::string tokenise(str_arg_t tmplate, replacements_t &replacements) {
  std::string tokenised_string = tmplate;
  for (auto &replacement : replacements) {
    auto string_ =
        tokenise(tokenised_string, replacement.token, replacement.tkn_replcmnt);
    tokenised_string = string_;
  };
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
    json += json_escape(name);
  }
  json += "]";

  return json;
}

} // namespace strings
} // namespace webview

#endif // defined(__cplusplus) && !defined(WEBVIEW_HEADER)
#endif // WEBVIEW_UTIL_FRONTEND_STRINGS_HH
