/*
 * MIT License
 *
 * Copyright (c) 2017 Serge Zaitsev
 * Copyright (c) 2022 Steffen André Langnes
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

#ifndef WEBVIEW_TYPES_TYPES_HH
#define WEBVIEW_TYPES_TYPES_HH

#if defined(__cplusplus) && !defined(WEBVIEW_HEADER)

#include "webview/errors/errors.hh"
#include "webview/log/trace_log.hh"
#include "webview/types/basic_result.hh"
#include <functional>

using namespace webview::errors;
namespace webview {
namespace types {

enum context_t { bind_t = 'b', unbind_t = 'u', eval_t = 'e' };
struct action_ctx_t {
  context_t bind = context_t::bind_t;
  context_t unbind = context_t::unbind_t;
  context_t eval = context_t::eval_t;
};

template <typename T> struct nested_api_t {
  T *self;
  nested_api_t(T *self) : self(self) {}
};

using str_arg_t = const std::string &;

using dispatch_fn_t = std::function<void()>;

template <typename T> using result = basic_result<T, error_info, exception>;

using noresult = basic_result<void, error_info, exception>;

} // namespace types
} // namespace webview

#endif // defined(__cplusplus) && !defined(WEBVIEW_HEADER)
#endif // WEBVIEW_TYPES_TYPES_HH
