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
_structs::tokens_t const token{};

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
