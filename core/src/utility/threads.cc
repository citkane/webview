#pragma once
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

#elif defined(__linux__)

#include <sys/syscall.h>
#include <unistd.h>

bool threads::is_main_thread() {
  const auto tid = static_cast<pid_t>(syscall(SYS_gettid));
  return (tid == getpid());
};

#elif defined(_WIN32)

#endif

#endif // defined(__cplusplus) && !defined(WEBVIEW_HEADER)