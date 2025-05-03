#include "webview_amalgamation.h"
#include <stddef.h>

static void terminate_webview(webview_t w) { webview_terminate(w); }

int main(void) {
  webview_t w = webview_create(0, NULL);
  webview_dispatch(w, terminate_webview(w));
  webview_run(w);
  webview_destroy(w);
  return 0;
}
