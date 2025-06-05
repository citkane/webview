#include "webview_amalgamation.h"

using namespace webview::api;
int main() {
  webview_cc_t w(false, nullptr);
  w.dispatch([&] { w.terminate(); });
  w.run();
  return 0;
}
