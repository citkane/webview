#ifndef WEBVIEW_UTILITY_THREADS_HH
#define WEBVIEW_UTILITY_THREADS_HH
#if defined(__cplusplus) && !defined(WEBVIEW_HEADER)

#include "webview/backends.hh"

namespace webview {
namespace utility {

class threads {
public:
  /// Flags if the current thread context is the main thread
  static bool is_main_thread();

  static void set_wv_instance(webview_t w);

  static webview_error_t dispatch(std::function<void()> fn);

  static void warn(std::string context);

private:
  /// The webview class instance
  static webview *wv_instance;

}; // class threads
} // namespace utility
} // namespace webview
#endif // defined(__cplusplus) && !defined(WEBVIEW_HEADER)
#endif // WEBVIEW_UTILITY_THREADS_HH