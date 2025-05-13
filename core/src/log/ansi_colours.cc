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

#ifndef WEBVIEW_LOG_ANSI_COLOURS_CC
#define WEBVIEW_LOG_ANSI_COLOURS_CC

#if defined(__cplusplus) && !defined(WEBVIEW_HEADER)
#include "webview/log/ansi_colours.hh"

namespace webview {
namespace log {
namespace _classes {

std::string ansi_t::to_ansi_string(std::initializer_list<int> codes) noexcept {
  std::string ansi_string;
  for (size_t i = 0; i < codes.size(); ++i) {
    auto code = std::to_string(*(codes.begin() + i));
    if (i == 0 && codes.size() == 1) {
      ansi_string += "\033[" + code;
    } else if (i == 0) {
      ansi_string += "\033[" + code + ";";
    } else if (i == codes.size() - 1) {
      ansi_string += code;
    } else {
      ansi_string += code + ";";
    }
  };
  ansi_string += "m";
  return ansi_string;
}

const std::string &ansi_t::yellow_s() {
  static const std::string instance = to_ansi_string({33});
  return instance;
}
const std::string &ansi_t::yellow_dim_s() {
  static const std::string instance = to_ansi_string({33, 2});
  return instance;
}
const std::string &ansi_t::green_s() {
  static const std::string instance = to_ansi_string({92});
  return instance;
}
const std::string &ansi_t::red_s() {
  static const std::string instance = to_ansi_string({91});
  return instance;
}
const std::string &ansi_t::blue_s() {
  static const std::string instance = to_ansi_string({94});
  return instance;
}
const std::string &ansi_t::blue_dark_s() {
  static const std::string instance = to_ansi_string({34});
  return instance;
}
const std::string &ansi_t::magenta_s() {
  static const std::string instance = to_ansi_string({95});
  return instance;
}
const std::string &ansi_t::default_c_s() {
  static const std::string instance = to_ansi_string({0});
  return instance;
}
const std::string &ansi_t::bold_s() {
  static const std::string instance = to_ansi_string({1});
  return instance;
}
const std::string &ansi_t::dim_s() {
  static const std::string instance = to_ansi_string({2});
  return instance;
}

} // namespace _classes
} // namespace log
} // namespace webview

#endif // defined(__cplusplus) && !defined(WEBVIEW_HEADER)
#endif // WEBVIEW_LOG_ANSI_COLOURS_CC