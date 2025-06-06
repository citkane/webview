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

#ifndef WEBVIEW_STRINGS_STRING_TEMPLATES_HH
#define WEBVIEW_STRINGS_STRING_TEMPLATES_HH

#if defined(__cplusplus) && !defined(WEBVIEW_HEADER)

/* Webview
 * ∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇ */

/* JS
 * ↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓ */

#define TEMPLATE_WEVBIEW_INIT_JS                                               \
  "\n\
(function() {\n\
  'use strict';\n\
\n\
  function generateId() {\n\
    var crypto = window.crypto || window.msCrypto;\n\
    var bytes = new Uint8Array(16);\n\
    crypto.getRandomValues(bytes);\n\
    return Array.prototype.slice.call(bytes).map(function(n) {\n\
      var s = n.toString(16);\n\
      return ((s.length % 2) == 1 ? '0' : '') + s;\n\
    }).join('');\n\
  }\n\
\n\
  var Webview = (function() {\n\
    var _promises = {};\n\
    function Webview_() {}\n\
\n\
    Webview_.prototype.post = function(message) {\n\
      return (_post_fn_)(message);\n\
    };\n\
\n\
    Webview_.prototype.sysop = function(command) {\n\
      this.post(JSON.stringify({\n\
        id: '_sysop',\n\
        method: command,\n\
        params: []\n\
      }));\n\
    };\n\
\n\
    Webview_.prototype.call = function(method) {\n\
      var _id = generateId();\n\
      var _params = Array.prototype.slice.call(arguments, 1);\n\
      var promise = new Promise(function(resolve, reject) {\n\
        _promises[_id] = { resolve, reject };\n\
      });\n\
      this.post(JSON.stringify({\n\
        id: _id,\n\
        method: method,\n\
        params: _params\n\
      }));\n\
      return promise;\n\
    };\n\
\n\
    Webview_.prototype.onReply = function(id, status, result) {\n\
      var promise = _promises[id];\n\
      if (result !== undefined) {\n\
        try {\n\
          result = JSON.parse(result);\n\
        } catch (e) {\n\
          promise.reject(new Error('Failed to parse binding result as JSON'));\n\
          return;\n\
        }\n\
      }\n\
      if (status === 0) {\n\
        promise.resolve(result);\n\
      } else {\n\
        promise.reject(result);\n\
      }\n\
    };\n\
\n\
    Webview_.prototype.onBind = function(name) {\n\
      if (window.hasOwnProperty(name)) {\n\
        throw new Error(`Binding '${name}' already exists`);\n\
      }\n\
      window[name] = (function() {\n\
        var params = [name].concat(Array.prototype.slice.call(arguments));\n\
        return Webview_.prototype.call.apply(this, params);\n\
      }).bind(this);\n\
    };\n\
\n\
    Webview_.prototype.onUnbind = function(name) {\n\
      if (!window.hasOwnProperty(name)) {\n\
        throw new Error(`Binding '${name}' does not exist`);\n\
      }\n\
      delete window[name];\n\
    };\n\
    return Webview_;\n\
  })();\n\
\n\
  window.__webview__ = new Webview();\n\
  window.__webview__.sysop('_webview_ready');\n\
\n\
  const domReadyInterval = setInterval(()=>{\n\
    const ready = document.readyState;\n\
    if (ready === 'interactive' || ready === 'complete') {\n\
      clearInterval(domReadyInterval);\n\
      window.__webview__.sysop('_dom_ready');\n\
    }\n\
  })\n\
\n\
})()\n\
"

#define TEMPLATE_ON_BIND_JS                                                    \
  "\n\
if (window.__webview__) {\n\
  try {\n\
    window.__webview__.onBind('_str_');\n\
  } catch(err) {\n\
    console.error(err);\n\
  } finally {\n\
    window.__webview__.sysop('_bind_done');\n\
  }\n\
}\n\
"

#define TEMPLATE_ON_UNBIND_JS                                                  \
  "\n\
if (window.__webview__) {\n\
  try {\n\
    window.__webview__.onUnbind('_str_');\n\
  } catch (err) {\n\
    console.error(err);\n\
  } finally {\n\
    window.__webview__.sysop('_unbind_done');\n\
  }\n\
}\n\
"

