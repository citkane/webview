#pragma once
#if defined(__cplusplus) && !defined(WEBVIEW_HEADER)

#include "webview/backends.hh"
#include "webview/detail/basic_result.hh"
#include "webview/types.hh"

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
};

} // namespace utility
} // namespace webview

#endif // defined(__cplusplus) && !defined(WEBVIEW_HEADER)