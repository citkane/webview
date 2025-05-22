#include "webview/test_driver.hh"
#include "webview/types/types.hh"

#define WEBVIEW_VERSION_MAJOR 1
#define WEBVIEW_VERSION_MINOR 2
#define WEBVIEW_VERSION_PATCH 3
#define WEBVIEW_VERSION_PRE_RELEASE "-test"
#define WEBVIEW_VERSION_BUILD_METADATA "+gaabbccd"

#include "../include/test_helper.hh"
#include "webview/webview.h"

#include <cassert>
#include <cstdint>

// This test should only run on Windows to enable us to perform a controlled
// "warm-up" of MS WebView2 in order to avoid the initial test from
// occationally timing out in CI.
#ifdef WEBVIEW_PLATFORM_WINDOWS
#include <iostream>

TEST_CASE("# Warm-up") {
  // Signal to the test runner that this may be a slow test.
  std::cerr << "[[slow]]" << std::endl; // NOLINT(performance-avoid-endl)
  webview::webview w(false, nullptr);
  w.dispatch([&]() { w.terminate(); });
  w.run();
}
#endif

using namespace webview::detail;
using namespace webview::test;
using namespace webview::types;
using namespace webview::log;
using namespace webview::strings;

TEST_CASE("Start app loop and terminate it") {
  webview::webview w(false, nullptr);
  w.dispatch([&]() { w.terminate(); });
  w.run();
}

void cb_assert_arg(webview_t w, void *arg) {
  REQUIRE(w != nullptr);
  REQUIRE(memcmp(arg, "arg", 3) == 0);
}

void cb_terminate(webview_t w, void *arg) {
  REQUIRE(arg == nullptr);
  webview_terminate(w);
}

TEST_CASE("Use C API to create a window, run app and terminate it") {
  webview_t w;
  w = webview_create(false, nullptr);
  webview_set_size(w, 480, 320, WEBVIEW_HINT_NONE);
  webview_set_title(w, "Test");
  webview_set_html(w, "set_html ok");
  webview_navigate(w, "data:text/plain,navigate%20ok");
  webview_dispatch(w, cb_assert_arg, (void *)"arg");
  webview_dispatch(w, cb_terminate, nullptr);
  webview_run(w);
  webview_destroy(w);
}

