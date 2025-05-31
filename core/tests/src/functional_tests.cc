#include "webview/test_driver.hh"
#include <string>

#define WEBVIEW_VERSION_MAJOR 1
#define WEBVIEW_VERSION_MINOR 2
#define WEBVIEW_VERSION_PATCH 3
#define WEBVIEW_VERSION_PRE_RELEASE "-test"
#define WEBVIEW_VERSION_BUILD_METADATA "+gaabbccd"

#include "webview/tests/test_helper.hh"
#include "webview/types/types.hh"
#include "webview/webview.h"
#include <cassert>
#include <cstdint>

static const auto &tracer = trace::tests;

// This test should only run on Windows to enable us to perform a controlled
// "warm-up" of MS WebView2 in order to avoid the initial test from
// occationally timing out in CI.
#ifdef WEBVIEW_PLATFORM_WINDOWS
#include <iostream>

TEST_CASE("# Warm-up") {
  // Signal to the test runner that this may be a slow test.
  std::cerr << "[[slow]]" << std::endl; // NOLINT(performance-avoid-endl)
  webview_cc_t w(false, nullptr);
  w.dispatch([&]() { w.terminate(); });
  w.run();
}
#endif

TEST_CASE("Start app loop and terminate it") {
  webview_cc_t w(false, nullptr);
  w.dispatch([&]() { w.terminate(); });
  w.run();
}

TEST_CASE("Detect main or child thread") {
  using namespace webview::detail::threading;
  REQUIRE(thread::is_main_thread() == true);

  auto child_thread =
      std::thread([] { REQUIRE(thread::is_main_thread() == false); });

  child_thread.join();
}

