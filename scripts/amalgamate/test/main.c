#include "webview_amalgamation.h"
#include <stddef.h>

//void terminate(const char *seq, const char *req, void *arg) {
//  webview_t w = (webview_t)arg;
//  webview_terminate(w);
//}
int main(void) {
  webview_t w = webview_create(0, NULL);
  //webview_bind(w, "terminate", terminate, w);
  //webview_init(w, "terminate()");
  webview_terminate(w);
  webview_run(w);
  return 0;
}
