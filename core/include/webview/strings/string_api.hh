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

_lib::strings::tokens_t const tokens{};
_lib::strings::sys_flags_t const sys_flags{};
_lib::strings::sys_ops_t const sys_ops{};

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

/// API for JS strings.
static const _lib::strings::frontend::js_string_t js{};
/// API for error message strings
static const _lib::strings::frontend::error_message_t err{};
/// API for test HTML strings
static const _lib::strings::tests::test_html_t test_html{};
/// API for test JS strings
static const _lib::strings::tests::test_js_t test_js{};

} // namespace strings
} // namespace webview

#endif // defined(__cplusplus) && !defined(WEBVIEW_HEADER)
#endif // WEBVIEW_STRINGS_STRING_API_HH
