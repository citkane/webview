#include "webview/api.h"
#include "webview/test_driver.hh"

#define WEBVIEW_VERSION_MAJOR 1
#define WEBVIEW_VERSION_MINOR 2
#define WEBVIEW_VERSION_PATCH 3
#define WEBVIEW_VERSION_PRE_RELEASE "-test"
#define WEBVIEW_VERSION_BUILD_METADATA "+gaabbccd"

#include "webview/utility/frontend_strings.hh"
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

TEST_CASE("Use C API to test binding and unbinding") {
  using namespace webview::utility;
  struct context_t {
    webview_t w;
    unsigned int number;
  } context{};
  auto test = +[](const char *seq, const char *req, void *arg) {
    auto increment = +[](const char *seq, const char * /*req*/, void *arg) {
      auto *context = static_cast<context_t *>(arg);
      ++context->number;
      webview_return(context->w, seq, 0, "");
    };
    auto context = static_cast<context_t *>(arg);
    std::string req_(req);
    // Bind and increment number.
    if (req_ == "[0]") {
      REQUIRE(context->number == 0);
      webview_bind(context->w, "increment", increment, context);
      webview_eval(context->w,
                   tokeniser(TEST_B_UB_CALL, TOKEN_VALUE, "1").c_str());
      webview_return(context->w, seq, 0, "");
      return;
    }
    // Unbind and make sure that we cannot increment even if we try.
    if (req_ == "[1]") {
      REQUIRE(context->number == 1);
      webview_unbind(context->w, "increment");
      webview_eval(context->w,
                   tokeniser(TEST_B_UB_CALL, TOKEN_VALUE, "2").c_str());
      webview_return(context->w, seq, 0, "");
      return;
    }
    // Number should not have changed but we can bind again and change the number.
    if (req_ == "[2,1]") {
      REQUIRE(context->number == 1);
      webview_bind(context->w, "increment", increment, context);
      webview_eval(context->w,
                   tokeniser(TEST_B_UB_CALL, TOKEN_VALUE, "3").c_str());
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
  webview_bind(w, "test", test, &context);
  // Attempting to bind multiple times only binds once
  webview_bind(w, "test", test, &context);
  webview_eval(w, "window.test(0);");
  webview_run(w);
  webview_destroy(w);
}

TEST_CASE("Test synchronous binding and unbinding") {
  using namespace webview::utility;

  unsigned int number = 0;
  webview::webview w(true, nullptr);
  auto test = [&](const std::string &req) -> std::string {
    auto increment = [&](const std::string & /*req*/) -> std::string {
      ++number;
      return "";
    };
    printf("%s\n", req.c_str());
    // Bind and increment number.
    if (req == "[0]") {
      REQUIRE(number == 0);
      w.bind("increment", increment);
      w.eval(tokeniser(TEST_B_UB_CALL, TOKEN_VALUE, "1"));
      return "";
    }
    // Unbind and make sure that we cannot increment even if we try.
    if (req == "[1]") {
      REQUIRE(number == 1);
      w.unbind("increment");
      w.eval(tokeniser(TEST_B_UB_CALL, TOKEN_VALUE, "2"));
      return "";
    }
    // We should have gotten an error on the JS side.
    // Number should not have changed but we can bind again and change the number.
    if (req == "[2,1]") {
      REQUIRE(number == 1);
      w.bind("increment", increment);
      w.eval(tokeniser(TEST_B_UB_CALL, TOKEN_VALUE, "3"));
      return "";
    }
    // Finish test.
    if (req == "[3]") {
      REQUIRE(number == 2);
      //w.dispatch([&] { w.terminate(); });
      w.terminate();
      return "";
    }
    REQUIRE(!"Should not reach here");
    return "";
  };
  w.set_html("Test synchronous binding and unbinding");
  // Attempting to remove non-existing binding is OK
  w.unbind("test");
  w.bind("test", test);
  // Attempting to bind multiple times only binds once
  w.bind("test", test);
  w.eval("window.test(0);");
  w.run();
}

TEST_CASE("The string returned from a binding call must be JSON") {
  constexpr auto html =
      R"html(<script>
  try {
    window.loadData()
      .then(() => window.endTest(0))
      .catch(() => window.endTest(1));
  } catch {
    window.endTest(2);
  }
</script>)html";

  webview::webview w(true, nullptr);

  w.bind("loadData", [](const std::string & /*req*/) -> std::string {
    return "\"hello\"";
  });

  w.bind("endTest", [&](const std::string &req) -> std::string {
    REQUIRE(req != "[2]");
    REQUIRE(req != "[1]");
    REQUIRE(req == "[0]");
    w.terminate();
    return "";
  });

  w.set_html(html);
  w.run();
}

TEST_CASE("The string returned of a binding call must not be JS") {
  constexpr const auto html =
      R"html(<script>
  try {
    window.loadData()
      .then(() => window.endTest(0))
      .catch(() => window.endTest(1));
  } catch {
    window.endTest(2);
  }
</script>)html";

  webview::webview w(true, nullptr);

  w.bind("loadData", [](const std::string & /*req*/) -> std::string {
    // Try to load malicious JS code
    return "(()=>{document.body.innerHTML='gotcha';return 'hello';})()";
  });

  w.bind("endTest", [&](const std::string &req) -> std::string {
    REQUIRE(req != "[0]");
    REQUIRE(req != "[2]");
    REQUIRE(req == "[1]");
    w.terminate();
    return "";
  });

  w.set_html(html);
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
#define TEST_CASE_URL "data:text/html,%3Chtml%3Ehello%3C%2Fhtml%3E"
#define TEST_CASE_INIT_JS                                                      \
  "window.x = 42;\n"                                                           \
  "window.onload = () => {\n"                                                  \
  " " TOKEN_POST_FN "\n"                                                       \
  "};"

  std::mutex mtx;
  std::unique_lock<std::mutex> lock(mtx);
  using namespace webview::detail;
  using namespace webview::utility;

  auto on_load_val = tester::post_value("\"loaded\"");
  auto init_js = tokeniser(TEST_CASE_INIT_JS, TOKEN_POST_FN, on_load_val);
  webview::webview wv{true, nullptr};
  webview::webview *wv_pointer = &wv;
  auto async_tests = std::thread(
      [](webview::webview *wv) {
        std::mutex mtx;
        std::unique_lock<std::mutex> lock(mtx);

        tester::worker_thread_ready(true);
        tester::expect_value("loaded");
        tester::worker_cv().wait_for(lock, tester::timeout_sec(2),
                                     []() { return tester::values_match(); });
        REQUIRE(tester::values_match());

        tester::expect_value("exiting 42");
        tester::post_value(wv, "\"exiting \" + window.x");
        tester::worker_cv().wait_for(lock, tester::timeout_sec(),
                                     [] { return tester::values_match(); });
        REQUIRE(tester::values_match());

        tester::terminate(wv);
      },
      wv_pointer);

  wv.navigate(TEST_CASE_URL);
  wv.init(init_js);
  tester::main_cv().wait(lock, [] { return tester::worker_thread_ready(); });

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