struct c_context_t {
  webview_t w;
  unsigned int number;
  void (*increment)(const char *, const char *, void *);
};
void cb_increment(const char *seq, const char * /*req*/, void *arg) {
  auto *ctx = static_cast<c_context_t *>(arg);
  ++ctx->number;
  webview_return(ctx->w, seq, 0, "");
}
void cb_bind_increment(void *w, void *arg) {
  auto ctx = static_cast<context_t *>(arg);
  webview_bind(w, "increment", cb_increment, ctx);
};
void cb_unbind_increment(void *w, void * /*arg*/) {
  webview_unbind(w, "increment");
};
void cb_eval_value1(void *w, void * /*arg*/) {
  webview_eval(w, tester_t::js.make_call_js(1).c_str());
};
void cb_eval_value2(void *w, void * /*arg*/) {
  webview_eval(w, tester_t::js.make_call_js(2).c_str());
};
void cb_eval_value3(void *w, void * /*arg*/) {
  webview_eval(w, tester_t::js.make_call_js(3).c_str());
};
TEST_CASE("Use C API to test binding and unbinding") {

  c_context_t context{};

  //  context.increment = +[](const char *seq, const char * /*req*/, void *arg) {
  //    auto *ctx = static_cast<context_t *>(arg);
  //    ++ctx->number;
  //    webview_return(ctx->w, seq, 0, "");
  //  };

  auto tests = +[](const char *seq, const char *req, void *arg) {
    auto context = static_cast<c_context_t *>(arg);
    std::string req_(req);

    // User defined native callback functions are called from a child thread,
    // so we use dispatch to get back on the main thread.
    // @todo Guarantee thread safety

    // Bind and increment number.
    if (req_ == "[0]") {
      REQUIRE(context->number == 0);

      webview_dispatch(context->w, cb_bind_increment, arg);
      webview_dispatch(context->w, cb_eval_value1, nullptr);
      webview_return(context->w, seq, 0, "");
      return;
    }
    // Unbind and make sure that we cannot increment even if we try.
    if (req_ == "[1]") {
      REQUIRE(context->number == 1);

      webview_dispatch(context->w, cb_unbind_increment, nullptr);
      webview_dispatch(context->w, cb_eval_value2, nullptr);
      webview_return(context->w, seq, 0, "");
      return;
    }
    // Number should not have changed but we can bind again and change the number.
    if (req_ == "[2,1]") {
      REQUIRE(context->number == 1);

      webview_dispatch(context->w, cb_bind_increment, arg);
      webview_dispatch(context->w, cb_eval_value3, nullptr);

      webview_return(context->w, seq, 0, "");
      return;
    }
    // Finish test.
    if (req_ == "[3]") {
      REQUIRE(context->number == 2);

      webview_terminate(context->w);
      return;
    }
    REQUIRE(!"Should not reach here");
  };

  auto w = webview_create(1, nullptr);
  context.w = w;
  webview_set_html(w, "Use C API to test binding and unbinding");
  // Attempting to remove non-existing binding is OK
  webview_unbind(w, "test");
  webview_bind(w, "test", tests, &context);
  // Attempting to bind multiple times only binds once
  webview_bind(w, "test", tests, &context);
  webview_eval(w, "window.test(0);");
  webview_run(w);
  webview_destroy(w);
}

TEST_CASE("Test synchronous binding and unbinding") {

  webview::webview w(true, nullptr);

  auto js = tester_t::js;
  auto html = tester_t::html;
  unsigned int number = 0;

  auto increment = [&](str_arg_t /*req*/) -> std::string {
    ++number;
    return "";
  };
  auto tests = [&](str_arg_t req) -> std::string {
    tester_t::resolve_on_main_thread(true);

    // Bind and increment number.
    if (req == "[0]") {
      REQUIRE(number == 0);
      w.bind("increment", increment);
      w.eval(js.make_call_js(1), true);
      return "";
    }

    // Unbind and make sure that we cannot increment even if we try.
    if (req == "[1]") {
      REQUIRE(number == 1);
      w.unbind("increment", true);
      w.eval(js.make_call_js(2), true);
      return "";
    }
    // We should have gotten an error on the JS side.
    // Number should not have changed but we can bind again and change the number.
    if (req == "[2,1]") {
      REQUIRE(number == 1);
      w.bind("increment", increment);
      w.eval(js.make_call_js(3), true);
      return "";
    }
    tester_t::resolve_on_main_thread(false);
    // Finish test.
    if (req == "[3]") {
      REQUIRE(number == 2);
      w.terminate();
      return "";
    }
    REQUIRE(!"Should not reach here");

    return "";
  };

  // Attempting to remove non-existing binding is OK
  w.unbind("test", true);
  w.bind("test", tests);
  // Attempting to bind multiple times only binds once
  w.bind("test", tests);
  w.set_html(html.bind_unbind);
  w.run();
}

TEST_CASE("The string returned from a binding call must be JSON") {
  webview::webview w(true, nullptr);
  auto html = tester_t::html;

  w.bind("loadData",
         [](str_arg_t /*req*/) -> std::string { return "\"hello\""; });
  w.bind("endTest", [&](str_arg_t req) -> std::string {
    REQUIRE(req != "[2]");
    REQUIRE(req != "[1]");
    REQUIRE(req == "[0]");
    w.terminate();
    return "";
  });
  w.set_html(html.string_returns(
      "The string returned from a binding call must be JSON"));
  w.run();
}

