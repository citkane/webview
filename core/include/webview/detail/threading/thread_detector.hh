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

#ifndef WEBVIEW_DETAIL_THREADING_THREAD_DETECTOR_HH
#define WEBVIEW_DETAIL_THREADING_THREAD_DETECTOR_HH

#if defined(__cplusplus) && !defined(WEBVIEW_HEADER)
#include "webview/lib/macros.h"
#include <atomic>

namespace webview {
namespace detail {
namespace threading {

#if defined(WEBVIEW_PLATFORM_WINDOWS)
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif

#include <windows.h>

namespace _lib {
class thread_detector_t {
public:
  thread_detector_t() noexcept { main_thread_id = GetCurrentThreadId(); }

  static bool is_main_thread() {
    return main_thread_id == GetCurrentThreadId();
  }

private:
  static DWORD main_thread_id;
};

} // namespace _lib

static const _lib::thread_detector_t thread{};

#endif

#if defined(WEBVIEW_PLATFORM_LINUX)
#include <sys/syscall.h>
#include <unistd.h>

class thread {
public:
  static bool is_main_thread() { return syscall(SYS_gettid) == getpid(); };
};

#endif

#if defined(WEBVIEW_PLATFORM_DARWIN)
#include <pthread.h>

class thread {
public:
  static bool is_main_thread() { return pthread_main_np() != 0; };
};

#endif

} // namespace threading
} // namespace detail
} // namespace webview

#endif // defined(__cplusplus) && !defined(WEBVIEW_HEADER)
#endif // WEBVIEW_DETAIL_THREADING_THREAD_DETECTOR_HH