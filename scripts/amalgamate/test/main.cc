#include "../../amalgamate/generated/webview_amalgamation.h"

using namespace webview::api;
int main() {
  webview_cc_t w(false, nullptr);
  w.terminate();
  w.run();
  return 0;
}