TEST_CASE("The string returned of a binding call must not be JS") {
  webview::webview w(true, nullptr);
  auto html = tester_t::html;

  w.bind("loadData", [](str_arg_t /*req*/) -> std::string {
    // Try to load malicious JS code
    return "(()=>{document.body.innerHTML='gotcha';return 'hello';})()";
  });
  w.bind("endTest", [&](str_arg_t req) -> std::string {
    REQUIRE(req != "[0]");
    REQUIRE(req != "[2]");
    REQUIRE(req == "[1]");
    w.terminate();
    return "";
  });
  w.set_html(html.string_returns(
      "The string returned of a binding call must not be JS"));
  w.run();
}

TEST_CASE("webview_version()") {
  auto vi = webview_version();
  REQUIRE(vi);
  REQUIRE(vi->version.major == 1);
  REQUIRE(vi->version.minor == 2);
  REQUIRE(vi->version.patch == 3);
  REQUIRE(std::string(vi->version_number) == "1.2.3");
  REQUIRE(std::string(vi->pre_release) == "-test");
  REQUIRE(std::string(vi->build_metadata) == "+gaabbccd");
  // The function should return the same pointer when called again.
  REQUIRE(webview_version() == vi);
}

TEST_CASE("Ensure that JS code can call native code and vice versa") {
  webview::webview wv{true, nullptr};
  tester_t test{&wv};
  auto tester = test.tester;
  auto js = tester_t::js;
  auto html = tester_t::html;

  auto async_tests = std::thread([&]() {
    std::mutex worker_mtx;
    std::unique_lock<std::mutex> lock(worker_mtx);

    tester.expect_value("loaded");
    tester_t::cv().wait_for(lock, tester.seconds(2),
                            [&tester] { return tester.values_match(); });

    REQUIRE(tester.get_value() == "loaded");

    tester.expect_value("exiting 42");
    tester.ping_value(R"("exiting " + window.x)");
    tester_t::cv().wait_for(lock, tester.seconds(2),
                            [&tester] { return tester.values_match(); });

    REQUIRE(tester.get_value() == "exiting 42");

    tester.terminate();
  });

  wv.init(js.init(R"("loaded")"));
  wv.navigate(html.navigate_encoded());
  wv.run();
  async_tests.join();
}

#define ASSERT_WEBVIEW_FAILED(expr) REQUIRE(WEBVIEW_FAILED(expr))

TEST_CASE("Bad C API usage without crash") {
  webview_t w{};
  REQUIRE(webview_get_window(w) == nullptr);
  REQUIRE(webview_get_native_handle(w, WEBVIEW_NATIVE_HANDLE_KIND_UI_WINDOW) ==
          nullptr);
  ASSERT_WEBVIEW_FAILED(webview_set_size(w, 0, 0, WEBVIEW_HINT_NONE));
  ASSERT_WEBVIEW_FAILED(webview_navigate(w, nullptr));
  ASSERT_WEBVIEW_FAILED(webview_set_title(w, nullptr));
  ASSERT_WEBVIEW_FAILED(webview_set_html(w, nullptr));
  ASSERT_WEBVIEW_FAILED(webview_init(w, nullptr));
  ASSERT_WEBVIEW_FAILED(webview_eval(w, nullptr));
  ASSERT_WEBVIEW_FAILED(webview_bind(w, nullptr, nullptr, nullptr));
  ASSERT_WEBVIEW_FAILED(webview_unbind(w, nullptr));
  ASSERT_WEBVIEW_FAILED(webview_return(w, nullptr, 0, nullptr));
  ASSERT_WEBVIEW_FAILED(webview_dispatch(w, nullptr, nullptr));
  ASSERT_WEBVIEW_FAILED(webview_terminate(w));
  ASSERT_WEBVIEW_FAILED(webview_run(w));
  ASSERT_WEBVIEW_FAILED(webview_destroy(w));
}
