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

#define WEVBIEW_INIT_JS                                                        \
  R"(\
(function() {\
  'use strict';\
\
  function generateId() {\
    var crypto = window.crypto || window.msCrypto;\
    var bytes = new Uint8Array(16);\
    crypto.getRandomValues(bytes);\
    return Array.prototype.slice.call(bytes).map(function(n) {\
      var s = n.toString(16);\
      return ((s.length % 2) == 1 ? '0' : '') + s;\
    }).join('');\
  }\
\
  var Webview = (function() {\
    var _promises = {};\
    function Webview_() {}\
\
    Webview_.prototype.post = function(message) {\
      return (_post_fn_)(message);\
    };\
\
    Webview_.prototype.sysop = function(command) {\
      this.post(JSON.stringify({\
        id: '_sysop',\
        method: command,\
        params: []\
      }));\
    };\
\
    Webview_.prototype.call = function(method) {\
      var _id = generateId();\
      var _params = Array.prototype.slice.call(arguments, 1);\
      var promise = new Promise(function(resolve, reject) {\
        _promises[_id] = { resolve, reject };\
      });\
      this.post(JSON.stringify({\
        id: _id,\
        method: method,\
        params: _params\
      }));\
      return promise;\
    };\
\
    Webview_.prototype.onReply = function(id, status, result) {\
      var promise = _promises[id];\
      if (result !== undefined) {\
        try {\
          result = JSON.parse(result);\
        } catch (e) {\
          promise.reject(new Error('Failed to parse binding result as JSON'));\
          return;\
        }\
      }\
      if (status === 0) {\
        promise.resolve(result);\
      } else {\
        promise.reject(result);\
      }\
    };\
\
    Webview_.prototype.onBind = function(name) {\
      if (window.hasOwnProperty(name)) {\
        throw new Error(`Binding '${name}' already exists`);\
      }\
      window[name] = (function() {\
        var params = [name].concat(Array.prototype.slice.call(arguments));\
        return Webview_.prototype.call.apply(this, params);\
      }).bind(this);\
    };\
\
    Webview_.prototype.onUnbind = function(name) {\
      if (!window.hasOwnProperty(name)) {\
        throw new Error(`Binding '${name}' does not exist`);\
      }\
      delete window[name];\
    };\
    return Webview_;\
  })();\
\
  window.__webview__ = new Webview();\
  const domReadyInterval = setInterval(()=>{\
    const ready = document.readyState;\
    if (ready === 'interactive' || ready === 'complete') {\
      clearInterval(domReadyInterval);\
      window.__webview__.sysop('_dom_ready');\
    }\
  })\
})()\
)"

#define ON_BIND_JS                                                             \
  R"(\
if (window.__webview__) {\
  try {\
    window.__webview__.onBind('_str_');\
  } catch(err) {\
    console.error(err);\
  } finally {\
    window.__webview__.sysop('_bind_done');\
  }\
}\
)"

#define ON_UNBIND_JS                                                           \
  R"(\
  if (window.__webview__) {\
  try {\
    window.__webview__.onUnbind('_str_');\
  } catch (err) {\
    console.error(err);\
  } finally {\
    window.__webview__.sysop('_unbind_done');\
  }\
}\
)"

#define ON_REPLY_JS                                                            \
  R"(\
window.__webview__.onReply('_id_', _status_, _result_)\
)"

#define BIND_JS                                                                \
  R"(\
(function() {\
  'use strict';\
  var methods = _js_names_;\
  methods.forEach(function(name) {\
    window.__webview__.onBind(name);\
  });\
})()\
)"

#define EVAL_WRAPPER_JS                                                        \
  R"(\
try {\
  // User JS\
  // ↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓\
  _user_js_\
  // ↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑\
  // User JS\
\
} catch (err) {\
  console.error(err);\
} finally {\
  window.__webview__.sysop('_frontend_eval_ready');\
}\
)"

/* Message strings
 * ↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓ */

#define REJECT_UNBOUND_M                                                       \
  R"(Promise id '_id_' was rejected because function '_str_' was unbound.)"

#define UNCAUGHT_EXP_M                                                         \
  R"(Uncaught exception from native user callback function '_str_':\
_what_)"

#define WEBVIEW_TERMINATED_M                                                   \
  R"(Native user callback function '_str_' failed because Webview terminated before it could complete.)"

/* ∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆
 * Webview
 * ----------------------------------------------------------------------------------------------------------- 
 * Tests
 * ∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇ */

/* Tests HTML
 * ↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓ */

#define TEST_STRING_RETURNS_HTML                                               \
  R"(\
<html><body>\
  <div>_str_</div>\
  <script>\
    try {\
      window.loadData()\
        .then(() => window.endTest(0))\
        .catch(() => window.endTest(1));\
    } catch {\
      window.endTest(2);\
    }\
  </script>\
</body></html>\
)"

#define TEST_BIND_UNBIND_HTML                                                  \
  R"(\
<html><body>\
  <div>Test synchronous binding and unbinding</div>\
  <script>\
  window.test(0);\
  </script>\
</body></html>\
)"

/* Tests JS
 * ↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓ */

#define TEST_VALUE_WRAPPER_JS                                                  \
  R"(\
window.__webview__.post(\
  JSON.stringify({\
    id: '_testop',\
    method: '_str_'\
  })\
)\
)"

#define TEST_VALUE_WRAPPER_JS_ESCAPED                                          \
  R"(\
window.__webview__.post(\
  JSON.stringify({\
    id: '_testop',\
    method: _str_\
  })\
)\
)"

#define TEST_MAKE_CALL_JS                                                      \
  R"(\
  try {\
    console.log('calling: window.increment', _int_);\
    window.increment()\
      .then((m) => {\
        console.log(m, 'window.test', _int_);\
        console.warn(window.test);\
        window.test(_int_);\
        console.log('sent: window.test(_int_)');\
      })\
      .catch((err) => {\
        console.error(err);\
        console.log('1: window.test(_int_, 1)');\
        console.warn(window.test);\
        window.test(_int_, 1).then(m => console.log(m));\
        console.log('1: sent: window.test(_int_, 1)');\
      })\
  } catch (err) {\
    console.error(err);\
    console.log('2: window.test(_int_, 1);');\
    console.warn(window.test);\
    window.test(_int_, 1).then(m => console.log(m));\
    console.log('2: sent: window.test(_int_, 1)');\
  }\
)"

#define TEST_INIT_JS                                                           \
  R"(\
window.x = 42;\
window.onload = () => {\
  _post_fn_\
}\
)"

/* ∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆
 * Tests */

#endif // defined(__cplusplus) && !defined(WEBVIEW_HEADER)
#endif // WEBVIEW_STRINGS_STRING_TEMPLATES_HH