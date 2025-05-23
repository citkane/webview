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
using namespace webview::strings::_structs;
using namespace webview::detail::frontend::_structs;
using replacements_t = std::initializer_list<tokenise_data_t>;

std::string js_string_t::onunbind(str_arg_t name) const {
  auto js_string = tokenise(js.onunbind, tokens.name, json_escape(name));
  return js_string;
}
std::string js_string_t::onbind(str_arg_t name) const {
  auto js_string = tokenise(js.onbind, tokens.name, json_escape(name));
  return js_string;
}
std::string js_string_t::onreply(str_arg_t id, int status,
                                 str_arg_t escaped_result) const {
  replacements_t replacements = {{tokens.id, json_escape(id)},
                                 {tokens.status, std::to_string(status)},
                                 {tokens.result, escaped_result}};
  auto js_string = tokenise(js.onreply, replacements);
  return js_string;
}
std::string js_string_t::init(str_arg_t post_fn) const {
  auto js_string = tokenise(js.init, tokens.post_fn, post_fn);
  return js_string;
}
std::string js_string_t::bind(std::vector<std::string> &bound_names) const {
  auto js_names = json_list(bound_names);
  auto js_string = tokenise(js.bind, tokens.js_names, js_names);
  return js_string;
}
std::string js_string_t::eval_wrapper(str_arg_t user_js) const {
  auto js_string = tokenise(js.eval_wrapper, tokens.user_js, user_js);
  return js_string;
}

std::string error_message_t::reject_unbound(str_arg_t id,
                                            str_arg_t name) const {
  replacements_t replacements = {{tokens.id, id}, {tokens.name, name}};
  auto js_string = tokenise(error.reject_unbound, replacements);
  return js_string;
}
std::string error_message_t::uncaught_exception(str_arg_t name,
                                                str_arg_t what) const {
  replacements_t replacements = {{tokens.name, name}, {tokens.what, what}};
  auto js_string = tokenise(error.uncaught_exp, replacements);
  return js_string;
}
std::string error_message_t::webview_terminated(str_arg_t name) const {
  auto js_string = tokenise(error.terminated, tokens.name, name);
  return js_string;
}

#endif // defined(__cplusplus) && !defined(WEBVIEW_HEADER)
#endif // WEBVIEW_FRONTEND_ENGINE_FRONTEND_CC