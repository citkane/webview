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
#include "webview/strings/string_api.hh"
#include "webview/types/types.hh"

using namespace webview::types;
namespace webview {
namespace strings {
namespace _templates {
namespace frontend {

inline const_str_ref WEVBIEW_INIT_JS() {
  static std::string init_js = std::string("") +
                               R"(
(function() {
    'use strict';

    function generateId() {
        var crypto = window.crypto || window.msCrypto;
        var bytes = new Uint8Array(16);
        crypto.getRandomValues(bytes);
        return Array.prototype.slice.call(bytes).map(function(n) {
            var s = n.toString(16);
            return ((s.length % 2) == 1 ? '0' : '') + s;
        }).join('');
    }

    var Webview = (function() {
        var _promises = {};
        function Webview_() {}

        Webview_.prototype.post = function(message) {
            return ()" + tokens.post_fn +
                               R"()(message);
        };

        Webview_.prototype.sysop = function(command) {
            this.post(JSON.stringify({
                id: )" + string::json.escape(sys_flags.sysop) +
                               R"(,
                method: command,
                params: []
            }));
        };

        Webview_.prototype.call = function(method) {
            var _id = generateId();
            var _params = Array.prototype.slice.call(arguments, 1);
            var promise = new Promise(function(resolve, reject) {
                _promises[_id] = { resolve, reject };
            });
            this.post(JSON.stringify({
                id: _id,
                method: method,
                params: _params
            }));
            return promise;
        };

        Webview_.prototype.onReply = function(id, status, result) {
            var promise = _promises[id];
            if (result !== undefined) {
                try {
                    result = JSON.parse(result);
                } catch (e) {
                    promise.reject(new Error("Failed to parse binding result as JSON"));
                    return;
                }
            }
            if (status === 0) {
                promise.resolve(result);
            } else {
                promise.reject(result);
            }
        };

        Webview_.prototype.onBind = function(name) {
            if (window.hasOwnProperty(name)) {
                throw new Error('Property "' + name + '" already exists');
            }
            window[name] = (function() {
                var params = [name].concat(Array.prototype.slice.call(arguments));
                return Webview_.prototype.call.apply(this, params);
            }).bind(this);
        };

        Webview_.prototype.onUnbind = function(name) {
            if (!window.hasOwnProperty(name)) {
                throw new Error('Property "' + name + '" does not exist');
            }
            delete window[name];
        };
        return Webview_;
    })();

    window.__webview__ = new Webview();
    const domReadyInterval = setInterval(()=>{
        const ready = document.readyState;
        if (ready === 'interactive' || ready === 'complete') {
            clearInterval(domReadyInterval);
            window.__webview__.sysop()" +
                               string::json.escape(sys_ops.dom_ready) +
                               R"();
        }
    })
})()
)";
  return init_js;
};

inline const_str_ref ON_BIND_JS() {
  static std::string on_bind = std::string("") + R"(
if (window.__webview__) {
    try {
        window.__webview__.onBind()" +
                               string::json.escape(tokens.str) + R"();
    } catch(err) {
        console.error(err);
    } finally {
        window.__webview__.sysop()" +
                               string::json.escape(sys_ops.bind_done) +
                               R"();
    }
})";
  return on_bind;
}

inline const_str_ref ON_UNBIND_JS() {
  static std::string on_unbind =
      std::string("") +
      R"(                                                           
if (window.__webview__) {
    try {
        window.__webview__.onUnbind()" +
      string::json.escape(tokens.str) + R"();
    } catch (err) {
        console.error(err);
    } finally {
        window.__webview__.sysop()" +
      string::json.escape(sys_ops.unbind_done) + R"();
    }
})";
  return on_unbind;
}

inline const_str_ref ON_REPLY_JS() {
  static std::string on_reply = std::string("window.__webview__.onReply(") +
                                string::json.escape(tokens.id) + ", " +
                                tokens.status + ", " + tokens.result + ")";
  return on_reply;
}

inline const_str_ref BIND_JS() {
  static std::string bind = std::string("") + R"(
(function() {
    'use strict';
    var methods = )" + tokens.js_names +
                            R"(;
    methods.forEach(function(name) {
        window.__webview__.onBind(name);
    });
})())";
  return bind;
}

inline const_str_ref EVAL_WRAPPER_JS() {
  static std::string eval_wrapper = std::string("") + R"(
try {)" + tokens.user_js + R"(
} catch (err) {
    console.error(err);
} finally {
    window.__webview__.sysop()" + string::json.escape(sys_ops.js_eval_start) +
                                    R"();
})";
  return eval_wrapper;
}

