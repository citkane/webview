#ifndef WEBVIEW_UTILITY_THREADS_CC
#define WEBVIEW_UTILITY_THREADS_CC
#if defined(__cplusplus) && !defined(WEBVIEW_HEADER)

#include "webview/utility/threads.hh"
#include "webview/errors.h"
#include "webview/utility/console.hh"

using namespace webview::utility;

void threads::set_wv_instance(webview_t w) {
  wv_instance = static_cast<webview *>(w);
};

webview_error_t threads::dispatch(std::function<void()> fn) {
  try {
    wv_instance->dispatch(fn);
    return WEBVIEW_ERROR_OK;
  } catch (...) {
    return WEBVIEW_ERROR_UNSPECIFIED;
  }
};

void threads::warn(std::string context) {
  auto message = "Calling " + context +
                 " from a child thread will likely cause undefined behaviour.";
  console::warn(message);
}

webview::webview *threads::wv_instance;

#if defined(__APPLE__)

#include <pthread.h>
bool threads::is_main_thread() { return pthread_main_np() != 0; };

#elif defined(__linux__) // defined(__APPLE__)

#include <sys/syscall.h>
#include <unistd.h>

bool threads::is_main_thread() {
  const auto tid = static_cast<pid_t>(syscall(SYS_gettid));
  return (tid == getpid());
};

#elif defined(_WIN32) // defined(__linux__)
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif // WIN32_LEAN_AND_MEAN

#include <atomic>
#include <windows.h>

namespace webview::utility {
struct unique_tag {};
template <typename Tag> struct main_thread {
  static std::atomic<DWORD> id;
  static std::atomic<bool> initialized;
};
template <typename Tag> std::atomic<DWORD> main_thread<Tag>::id{0};
template <typename Tag> std::atomic<bool> main_thread<Tag>::initialized{false};
struct capture_main_thread_t {
  capture_main_thread_t() noexcept {
    bool expected = false;
    auto &initialized = main_thread<unique_tag>::initialized;
    if (initialized.compare_exchange_strong(expected, true,
                                            std::memory_order_relaxed,
                                            std::memory_order_relaxed)) {
      main_thread<unique_tag>::id.store(GetCurrentThreadId(),
                                        std::memory_order_relaxed);
    }
  }
};
static const capture_main_thread_t capturer;
} // namespace webview::utility
namespace {
inline bool webview_utility_inline_is_main_thread() {
  return GetCurrentThreadId() ==
         main_thread<unique_tag>::id.load(std::memory_order_relaxed);
};
} // namespace

bool threads::is_main_thread() {
  return webview_utility_inline_is_main_thread();
};

#endif // #if defined(__APPLE__) #elif defined(__linux__)  #elif defined(_WIN32)
#endif // defined(__cplusplus) && !defined(WEBVIEW_HEADER)
#endif // WEBVIEW_UTILITY_THREADS_CC