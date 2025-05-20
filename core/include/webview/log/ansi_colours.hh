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

#ifndef WEBVIEW_LOG_ANSI_COLOURS_HH
#define WEBVIEW_LOG_ANSI_COLOURS_HH

#if defined(__cplusplus) && !defined(WEBVIEW_HEADER)
#include "webview/types/types.hh"
#include <string>

namespace webview {
namespace log {
namespace _classes {

class ansi_t {
public:
  ~ansi_t() = default;
  ansi_t() = default;
  struct ansi_colours_t {
    std::string yellow = yellow_s();
    std::string yellow_dim = yellow_dim_s();
    std::string green = green_s();
    std::string red = red_s();
    std::string blue = blue_s();
    std::string blue_dark = blue_dark_s();
    std::string magenta = magenta_s();
    std::string default_c = default_c_s();
    std::string bold = bold_s();
    std::string dim = dim_s();
  } ansi{};

private:
  static str_arg_t yellow_s();
  static str_arg_t yellow_dim_s();
  static str_arg_t green_s();
  static str_arg_t red_s();
  static str_arg_t blue_s();
  static str_arg_t blue_dark_s();
  static str_arg_t magenta_s();
  static str_arg_t default_c_s();
  static str_arg_t bold_s();
  static str_arg_t dim_s();

  static std::string to_ansi_string(std::initializer_list<int> codes) noexcept;
};

} // namespace _classes
} // namespace log
} // namespace webview

#endif // defined(__cplusplus) && !defined(WEBVIEW_HEADER)
#endif // WEBVIEW_LOG_ANSI_COLOURS_HH