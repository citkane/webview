#pragma once
#include <iostream>
#if defined(__cplusplus) && !defined(WEBVIEW_HEADER)

#include "webview/utility/console.hh"

using namespace webview::utility;

void console::warn(std::string message) {
  if (!get_console()) {
    return;
  };
  std::string prefix = "[WEBVIEW][WARNING]: ";
  message = set_color(colors.yellow, prefix + message);
  printf("%s\n", message.c_str());
  release_console();
};

bool console::get_console() {
#if defined(_WIN32)
  if (!win_console.has_console) {
    return false;
  }
  freopen("CONOUT$", "w", stdout);
  freopen("CONOUT$", "w", stderr);
  std::ios::sync_with_stdio(true);
  return true;
#else
  std::cout.flush();
  return true;
#endif // defined(_WIN32) #else
};

void console::release_console() {
#if defined(_WIN32)
  if (!win_console.has_console) {
    return;
  }
  _dup2(win_console.ex_stdout, _fileno(stdout));
  _dup2(win_console.ex_sterr, _fileno(stderr));
  _close(win_console.ex_stdout);
  _close(win_console.ex_sterr);
  FreeConsole();
#endif // defined(_WIN32)
};

#if defined(_WIN32)

win_console_t console::win_console {
  _dup(_fileno(stdout));
  _dup(_fileno(stderr));
  AttachConsole(ATTACH_PARENT_PROCESS)
};

void console::print(C S::*color, const char *message) {};

#else

colors_t console::colors{33};

template <typename Color>
std::string console::set_color(Color color, std::string message) {
  message = "\x1B[" + std::to_string(color) + "m" + message + "\033[0m";
  return message;
}

#endif // defined(_WIN32) #else
#endif // defined(__cplusplus) && !defined(WEBVIEW_HEADER)