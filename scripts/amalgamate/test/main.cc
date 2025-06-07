#include "../generated/webview_amalgamation.h"

#ifdef _WIN32
int WINAPI WinMain(HINSTANCE /*hInst*/, HINSTANCE /*hPrevInst*/,
                   LPSTR /*lpCmdLine*/, int /*nCmdShow*/) {
#else
int main() {
#endif
  try {
    webview_cc_t wv(true, nullptr);
    wv.terminate();
    wv.run();
    return 0;
  } catch (...) {
    return 1;
  }
}
