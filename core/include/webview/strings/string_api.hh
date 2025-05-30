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

#ifndef WEBVIEW_STRINGS_STRING_API_HH
#define WEBVIEW_STRINGS_STRING_API_HH

#if defined(__cplusplus) && !defined(WEBVIEW_HEADER)
#include "webview/strings/string_maker.hh"
#include "webview/types/types.hh"
#include <string>

namespace webview {
namespace strings {
namespace _lib {

/*
 * Do not change any value without explixitly updating the string templates.
 * NB!!
 */

struct tokens_t {
  tokens_t() noexcept = default;
  std::string str = "_str_";
  std::string intval = "_int_";
  std::string var = "_var_";
  std::string id = "_id_";
  std::string status = "_status_";
  std::string result = "_result_";
  std::string post_fn = "_post_fn_";
  std::string js_names = "_js_names_";
  std::string user_js = "_user_js_";
  std::string what = "_what_";
};

struct sys_flags_t {
  sys_flags_t() noexcept = default;
  std::string sysop = "_sysop";
  std::string testop = "_testop";
};

struct sys_ops_t {
  sys_ops_t() noexcept = default;
  std::string dom_ready = "_dom_ready";
  std::string bind_done = "_bind_done";
  std::string unbind_done = "_unbind_done";
  std::string js_eval_start = "_frontend_eval_ready";
};

/*
 * END NB!!
 * Do not change any value without explixitly updating the string templates.
 */

} // namespace _lib

using namespace _lib;
using namespace _lib::frontend;
using namespace _lib::tests;

tokens_t const tokens{};
sys_flags_t const sys_flags{};
sys_ops_t const sys_ops{};

/// Performs string replacement for tokens.
/// @todo REGEX is probably going to be optimal for performance
std::string tokenise(cnst_str_r tmplate, cnst_str_r token,
                     cnst_str_r tkn_replcmnt) {
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
std::string tokenise(cnst_str_r tmplate, strg_replacements_t &replacements) {
  std::string tokenised_string = tmplate;
  for (auto &replacement : replacements) {
    auto string_ =
        tokenise(tokenised_string, replacement.token, replacement.tkn_replcmnt);
    tokenised_string = string_;
  };
  return tokenised_string;
}

/// String API.
class string {
public:
  static const js_string_t js;
  static const error_message_t err;
  static const json_t json;
  struct tests {
    static const test_html_t html;
    static const test_js_t js;
  };
};

const js_string_t string::js{};
const error_message_t string::err{};
const json_t string::json{};
const test_html_t string::tests::html{};
const test_js_t string::tests::js{};

} // namespace strings
} // namespace webview

#endif // defined(__cplusplus) && !defined(WEBVIEW_HEADER)
#endif // WEBVIEW_STRINGS_STRING_API_HH
