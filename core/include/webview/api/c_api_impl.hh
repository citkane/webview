/*
 * MIT License
 *
 * Copyright (c) 2017 Serge Zaitsev
 * Copyright (c) 2022 Steffen André Langnes
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#ifndef WEBVIEW_C_API_IMPL_HH
#define WEBVIEW_C_API_IMPL_HH

#include "webview/strings/json.hh"
#if defined(__cplusplus) && !defined(WEBVIEW_HEADER)
#include "webview/api/c_api_imp_lib.hh"
#include "webview/detail/threading/thread_detector.hh"
#include "webview/errors/errors.h"
#include "webview/log/console_log.hh"
#include "webview/types/types.hh"

using namespace webview::api;
using namespace webview::api::_lib;
using namespace webview::detail::threading;
using namespace webview::log;

WEBVIEW_API webview_t webview_create(int debug, void *wnd) {
  if (!thread::is_main_thread()) {
    throw exception{WEBVIEW_ERROR_INVALID_STATE,
                    R"(Webview must be created from the main thread.)"};
  };

  webview_cc_t *w{};
  auto err = api_filter(
      [=]() -> result<webview_cc_t *> {
        return new webview_cc_t{static_cast<bool>(debug), wnd};
      },
      [&](webview_cc_t *w_) { w = w_; });
  if (err == WEBVIEW_ERROR_OK) {
    return w;
  }
  console.error("Failed to create Webview", WEBVIEW_ERROR_UNSPECIFIED);
  return nullptr;
}

WEBVIEW_DEPRECATED(DEPRECATE_WEBVIEW_DESTROY)
WEBVIEW_API webview_error_t webview_destroy(webview_t w) {
  if (thread::is_main_thread()) {
    delete cast_to_webview(w);
  } else {
    auto wv = static_cast<webview::detail::engine_base *>(w);
    IGNORE_DEPRECATED_DECLARATIONS
    wv->dispatch([&] { delete wv; });
    RESTORE_IGNORED_WARNINGS
  }
  return {};
}

WEBVIEW_API webview_error_t webview_run(webview_t w) {
  return api_filter([=] { return cast_to_webview(w)->run(); });
}

WEBVIEW_API webview_error_t webview_terminate(webview_t w) {
  return api_filter([=] { return cast_to_webview(w)->terminate(); });
}

IGNORE_DEPRECATED_DECLARATIONS
WEBVIEW_DEPRECATED(DEPRECATE_WEBVIEW_DISPATCH)
WEBVIEW_API webview_error_t webview_dispatch(webview_t w,
                                             void (*fn)(webview_t, void *),
                                             void *arg) {
  if (!fn) {
    console.error("No `fn` argument was passed to webview_dispatch",
                  WEBVIEW_ERROR_INVALID_ARGUMENT);
    return WEBVIEW_ERROR_INVALID_ARGUMENT;
  }
  return api_filter(
      [=] { return cast_to_webview(w)->dispatch([=]() { fn(w, arg); }); });
}
RESTORE_IGNORED_WARNINGS

WEBVIEW_API void *webview_get_window(webview_t w) {
  void *window = nullptr;
  auto err = api_filter([=] { return cast_to_webview(w)->window(); },
                        [&](void *value) { window = value; });
  if (err == WEBVIEW_ERROR_OK) {
    return window;
  }
  console.error("Failed to get a valid `window` handle", err);
  return nullptr;
}

WEBVIEW_API void *webview_get_native_handle(webview_t w,
                                            webview_native_handle_kind_t kind) {
  void *handle{};
  auto err = api_filter(
      [=]() -> result<void *> {
        auto *w_ = cast_to_webview(w);
        switch (kind) {
        case WEBVIEW_NATIVE_HANDLE_KIND_UI_WINDOW:
          return w_->window();
        case WEBVIEW_NATIVE_HANDLE_KIND_UI_WIDGET:
          return w_->widget();
        case WEBVIEW_NATIVE_HANDLE_KIND_BROWSER_CONTROLLER:
          return w_->browser_controller();
        default:
          return error_info{WEBVIEW_ERROR_INVALID_ARGUMENT};
        }
      },
      [&](void *handle_) { handle = handle_; });
  if (err == WEBVIEW_ERROR_OK) {
    return handle;
  }
  console.error("Failed to get a valid `" + console.util.get_handle_kind(kind) +
                    "` handle",
                err);
  return nullptr;
}

WEBVIEW_API webview_error_t webview_set_title(webview_t w, const char *title) {
  if (!title) {
    console.warn("No `title` argument was passed to webview_set_title: Command "
                 "ignored!");
    return WEBVIEW_ERROR_INVALID_ARGUMENT;
  }
  return api_filter([=] { return cast_to_webview(w)->set_title(title); });
}

WEBVIEW_API webview_error_t webview_set_size(webview_t w, int width, int height,
                                             webview_hint_t hints) {
  return api_filter(
      [=] { return cast_to_webview(w)->set_size(width, height, hints); });
}

WEBVIEW_API webview_error_t webview_navigate(webview_t w, const char *url) {
  if (!url) {
    console.warn(
        "No `url` argument was passed to webview_navigate: Command ignored!");
    return WEBVIEW_ERROR_INVALID_ARGUMENT;
  }
  return api_filter([=] { return cast_to_webview(w)->navigate(url); });
}

WEBVIEW_API webview_error_t webview_set_html(webview_t w, const char *html) {
  if (!html) {
    console.warn(
        "No `html` argument was passed to webview_set_html: Command ignored!");
    return WEBVIEW_ERROR_INVALID_ARGUMENT;
  }
  return api_filter([=] { return cast_to_webview(w)->set_html(html); });
}

WEBVIEW_API webview_error_t webview_init(webview_t w, const char *js) {
  if (!js) {
    console.warn(
        "No `js` argument was passed to webview_init: Command ignored!");
    return WEBVIEW_ERROR_INVALID_ARGUMENT;
  }
  return api_filter([=] { return cast_to_webview(w)->init(js); });
}

WEBVIEW_API webview_error_t webview_eval(webview_t w, const char *js) {
  if (!js) {
    console.warn(
        "No `js` argument was passed to webview_eval: Command ignored!");
    return WEBVIEW_ERROR_INVALID_ARGUMENT;
  }
  return api_filter([=] { return cast_to_webview(w)->eval(js); });
}

WEBVIEW_API webview_error_t webview_bind(webview_t w, const char *name,
                                         void (*fn)(const char *id,
                                                    const char *req, void *arg),
                                         void *arg) {
  if (!name || !fn) {
    if (!name) {
      console.warn(
          "No `name` argument was passed  to webview_bind: Command ignored!");
    }
    if (!fn) {
      console.warn(
          "No `fn` argument was passed to webview_bind: Command ignored!");
    }
    return WEBVIEW_ERROR_INVALID_ARGUMENT;
  }
  return api_filter([=] {
    return cast_to_webview(w)->bind(
        name,
        [=](cnst_str_r seq, cnst_str_r req, void *arg_) {
          fn(seq.c_str(), req.c_str(), arg_);
        },
        arg);
  });
}

WEBVIEW_API webview_error_t webview_unbind(webview_t w, const char *name) {
  if (!name) {
    console.warn(
        "No `name` argument was passed to webview_unbind: Command ignored!");
    return WEBVIEW_ERROR_INVALID_ARGUMENT;
  }
  return api_filter([=] { return cast_to_webview(w)->unbind(name); });
}

WEBVIEW_API webview_error_t webview_return(webview_t w, const char *id,
                                           int status, const char *result) {
  if (!id || !result) {
    if (!id) {
      console.warn(
          "No `id` argument was passed to webview_return: Command ignored!");
    }
    if (!result) {
      console.warn("No `result` argument was passed to webview_return: Command "
                   "ignored!");
    }
    return WEBVIEW_ERROR_INVALID_ARGUMENT;
  }
  return api_filter(
      [=] { return cast_to_webview(w)->resolve(id, status, result); });
}

WEBVIEW_API const webview_version_info_t *webview_version(void) {
  return &library_version_info;
}

WEBVIEW_API const char *json_parse(const char *s, const char *key,
                                   const int index) {
  auto result = webview::strings::json.parse(s, key, index);
  auto result_c = result.c_str();
  return result_c;
}

#endif // defined(__cplusplus) && !defined(WEBVIEW_HEADER)
#endif // WEBVIEW_C_API_IMPL_HH
