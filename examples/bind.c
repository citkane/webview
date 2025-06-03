#include "bind_lib.h"
#include "webview/lib/macros.h"
#include "webview/webview.h"

typedef struct {
  webview_t w;
  int count;
} example_ctx_t;

IGNORE_UNUSED_PARAMETERS
void compute(const char *id, const char *req, void *arg) {

  example_ctx_t *ctx = (example_ctx_t *)arg;
  sleep(3);
  webview_return(ctx->w, id, 0, random_number());
}
RESTORE_IGNORED_WARNINGS

static void count(const char *id, const char *req, void *arg) {
  example_ctx_t *ctx = (example_ctx_t *)arg;
  // We use the inbuilt Webview JSON utility from the C api.
  const char *increment_str = json_parse(req, "", 0);
  int increment = atoi(increment_str);
  int result = ctx->count += increment;
  webview_return(ctx->w, id, 0, to_string(result));
}

#ifdef _WIN32
#include <windows.h>
int WINAPI WinMain(HINSTANCE hInst, HINSTANCE hPrevInst, LPSTR lpCmdLine,
                   int nCmdShow) {
  (void)hInst;
  (void)hPrevInst;
  (void)lpCmdLine;
  (void)nCmdShow;
#else
int main(void) {
#endif
  webview_t w = webview_create(1, NULL);
  example_ctx_t context = {w, 0};

  webview_set_title(w, "Bind Example");
  webview_set_size(w, 480, 320, WEBVIEW_HINT_NONE);

  // A binding that counts up or down and immediately returns the new value.
  webview_bind(w, "count", count, &context);

  // A binding that creates a new thread and returns the result at a later time.
  webview_bind(w, "compute", compute, &context);

  webview_set_html(w, WEBVIEW_BIND_EXAMPLE_HTML);
  webview_run(w);
  return 0;
}
