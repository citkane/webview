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

#ifndef WEBVIEW_FRONTEND_ENGINE_FRONTEND_CC
#define WEBVIEW_FRONTEND_ENGINE_FRONTEND_CC

#if defined(__cplusplus) && !defined(WEBVIEW_HEADER)
#include "webview/detail/frontend/engine_frontend.hh"

using namespace webview::strings;
using namespace webview::detail::frontend::_structs;

std::string js_string_t::onunbind(str_arg_t name) const {
  return tokenise(js.onunbind, token.name, json_escape(name));
}
std::string js_string_t::onbind(str_arg_t name) const {
  return tokenise(js.onbind, token.name, json_escape(name));
}
std::string js_string_t::onreply(std::string id, int status,
                                 std::string escaped_result) const {
  return tokenise(js.onreply, {{token.id, json_escape(id)},
                               {token.status, std::to_string(status)},
                               {token.result, escaped_result}});
}
std::string js_string_t::init(str_arg_t post_fn) const {
  return tokenise(js.init, token.post_fn, post_fn);
}
std::string js_string_t::bind(std::vector<std::string> &bound_names) const {
  auto js_names = json_list(bound_names);
  return tokenise(js.bind, token.js_names, js_names);
}
std::string js_string_t::eval_wrapper(std::string user_js) const {
  return tokenise(js.eval_wrapper, token.user_js, user_js);
}

std::string error_message_t::reject_unbound(std::string id,
                                            std::string name) const {
  return tokenise(error.reject_unbound, {{token.id, id}, {token.name, name}});
}
std::string error_message_t::uncaught_exception(std::string name,
                                                std::string what) const {
  return tokenise(error.uncaught_exp, {{token.name, name}, {token.what, what}});
}
std::string error_message_t::webview_terminated(std::string name) const {
  return tokenise(error.terminated, token.name, name);
}

#endif // defined(__cplusplus) && !defined(WEBVIEW_HEADER)
#endif // WEBVIEW_FRONTEND_ENGINE_FRONTEND_CC