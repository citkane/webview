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

#ifndef WEBVIEW_STRINGS_STRING_MAKER_CC
#define WEBVIEW_STRINGS_STRING_MAKER_CC

#if defined(__cplusplus) && !defined(WEBVIEW_HEADER)
#include "webview/strings/string_maker.hh"
#include "webview/strings/string_api.hh"
#include "webview/strings/string_templates.hh"

using namespace webview::strings;
using namespace webview::strings::templates;

/* Frontend
 * ∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇ */

/* JS
 * ↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓ */

std::string js_string_t::onunbind(cnst_str_r name) const {
  return tokenise(js::ON_UNBIND_JS(), tokens.str, name);
}
std::string js_string_t::onbind(cnst_str_r name) const {
  return tokenise(js::ON_BIND_JS(), tokens.str, name);
}
std::string js_string_t::onreply(cnst_str_r id, int status,
                                 cnst_str_r escaped_result) const {
  strg_replacements_t replacements = {{tokens.id, id},
                                      {tokens.status, std::to_string(status)},
                                      {tokens.result, escaped_result}};
  return tokenise(js::ON_REPLY_JS(), replacements);
}
std::string js_string_t::init(cnst_str_r post_fn) const {
  return tokenise(js::WEVBIEW_INIT_JS(), tokens.post_fn, post_fn);
}
std::string js_string_t::bind(std::vector<std::string> &bound_names) const {
  auto js_names = string::json.to_list(bound_names);
  return tokenise(js::BIND_JS(), tokens.js_names, js_names);
}
std::string js_string_t::eval_wrapper(cnst_str_r user_js) const {
  return tokenise(js::EVAL_WRAPPER_JS(), tokens.user_js, user_js);
}

/* Message strings
 * ↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓ */

std::string error_message_t::reject_unbound(cnst_str_r id,
                                            cnst_str_r name) const {
  strg_replacements_t replacements = {{tokens.id, id}, {tokens.str, name}};
  return tokenise(messages::REJECT_UNBOUND_M(), replacements);
}
std::string error_message_t::uncaught_exception(cnst_str_r name,
                                                cnst_str_r what) const {
  strg_replacements_t replacements = {{tokens.str, name}, {tokens.what, what}};
  return tokenise(messages::UNCAUGHT_EXP_M(), replacements);
}
std::string error_message_t::webview_terminated(cnst_str_r name) const {
  return tokenise(messages::WEBVIEW_TERMINATED_M(), tokens.str, name);
}

/* ∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆
 * Frontend
 * ----------------------------------------------------------------------------------------------------------- 
 * Tests
 * ∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇ */

/* Tests JS
 * ↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓ */

std::string test_js_t::init(cnst_str_r init_value, bool escaped) const {
  auto init_value_js = post_value(init_value, escaped);
  return tokenise(templates::tests::js::TEST_INIT_JS(), tokens.post_fn,
                  init_value_js);
}
std::string test_js_t::post_value(cnst_str_r value, bool escaped) const {
  auto tmplt = escaped ? templates::tests::js::TEST_VALUE_WRAPPER_JS_ESCAPED()
                       : templates::tests::js::TEST_VALUE_WRAPPER_JS();
  return tokenise(tmplt, tokens.str, value);
}
std::string test_js_t::make_call_js(unsigned int result) const {
  return tokenise(templates::tests::js::TEST_MAKE_CALL_JS(), tokens.intval,
                  std::to_string(result));
}

/* Tests HTML
 * ↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓ */

std::string test_html_t::string_returns(cnst_str_r title) const {
  return tokenise(templates::tests::html::TEST_STRING_RETURNS_HTML(),
                  tokens.str, title);
}
std::string test_html_t::navigate_encoded() const {
  std::string encoding = "data:text/html,";
  std::string html =
      "%3Chtml%3EEnsure%20that%20JS%20code%20can%20call%20native%"
      "20code%20and%"
      "20vice%20versa%3C%2Fhtml%3E";
  return encoding + html;
}
std::string test_html_t::bind_unbind() const {
  return templates::tests::html::TEST_BIND_UNBIND_HTML();
}

/* ∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆
 * Tests */

#endif // defined(__cplusplus) && !defined(WEBVIEW_HEADER)
#endif // WEBVIEW_STRINGS_STRING_MAKER_CC