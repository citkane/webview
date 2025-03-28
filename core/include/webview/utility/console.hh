#pragma once
#if defined(__cplusplus) && !defined(WEBVIEW_HEADER)

#include <string>

#if defined(_WIN32)
struct win_console_t {
  int ex_stdout;
  int ex_stderr;
  bool has_console;
};
#endif // defined(_WIN32)

struct colors_t {
  int yellow;
};

namespace webview {
namespace utility {

class console {
public:
  static void warn(std::string message);
  /** TODO
   *
   * Add appropriately colorised console methods for future
   * refactoring into Webview 
   *
   * static void error(std::string &message);
   * static void log(std::string &message);
   */

private:
  static bool get_console();

  static void release_console();

  template <typename Color>
  static std::string set_color(Color color, std::string message);

  static colors_t colors;

#if defined(_WIN32)
  static win_console_t win_console;
#endif // defined(_WIN32)
};

} // namespace utility
} // namespace webview

#endif // defined(__cplusplus) && !defined(WEBVIEW_HEADER)