#include "../../amalgamate/generated/webview_amalgamation.h"

int main() {
  webview_cc_t wv(false, nullptr);
  wv.bind(
      "terminate",
      [&](cnst_str_r seq, cnst_str_r req, void *arg) { wv.terminate(); },
      nullptr);
  wv.init("terminate()");
  wv.run();
  return 0;
}
