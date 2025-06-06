#include "webview_amalgamation.h"

int main() {
  try {
    webview_cc_t wv(true, nullptr);
    //wv.bind(
    //    "terminate",
    //    [&](cnst_str_r seq, cnst_str_r req, void *arg) { wv.terminate(); },
    //    nullptr);
    //wv.navigate("https://github.com");
    //wv.init("setTimeout(()=>{terminate();}, 5000)");
    //wv.set_title("amalgamate");
    //wv.set_html("amalgamate");
    wv.terminate();
    wv.run();
    return 0;
  } catch (...) {
    return 1;
  }
}
