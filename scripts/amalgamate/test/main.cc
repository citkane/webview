#include "webview_amalgamation.h"

using namespace webview::detail;

int main() {
  webview_cc w(false, nullptr);
  w.dispatch([&] { w.terminate(); });
  w.run();
  return 0;
}
