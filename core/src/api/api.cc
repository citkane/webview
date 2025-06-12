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

#if defined(__cplusplus) && !defined(WEBVIEW_HEADER)
#include "api/api.h"
#include "api/api_lib.hh"
#include "detail/threading/thread_detector.hh"
#include "errors/errors.h"
#include "log/console_log.hh"
#include "strings/json.hh"
#include "types/types.hh"

using namespace webview::api;
using namespace webview::log;
using namespace webview::_lib::_api;
using namespace webview::detail::threading;

/* PUBLIC C API implementations
 * ∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇ */

WEBVIEW_API webview_t webview_create(int debug, void *wnd) {
  webview_cc_t *w{};
  auto err_ = api_filter(
      [=]() -> result<webview_cc_t *> {
        return new webview_cc_t{static_cast<bool>(debug), wnd};
      },
      [&](webview_cc_t *w_) { w = w_; });
  if (err_ == WEBVIEW_ERROR_OK) {
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
    auto wv = static_cast<engine_base *>(w);
    IGNORE_DEPRECATED_DECLARATIONS
    wv->dispatch([&] { delete wv; });
    RESTORE_IGNORED_WARNINGS
  }
  return {};
}

WEBVIEW_API int webview_run(webview_t w) {
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
  auto err_ = api_filter([=] { return cast_to_webview(w)->window(); },
                         [&](void *value) { window = value; });
  if (err_ == WEBVIEW_ERROR_OK) {
    return window;
  }
  console.error("Failed to get a valid `window` handle", err_);
  return nullptr;
}

WEBVIEW_API void *webview_get_native_handle(webview_t w,
                                            webview_native_handle_kind_t kind) {
  void *handle{};
  auto err_ = api_filter(
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
  if (err_ == WEBVIEW_ERROR_OK) {
    return handle;
  }
  console.error("Failed to get a valid `" + console.util.get_handle_kind(kind) +
                    "` handle",
                err_);
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

WEBVIEW_API int webview_init(webview_t w, const char *js) {
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

WEBVIEW_API webview_error_t json_parse(char **buffer, const char *json_str,
                                       const char *key, int index) {
  std::string parsed_res;
  auto err_mess = "Failed to JSON parse the string";
  try {
    parsed_res = webview::strings::json.parse(json_str, key, index);
  } catch (...) {
    console.error(err_mess, WEBVIEW_ERROR_UNSPECIFIED);
    return WEBVIEW_ERROR_UNSPECIFIED;
  }
  return alloc_string_buffer(buffer, parsed_res, err_mess);
}

WEBVIEW_API webview_error_t json_escape(char **buffer, const char *str,
                                        bool add_quotes) {
  std::string escaped_res;
  auto err_mess = "Failed to JSON parse the string";
  try {
    escaped_res = webview::strings::json.escape(str, add_quotes);
  } catch (...) {
    console.error(err_mess, WEBVIEW_ERROR_UNSPECIFIED);
    return WEBVIEW_ERROR_UNSPECIFIED;
  }
  return alloc_string_buffer(buffer, escaped_res, err_mess);
}

/* ∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆
 * PUBLIC C API implementations
 * ----------------------------------------------------------------------------------------------------------- 
 * API lib implementations
 * ∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇∇ */

template <typename WorkFn, typename ResultFn>
webview_error_t
webview::_lib::_api::api_filter(WorkFn &&do_work,
                                ResultFn &&put_result) noexcept {
  try {
    auto result = do_work();
    if (result.ok()) {
      put_result(result.value());
      return WEBVIEW_ERROR_OK;
    }
    return result.error().code();
  } catch (const exception &e) {
    return e.error().code();
  } catch (...) {
    return WEBVIEW_ERROR_UNSPECIFIED;
  }
}

template <typename WorkFn>
webview_error_t webview::_lib::_api::api_filter(WorkFn &&do_work) noexcept {
  try {
    auto result = do_work();
    if (result.ok()) {
      return WEBVIEW_ERROR_OK;
    }
    return result.error().code();
  } catch (const exception &e) {
    return e.error().code();
  } catch (...) {
    return WEBVIEW_ERROR_UNSPECIFIED;
  }
}

inline webview_cc_t *webview::_lib::_api::cast_to_webview(void *w) {
  if (!w) {
    throw exception{WEBVIEW_ERROR_INVALID_ARGUMENT,
                    "Cannot cast null pointer to webview instance"};
  }
  return static_cast<webview_cc_t *>(w);
}

IGNORE_UNUSED_PARAMETERS
// NOLINTBEGIN(misc-unused-parameters)
webview_error_t webview::_lib::_api::alloc_string_buffer(char **buffer,
                                                         cnst_str_r str,
                                                         cnst_str_r err_mess) {
  auto size = str.size() + 1;
  // NOLINTNEXTLINE(hicpp-no-malloc, cppcoreguidelines-no-malloc)
  *buffer = static_cast<char *>(malloc(size));
#ifdef _MSC_VER
  auto res = strcpy_s(*buffer, size, str.c_str());
  if (res != 0) {
    console.error(std::string("MSVC: ") + err_mess, WEBVIEW_ERROR_UNSPECIFIED);
    return WEBVIEW_ERROR_UNSPECIFIED;
  }
#else
  strcpy(*buffer, str.c_str());
#endif
  return WEBVIEW_ERROR_OK;
}
// NOLINTEND(misc-unused-parameters)
RESTORE_IGNORED_WARNINGS

/* ∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆∆
 * API lib implementations */

#endif // defined(__cplusplus) && !defined(WEBVIEW_HEADER)
#endif // WEBVIEW_C_API_IMPL_HH