#define TEMPLATE_ON_REPLY_JS                                                   \
  "\n\
window.__webview__.onReply('_id_', _status_, _result_)\n\
"

#define TEMPLATE_BIND_JS                                                       \
  "\n\
(function() {\n\
  'use strict';\n\
  var methods = _js_names_;\n\
  methods.forEach(function(name) {\n\
    window.__webview__.onBind(name);\n\
  });\n\
})()\n\
"

#define TEMPLATE_EVAL_WRAPPER_JS                                               \
  "\n\
try {\n\
  // User JS\n\
  // ↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓\n\
  _user_js_\n\
  // ↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑\n\
  // User JS\n\
\n\
} catch (err) {\n\
  console.error(err);\n\
} finally {\n\
  window.__webview__.sysop('_frontend_eval_ready');\n\
}\n\
"

#define TEMPLATE_USER_INIT_WRAPPER_JS                                          \
  "\n\
try {\n\
  // User JS\n\
  // ↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓\n\
  _user_js_\n\
  // ↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑\n\
  // User JS\n\
\n\
} catch (err) {\n\
  console.error(err);\n\
}\n\
"

/* Message strings
 * ↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓ */

#define TEMPLATE_REJECT_UNBOUND_M                                              \
  "Promise id '_id_' was rejected because function '_str_' was unbound."

#define TEMPLATE_UNCAUGHT_EXP_M                                                \
  "Uncaught exception from native user callback function '_str_':\n\
_what_"

#define TEMPLATE_WEBVIEW_TERMINATED_M                                          \
  "Native user callback function '_str_' failed because Webview terminated "   \
  "before it could complete."

/* ∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆
 * Webview
 * ----------------------------------------------------------------------------------------------------------- 
 * Tests
 * ∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇ */

/* Tests HTML
 * ↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓ */

#define TEMPLATE_TEST_STRING_RETURNS_HTML                                      \
  "\n\
<html><body>\n\
  <div>_str_</div>\n\
  <script>\n\
  try {\n\
    window.loadData()\n\
      .then(() => window.endTest(0))\n\
      .catch((err) => {\n\
        console.error(err);\n\
        window.endTest(1);\n\
      });\n\
  } catch (err) {\n\
    console.error(err);\n\
    window.endTest(2);\n\
  }\n\
  </script>\n\
</body></html>\n\
"

/* Tests JS
 * ↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓ */

#define TEMPLATE_TEST_BIND_UNBIND_INIT_JS                                      \
  "\n\
window.test(0)\n\
  .then(m => console.log(m))\n\
  .catch(err => console.error(err));\n\
"

#define TEMPLATE_TEST_VALUE_WRAPPER_JS                                         \
  "\n\
window.__webview__.post(\n\
  JSON.stringify({\n\
    id: '_testop',\n\
    method: '_str_'\n\
  })\n\
)\n\
"

#define TEMPLATE_TEST_VALUE_WRAPPER_ESCAPED_JS                                 \
  "\n\
window.__webview__.post(\n\
  JSON.stringify({\n\
    id: '_testop',\n\
    method: _str_\n\
  })\n\
)\n\
"

#define TEMPLATE_TEST_BIND_UNBIND_JS                                           \
  "\n\
try {\n\
  window.increment()\n\
    .then(m => {\n\
      console.log(m);\n\
      window.test(_int_)\n\
        .then(m => console.log(m))\n\
        .catch(err => console.error(err));\n\
    })\n\
    .catch(err => {\n\
      console.error(err);\n\
      window.test(_int_, 1)\n\
        .then(m => console.log(m))\n\
        .catch(err => console.error(err));\n\
    })\n\
} catch (err) {\n\
  console.error(err);\n\
  window.test(_int_, 1)\n\
    .then(m => console.log(m))\n\
    .catch(err => console.error(err));\n\
}\n\
"

#define TEMPLATE_TEST_ONLOAD_JS                                                \
  "\n\
window.x = 42;\n\
window.onload = () => {\n\
  _post_fn_\n\
}\n\
"

/* ∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆
 * Tests */

#endif // defined(__cplusplus) && !defined(WEBVIEW_HEADER)
#endif // WEBVIEW_STRINGS_STRING_TEMPLATES_HH