TEST_CASE("Use C API to create a window, run app and terminate it") {
  static auto cb_assert_arg = +[](webview_t w, void *arg) {
    REQUIRE(w != nullptr);
    REQUIRE(memcmp(arg, "arg", 3) == 0);
  };
  static auto cb_terminate = +[](webview_t w, void *arg) {
    REQUIRE(arg == nullptr);
    webview_terminate(w);
  };

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

TEST_CASE("Use C API to test binding and unbinding") {
  struct c_context_t {
    webview_t w;
    unsigned int number;
    bool res1;
    bool res2;
    bool res3;
    bool res4;
  } ctx{};

  auto static increment =
      +[](const char *seq, const char * /*req*/, void *arg) {
        auto *ctx = static_cast<c_context_t *>(arg);
        ++ctx->number;
        std::string message =
            "\"Incremented: " + std::to_string(ctx->number) + "\"";
        webview_return(ctx->w, seq, 0, message.c_str());
      };

  auto static tests = +[](const char *seq, const char *req, void *arg) {
    auto ctx = static_cast<c_context_t *>(arg);
    std::string req_(req);
    // Bind and increment number.
    if (req_ == "[0]") {
      ctx->res1 = (ctx->number == 0);
      webview_bind(ctx->w, "increment", increment, ctx);
      webview_eval(ctx->w, string::tests::js.make_call_js(1).c_str());
      webview_return(ctx->w, seq, 0, R"("Returned: [0]")");
      return;
    }
    // Unbind and make sure that we cannot increment even if we try.
    if (req_ == "[1]") {
      ctx->res2 = (ctx->number == 1);
      webview_unbind(ctx->w, "increment");
      webview_eval(ctx->w, string::tests::js.make_call_js(2).c_str());
      webview_return(ctx->w, seq, 0, R"("Returned: [1]")");
      return;
    }
    // Number should not have changed but we can bind again and change the number.
    if (req_ == "[2,1]") {
      ctx->res3 = (ctx->number == 1);
      webview_bind(ctx->w, "increment", increment, ctx);
      webview_eval(ctx->w, string::tests::js.make_call_js(3).c_str());
      webview_return(ctx->w, seq, 0, R"("Returned: [2,1]")");
      return;
    }
    // Finish test.
    if (req_ == "[3]") {
      ctx->res4 = (ctx->number == 2);
      webview_return(ctx->w, seq, 0, R"("Returned: [3]")");
      webview_terminate(ctx->w);
      return;
    }
    REQUIRE(!"Should not reach here");
  };

  auto w = webview_create(1, nullptr);
  ctx.w = w;
  webview_set_html(w, "Use C API to test binding and unbinding");
  // Attempting to remove non-existing binding is OK
  webview_unbind(w, "test");
  webview_bind(w, "test", tests, &ctx);
  // Attempting to bind multiple times only binds once
  webview_bind(w, "test", tests, &ctx);
  webview_eval(w, R"(window.test(0).then(m => console.log(m));)");
  webview_run(w);
  auto passed = ctx.res1 && ctx.res2 && ctx.res3 && ctx.res4;

  if (!passed) {
    tracer.print_here(tester::res_string("res1", ctx.res1));
    tracer.print_here(tester::res_string("res2", ctx.res2));
    tracer.print_here(tester::res_string("res3", ctx.res3));
    tracer.print_here(tester::res_string("res4", ctx.res4));
  }

  REQUIRE(passed);
}

TEST_CASE("Test synchronous binding and unbinding") {
  webview_cc_t w(true, nullptr);
  int number = 0;
  int res1;
  int res2;
  int res3;
  int res4;

  auto increment = [&](cnst_str_r /*req*/) -> std::string {
    ++number;
    return "";
  };

  auto tests = [&](cnst_str_r req) -> std::string {
    // Bind and increment number.
    if (req == "[0]") {
      res1 = number;
      w.bind("increment", increment);
      w.eval(string::tests::js.make_call_js(1));
      return "";
    }

    // Unbind and make sure that we cannot increment even if we try.
    if (req == "[1]") {
      res2 = number;
      w.unbind("increment", true);
      w.eval(string::tests::js.make_call_js(2), true);
      return "";
    }
    // We should have gotten an error on the JS side.
    // Number should not have changed but we can bind again and change the number.
    if (req == "[2,1]") {
      res3 = number;
      w.bind("increment", increment);
      w.eval(string::tests::js.make_call_js(3), true);
      return "";
    }
    // Finish test.
    if (req == "[3]") {
      res4 = number;
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
  w.set_html(string::tests::html.bind_unbind());
  w.run();
  auto passed = res1 == 0 && res2 == 1 && res3 == 1 && res4 == 2;
  if (!passed) {
    tracer.print_here(tester::res_string("res1", res1));
    tracer.print_here(tester::res_string("res2", res2));
    tracer.print_here(tester::res_string("res3", res3));
    tracer.print_here(tester::res_string("res4", res4));
  }
  REQUIRE(passed);
}

TEST_CASE("The string returned from a binding call must be JSON") {
  webview_cc_t w(true, nullptr);

  w.bind("loadData",
         [](cnst_str_r /*req*/) -> std::string { return "\"hello\""; });
  w.bind("endTest", [&](cnst_str_r req) -> std::string {
    REQUIRE(req != "[2]");
    REQUIRE(req != "[1]");
    REQUIRE(req == "[0]");
    w.terminate();
    return "";
  });
  w.set_html(string::tests::html.string_returns(
      "The string returned from a binding call must be JSON"));
  w.run();
}

TEST_CASE("The string returned of a binding call must not be JS") {
  webview_cc_t w(true, nullptr);

  w.bind("loadData", [](cnst_str_r /*req*/) -> std::string {
    // Try to load malicious JS code
    return "(()=>{document.body.innerHTML='gotcha';return 'hello';})()";
  });
  w.bind("endTest", [&](cnst_str_r req) -> std::string {
    REQUIRE(req != "[0]");
    REQUIRE(req != "[2]");
    REQUIRE(req == "[1]");
    w.terminate();
    return "";
  });
  w.set_html(string::tests::html.string_returns(
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
  webview_cc_t wv{true, nullptr};

  auto async_tests = std::thread([&]() {
    std::mutex worker_mtx;
    std::unique_lock<std::mutex> lock(worker_mtx);

    tester::expect_value("loaded");
    tester::cv().wait_for(lock, tester::seconds(2),
                          [&] { return tester::values_match(); });

    REQUIRE(tester::get_value() == "loaded");

    tester::expect_value("exiting 42");
    tester::ping_value(R"("exiting " + window.x)", &wv, true);
    tester::cv().wait_for(lock, tester::seconds(2),
                          [&] { return tester::values_match(); });

    REQUIRE(tester::get_value() == "exiting 42");

    wv.terminate();
  });

  wv.init(string::tests::js.init("loaded"));
  wv.navigate(string::tests::html.navigate_encoded());
  wv.run();
  async_tests.join();
}

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
