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

#ifndef WEBVIEW_DETAIL_ATOMIC_API_HH
#define WEBVIEW_DETAIL_ATOMIC_API_HH

#if defined(__cplusplus) && !defined(WEBVIEW_HEADER)
#include "webview/types/types.hh"

using namespace webview::types;
namespace webview {
namespace detail {

class engine_queue;

namespace threading {
namespace _lib {

/// Nested API to get and set if the window DOM is ready
struct atomic_dom_ready_t : nested_api_t<engine_queue> {
  atomic_dom_ready_t(engine_queue *self) : nested_api_t(self) {}
  /// Query if the threading is ready to do work.
  bool ready() const;
  /// Notify the queue that the threading is ready to receive work.
  void ready(bool flag);
};

/// Nested API to get and set if various queue operations have completed
struct atomic_done_t : nested_api_t<engine_queue> {
  atomic_done_t(engine_queue *self) : nested_api_t(self) {}
  /// Gets the bind flag state
  bool bind() const;
  /// Sets the bind flag state
  void bind(bool val);
  /// Gets the unbind flag state
  bool unbind() const;
  /// Sets the unbind flag state
  void unbind(bool val);
  /// Gets the eval flag state
  bool eval() const;
  /// Sets the eval flag state
  void eval(bool val);
};

/// Root API to work with atomic flags
struct atomic_api_t : nested_api_t<engine_queue> {
  atomic_api_t(engine_queue *self) : nested_api_t(self) {}

  /// Get and set if the window DOM is ready
  atomic_dom_ready_t dom{this->self};
  /// Get and set if various queue operations have completed
  atomic_done_t done{this->self};
  /// Query if Webview is busy shutting down
  bool terminating() const;
  /// Evaluate mutiple conditions with automatic `terminating` guard
  bool AND(std::initializer_list<bool> flags) const;
};

} // namespace _lib
} // namespace threading
} // namespace detail
} // namespace webview

#endif // defined(__cplusplus) && !defined(WEBVIEW_HEADER)
#endif // WEBVIEW_DETAIL_ATOMIC_API_HH