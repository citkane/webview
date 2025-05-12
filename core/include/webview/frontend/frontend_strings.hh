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

#include "webview/detail/json.hh"
#include <string>
#include <vector>

namespace webview {
namespace utility {

#define SYSTEM_NOTIFICATION_FLAG "sysop"
#define TEST_NOTIFICATION_FLAG "tests"

// Containter: Template replacement tokens
namespace templates {

#define TOKEN_NAME "_name_"
#define TOKEN_ID "_id_"
#define TOKEN_STATUS "_status_"
#define TOKEN_RESULT "_result_"
#define TOKEN_POST_FN "_post_fn_"
#define TOKEN_JS_NAMES "_js_names_"
#define TOKEN_USER_JS "_user_js_"
#define TOKEN_WHAT "_what_"
#define TOKEN_VALUE "_value_"

} // namespace templates

/// Performs string replacement for tokens.
/// @todo REGEX is probably going to be optimal for performance
std::string tokeniser(const std::string &template_string,
                      const std::string &token,
                      const std::string &replacement) {
  if (token.empty()) {
    return template_string;
  }
  std::string tokenised_string = template_string;
  size_t start_pos = 0;
  while ((start_pos = tokenised_string.find(token, start_pos)) !=
         std::string::npos) {
    tokenised_string.replace(start_pos, token.length(), replacement);
    start_pos += replacement.length();
  }

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
    json += webview::detail::json_escape(name);
  }
  json += "]";

  return json;
}

} // namespace utility
} // namespace webview

#endif // defined(__cplusplus) && !defined(WEBVIEW_HEADER)
#endif // WEBVIEW_UTIL_FRONTEND_STRINGS_HH