inline const_str_ref REJECT_UNBOUND_M() {
  static std::string reject_unbound_m = R"(
"Promise id )" + tokens.id + R"( was rejected because function ")" +
                                        tokens.str + R"(" was unbound.)";
  return reject_unbound_m;
}

inline const_str_ref UNCAUGHT_EXP_M() {
  static std::string uncought_exp_m = R"(
Uncaught exception from native user callback function ")" +
                                      tokens.str + R"(":
)" + tokens.what;
  return uncought_exp_m;
}

inline const_str_ref WEBVIEW_TERMINATED_M() {
  static std::string webview_terminated_m =
      R"(
Native user callback function ")" +
      tokens.str +
      R"(" failed because Webview terminated before it could complete.)";
  return webview_terminated_m;
}
} // namespace frontend

namespace tests {

inline const_str_ref TEST_STRING_RETURNS_HTML() {
  static std::string html = std::string("") + R"(
<html><body>
    <div>)" + tokens.str + R"(</div>
    <script>
        try {
            window.loadData()
                .then(() => window.endTest(0))
                .catch(() => window.endTest(1));
        } catch {
            window.endTest(2);
        }
    </script>
</body></html>)";
  return html;
}

inline const_str_ref TEST_BIND_UNBIND_HTML() {
  static std::string str = R"(
<html><body>                                   
  <div>Test synchronous binding and unbinding</div>
  <script>                                        
    window.test(0);                               
  </script>                                       
</body></html>
  )";
  return str;
}

static std::string TEST_VALUE_WRAPPER_JS(bool escaped) {
  return std::string("") + R"(
window.__webview__.post(
    JSON.stringify({
        id: )" +
         string::json.escape(sys_flags.testop) +
         R"(,
        method: )" +
         (escaped ? tokens.str : string::json.escape(tokens.str)) +
         R"(
    })
);)";
}

inline const_str_ref TEST_MAKE_CALL_JS() {
  static std::string make_call = std::string("") + R"(
    try {
        console.log("window.increment", _int_);
        window.increment()
            .then(() => { 
                console.log("window.test", _int_);
                window.test(_int_);
            })
            .catch((err) => {
                console.error(err, window.test);
                console.log("1: window.test(_int_, 1);");
                window.test(_int_, 1);
            })
    } catch (err) {
        console.error(err, window.test);
        console.log("2: window.test(_int_, 1);");
        window.test(_int_, 1);
    })";
  return make_call;
}

inline const_str_ref TEST_INIT_JS() {
  static std::string init_js = std::string("") + R"(
window.x = 42;
window.onload = () => {
    )" + tokens.post_fn + R"(
})";
  return init_js;
}

} // namespace tests
} // namespace _templates

using namespace _templates::frontend;
namespace frontend {
namespace _lib {
struct templates_js_t {
  templates_js_t() noexcept = default;
  std::string init = WEVBIEW_INIT_JS();
  std::string onbind = ON_BIND_JS();
  std::string onunbind = ON_UNBIND_JS();
  std::string onreply = ON_REPLY_JS();
  std::string bind = BIND_JS();
  std::string eval_wrapper = EVAL_WRAPPER_JS();
};

struct error_messages_t {
  error_messages_t() noexcept = default;
  std::string reject_unbound = REJECT_UNBOUND_M();
  std::string uncaught_exp = UNCAUGHT_EXP_M();
  std::string terminated = WEBVIEW_TERMINATED_M();
};

} // namespace _lib
} // namespace frontend

using namespace _templates::tests;
namespace test {
namespace _lib {
struct templates_html_t {
  templates_html_t() noexcept = default;
  std::string string_returns = TEST_STRING_RETURNS_HTML();
  std::string bind_unbind = TEST_BIND_UNBIND_HTML();
};

struct templates_js_t {
  templates_js_t() noexcept = default;
  std::string value_wrapper(bool escaped) const {
    return TEST_VALUE_WRAPPER_JS(escaped);
  }
  std::string make_call = TEST_MAKE_CALL_JS();
  std::string init = TEST_INIT_JS();
};

} // namespace _lib
} // namespace test

} // namespace strings
} // namespace webview

#endif // defined(__cplusplus) && !defined(WEBVIEW_HEADER)
#endif // WEBVIEW_STRINGS_STRING_TEMPLATES_HH