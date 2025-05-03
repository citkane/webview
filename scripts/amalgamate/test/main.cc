#include "webview_amalgamation.h"

int main() {
  webview::webview w(false, nullptr);
  w.dispatch([&] { w.terminate(); });
  w.run();
  return 0;
}
