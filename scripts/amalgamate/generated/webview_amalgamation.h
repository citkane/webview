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
/*
 * MIT License
 *
 * Copyright (c) 2025 Michael Jonker
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
/*
 * MIT License
 *
 * Copyright (c) 2022 Steffen André Langnes
 * Copyright (c) 2025 Michael Jonker
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
/*
 * MIT License
 *
 * Copyright (c) 2022 Steffen André Langnes
 * Copyright (c) 2025 Michael Jonker
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
/*
 * MIT License
 *
 * Copyright (c) 2017 Serge Zaitsev
 * Copyright (c) 2022 Steffen André Langnes
 * Copyright (c) 2025 Michael Jonker
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
/*
 * MIT License
 *
 * Copyright (c) 2025 Michael Jonker
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
// file begin: include/webview/errors/errors.h
#ifndef WEBVIEW_ERRORS_H
#define WEBVIEW_ERRORS_H

/// @name Errors
/// @{

/**
 * @brief Error codes returned to callers of the API.
 *
 * The following codes are commonly used in the library:
 * - @c WEBVIEW_ERROR_OK
 * - @c WEBVIEW_ERROR_UNSPECIFIED
 * - @c WEBVIEW_ERROR_INVALID_ARGUMENT
 * - @c WEBVIEW_ERROR_INVALID_STATE
 *
 * With the exception of @c WEBVIEW_ERROR_OK which is normally expected,
 * the other common codes do not normally need to be handled specifically.
 * Refer to specific functions regarding handling of other codes.
 */
typedef enum {
  /// Missing dependency.
  WEBVIEW_ERROR_MISSING_DEPENDENCY = -5,
  /// Operation canceled.
  WEBVIEW_ERROR_CANCELED = -4,
  /// Invalid state detected.
  WEBVIEW_ERROR_INVALID_STATE = -3,
  /// One or more invalid arguments have been specified e.g. in a function call.
  WEBVIEW_ERROR_INVALID_ARGUMENT = -2,
  /// An unspecified error occurred. A more specific error code may be needed.
  WEBVIEW_ERROR_UNSPECIFIED = -1,
  /// OK/Success. Functions that return error codes will typically return this
  /// to signify successful operations.
  WEBVIEW_ERROR_OK = 0,
  /// Signifies that something already exists.
  WEBVIEW_ERROR_DUPLICATE = 1,
  /// Signifies that something does not exist.
  WEBVIEW_ERROR_NOT_FOUND = 2
} webview_error_t;

/// @}

#endif // WEBVIEW_ERRORS_H
// file end: include/webview/errors/errors.h
// file begin: include/webview/lib/macros.h
#ifndef WEBVIEW_MACROS_H
#define WEBVIEW_MACROS_H

/**
 * Used to specify function linkage such as extern, inline, etc.
 *
 * When @c WEBVIEW_API is not already defined, the defaults are as follows:
 *
 * - @c inline when compiling C++ code.
 * - @c extern when compiling C code.
 *
 * The following macros can be used to automatically set an appropriate
 * value for @c WEBVIEW_API:
 *
 * - Define @c WEBVIEW_BUILD_SHARED when building a shared library.
 * - Define @c WEBVIEW_SHARED when using a shared library.
 * - Define @c WEBVIEW_STATIC when building or using a static library.
 */
#ifndef WEBVIEW_API
#if defined(WEBVIEW_SHARED) || defined(WEBVIEW_BUILD_SHARED)
#if defined(_WIN32) || defined(__CYGWIN__)
#if defined(WEBVIEW_BUILD_SHARED)
#define WEBVIEW_API __declspec(dllexport)
#else
#define WEBVIEW_API __declspec(dllimport)
#endif
#else
#define WEBVIEW_API __attribute__((visibility("default")))
#endif
#elif !defined(WEBVIEW_STATIC) && defined(__cplusplus)
#define WEBVIEW_API inline
#else
#define WEBVIEW_API extern
#endif
#endif

/// @name Used internally
/// @{

/// Utility macro for stringifying a macro argument.
#define WEBVIEW_STRINGIFY(x) #x

/// Utility macro for stringifying the result of a macro argument expansion.
#define WEBVIEW_EXPAND_AND_STRINGIFY(x) WEBVIEW_STRINGIFY(x)

/// @}

/// @brief Evaluates to @c TRUE for error codes indicating success or
///        additional information.
#define WEBVIEW_SUCCEEDED(error) ((int)(error) >= 0)

/// Evaluates to @c TRUE if the given error code indicates failure.
#define WEBVIEW_FAILED(error) ((int)(error) < 0)

#ifdef __cplusplus
#ifndef WEBVIEW_HEADER

#if defined(__APPLE__)
#define WEBVIEW_PLATFORM_DARWIN
#elif defined(__unix__)
#define WEBVIEW_PLATFORM_LINUX
#elif defined(_WIN32)
#define WEBVIEW_PLATFORM_WINDOWS
#else
#error "Unable to detect current platform"
#endif

#if !defined(WEBVIEW_GTK) && !defined(WEBVIEW_COCOA) && !defined(WEBVIEW_EDGE)
#if defined(WEBVIEW_PLATFORM_DARWIN)
#define WEBVIEW_COCOA
#elif defined(WEBVIEW_PLATFORM_LINUX)
#define WEBVIEW_GTK
#elif defined(WEBVIEW_PLATFORM_WINDOWS)
#define WEBVIEW_EDGE
#else
#error "please, specify webview backend"
#endif
#endif

#ifndef WEBVIEW_DEPRECATED
#if __cplusplus >= 201402L
#define WEBVIEW_DEPRECATED(reason) [[deprecated(reason)]]
#elif defined(_MSC_VER)
#define WEBVIEW_DEPRECATED(reason) __declspec(deprecated(reason))
#else
#define WEBVIEW_DEPRECATED(reason) __attribute__((deprecated(reason)))
#endif
#endif

#ifndef WEBVIEW_DEPRECATED_PRIVATE
#define WEBVIEW_DEPRECATED_PRIVATE                                             \
  WEBVIEW_DEPRECATED("Private API should not be used")
#endif

#endif // WEBVIEW_HEADER
#endif // __cplusplus

#endif // WEBVIEW_MACROS_H
// file end: include/webview/lib/macros.h
// file begin: include/webview/types/types.h
#ifndef WEBVIEW_TYPES_H
#define WEBVIEW_TYPES_H

/// Holds the elements of a MAJOR.MINOR.PATCH version number.
typedef struct {
  /// Major version.
  unsigned int major;
  /// Minor version.
  unsigned int minor;
  /// Patch version.
  unsigned int patch;
} webview_version_t;

/// Holds the library's version information.
typedef struct {
  /// The elements of the version number.
  webview_version_t version;
  /// SemVer 2.0.0 version number in MAJOR.MINOR.PATCH format.
  char version_number[32];
  /// SemVer 2.0.0 pre-release labels prefixed with "-" if specified, otherwise
  /// an empty string.
  char pre_release[48];
  /// SemVer 2.0.0 build metadata prefixed with "+", otherwise an empty string.
  char build_metadata[48];
} webview_version_info_t;

/// Pointer to a webview instance.
typedef void *webview_t;

/// Native handle kind. The actual type depends on the backend.
typedef enum {
  /// Top-level window. @c GtkWindow pointer (GTK), @c NSWindow pointer (Cocoa)
  /// or @c HWND (Win32).
  WEBVIEW_NATIVE_HANDLE_KIND_UI_WINDOW,
  /// Browser widget. @c GtkWidget pointer (GTK), @c NSView pointer (Cocoa) or
  /// @c HWND (Win32).
  WEBVIEW_NATIVE_HANDLE_KIND_UI_WIDGET,
  /// Browser controller. @c WebKitWebView pointer (WebKitGTK), @c WKWebView
  /// pointer (Cocoa/WebKit) or @c ICoreWebView2Controller pointer
  /// (Win32/WebView2).
  WEBVIEW_NATIVE_HANDLE_KIND_BROWSER_CONTROLLER
} webview_native_handle_kind_t;

/// Window size hints
typedef enum {
  /// Width and height are default size.
  WEBVIEW_HINT_NONE,
  /// Width and height are minimum bounds.
  WEBVIEW_HINT_MIN,
  /// Width and height are maximum bounds.
  WEBVIEW_HINT_MAX,
  /// Window size can not be changed by a user.
  WEBVIEW_HINT_FIXED
} webview_hint_t;

#endif // WEBVIEW_TYPES_H
// file end: include/webview/types/types.h
// file begin: include/webview/detail/frontend/user_script.hh
#ifndef WEBVIEW_DETAIL_USER_SCRIPT_HH
#define WEBVIEW_DETAIL_USER_SCRIPT_HH

#if defined(__cplusplus) && !defined(WEBVIEW_HEADER)

#include <functional>
#include <memory>
#include <string>
#include <utility>

namespace webview {
namespace detail {

/* ************************************************************************
 * Handlers and types for managing the Webview
 * `bind` -> JS call().then() -> `resolve`
 * execution and promise resolution lifecycle.
 **************************************************************************/

/// Type definition for a native promise callback function,
/// ie. `void callback("id", "params", arg)`;
using binding_t = std::function<void(std::string, std::string, void *)>;

/// Handlers for a bound JS function / native promise resolution lifecycle.
class user_script {
public:
  class impl;
  using impl_deleter = std::function<void(impl *)>;
  using impl_ptr = std::unique_ptr<impl, impl_deleter>;

  user_script(const std::string &code, impl_ptr &&impl_)
      : m_code{code}, m_impl{std::move(impl_)} {}

  user_script(const user_script &other) = delete;
  user_script &operator=(const user_script &other) = delete;
  user_script(user_script &&other) noexcept { *this = std::move(other); }

  user_script &operator=(user_script &&other) noexcept {
    if (this == &other) {
      return *this;
    }
    m_code = std::move(other.m_code);
    m_impl = std::move(other.m_impl);
    return *this;
  }

  const std::string &get_code() const { return m_code; }

  impl &get_impl() { return *m_impl; }

  const impl &get_impl() const { return *m_impl; }

private:
  std::string m_code;
  impl_ptr m_impl;
};

/// A type for handling a bound JS promise resolution on the native side.
class binding_ctx_t {
public:
  binding_ctx_t(binding_t callback, void *arg)
      : m_callback(callback), m_arg(arg) {}
  /// Executes the bound JS function native promise resolution callback.
  void call(std::string id, std::string args) const {
    if (m_callback) {
      m_callback(id, args, m_arg);
    }
  }

private:
  /// This native function is called upon execution of the bound JS function.
  binding_t m_callback;
  /// This user-supplied argument is passed to the native callback function,
  /// ie. `webview_bind(w, "name", fn, **arg**);`
  void *m_arg;
};

} // namespace detail
} // namespace webview

#endif // defined(__cplusplus) && !defined(WEBVIEW_HEADER)
#endif // WEBVIEW_DETAIL_USER_SCRIPT_HH
// file end: include/webview/detail/frontend/user_script.hh
// file begin: include/webview/lib/json.hh
#ifndef WEBVIEW_DETAIL_JSON_HH
#define WEBVIEW_DETAIL_JSON_HH

#if defined(__cplusplus) && !defined(WEBVIEW_HEADER)

#include <cassert>
#include <cstring>
#include <string>

namespace webview {
namespace detail {

inline int json_parse_c(const char *s, size_t sz, const char *key, size_t keysz,
                        const char **value, size_t *valuesz) {
  enum {
    JSON_STATE_VALUE,
    JSON_STATE_LITERAL,
    JSON_STATE_STRING,
    JSON_STATE_ESCAPE,
    JSON_STATE_UTF8
  } state = JSON_STATE_VALUE;
  const char *k = nullptr;
  int index = 1;
  int depth = 0;
  int utf8_bytes = 0;

  *value = nullptr;
  *valuesz = 0;

  if (key == nullptr) {
    index = static_cast<decltype(index)>(keysz);
    if (index < 0) {
      return -1;
    }
    keysz = 0;
  }

  for (; sz > 0; s++, sz--) {
    enum {
      JSON_ACTION_NONE,
      JSON_ACTION_START,
      JSON_ACTION_END,
      JSON_ACTION_START_STRUCT,
      JSON_ACTION_END_STRUCT
    } action = JSON_ACTION_NONE;
    auto c = static_cast<unsigned char>(*s);
    switch (state) {
    case JSON_STATE_VALUE:
      if (c == ' ' || c == '\t' || c == '\n' || c == '\r' || c == ',' ||
          c == ':') {
        continue;
      } else if (c == '"') {
        action = JSON_ACTION_START;
        state = JSON_STATE_STRING;
      } else if (c == '{' || c == '[') {
        action = JSON_ACTION_START_STRUCT;
      } else if (c == '}' || c == ']') {
        action = JSON_ACTION_END_STRUCT;
      } else if (c == 't' || c == 'f' || c == 'n' || c == '-' ||
                 (c >= '0' && c <= '9')) {
        action = JSON_ACTION_START;
        state = JSON_STATE_LITERAL;
      } else {
        return -1;
      }
      break;
    case JSON_STATE_LITERAL:
      if (c == ' ' || c == '\t' || c == '\n' || c == '\r' || c == ',' ||
          c == ']' || c == '}' || c == ':') {
        state = JSON_STATE_VALUE;
        s--;
        sz++;
        action = JSON_ACTION_END;
      } else if (c < 32 || c > 126) {
        return -1;
      } // fallthrough
    case JSON_STATE_STRING:
      if (c < 32 || (c > 126 && c < 192)) {
        return -1;
      } else if (c == '"') {
        action = JSON_ACTION_END;
        state = JSON_STATE_VALUE;
      } else if (c == '\\') {
        state = JSON_STATE_ESCAPE;
      } else if (c >= 192 && c < 224) {
        utf8_bytes = 1;
        state = JSON_STATE_UTF8;
      } else if (c >= 224 && c < 240) {
        utf8_bytes = 2;
        state = JSON_STATE_UTF8;
      } else if (c >= 240 && c < 247) {
        utf8_bytes = 3;
        state = JSON_STATE_UTF8;
      } else if (c >= 128 && c < 192) {
        return -1;
      }
      break;
    case JSON_STATE_ESCAPE:
      if (c == '"' || c == '\\' || c == '/' || c == 'b' || c == 'f' ||
          c == 'n' || c == 'r' || c == 't' || c == 'u') {
        state = JSON_STATE_STRING;
      } else {
        return -1;
      }
      break;
    case JSON_STATE_UTF8:
      if (c < 128 || c > 191) {
        return -1;
      }
      utf8_bytes--;
      if (utf8_bytes == 0) {
        state = JSON_STATE_STRING;
      }
      break;
    default:
      return -1;
    }

    if (action == JSON_ACTION_END_STRUCT) {
      depth--;
    }

    if (depth == 1) {
      if (action == JSON_ACTION_START || action == JSON_ACTION_START_STRUCT) {
        if (index == 0) {
          *value = s;
        } else if (keysz > 0 && index == 1) {
          k = s;
        } else {
          index--;
        }
      } else if (action == JSON_ACTION_END ||
                 action == JSON_ACTION_END_STRUCT) {
        if (*value != nullptr && index == 0) {
          *valuesz = static_cast<size_t>(s + 1 - *value);
          return 0;
        } else if (keysz > 0 && k != nullptr) {
          if (keysz == static_cast<size_t>(s - k - 1) &&
              memcmp(key, k + 1, keysz) == 0) {
            index = 0;
          } else {
            index = 2;
          }
          k = nullptr;
        }
      }
    }

    if (action == JSON_ACTION_START_STRUCT) {
      depth++;
    }
  }
  return -1;
}

constexpr bool is_json_special_char(char c) {
  return c == '"' || c == '\\' || c == '\b' || c == '\f' || c == '\n' ||
         c == '\r' || c == '\t';
}

constexpr bool is_ascii_control_char(char c) { return c >= 0 && c <= 0x1f; }

inline std::string json_escape(const std::string &s, bool add_quotes = true) {
  // Calculate the size of the resulting string.
  // Add space for the double quotes.
  size_t required_length = add_quotes ? 2 : 0;
  for (auto c : s) {
    if (is_json_special_char(c)) {
      // '\' and a single following character
      required_length += 2;
      continue;
    }
    if (is_ascii_control_char(c)) {
      // '\', 'u', 4 digits
      required_length += 6;
      continue;
    }
    ++required_length;
  }
  // Allocate memory for resulting string only once.
  std::string result;
  result.reserve(required_length);
  if (add_quotes) {
    result += '"';
  }
  // Copy string while escaping characters.
  for (auto c : s) {
    if (is_json_special_char(c)) {
      static constexpr char special_escape_table[256] =
          "\0\0\0\0\0\0\0\0btn\0fr\0\0"
          "\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0"
          "\0\0\"\0\0\0\0\0\0\0\0\0\0\0\0\0"
          "\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0"
          "\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0"
          "\0\0\0\0\0\0\0\0\0\0\0\0\\";
      result += '\\';
      // NOLINTNEXTLINE(cppcoreguidelines-pro-bounds-constant-array-index)
      result += special_escape_table[static_cast<unsigned char>(c)];
      continue;
    }
    if (is_ascii_control_char(c)) {
      // Escape as \u00xx
      static constexpr char hex_alphabet[]{"0123456789abcdef"};
      auto uc = static_cast<unsigned char>(c);
      auto h = (uc >> 4) & 0x0f;
      auto l = uc & 0x0f;
      result += "\\u00";
      // NOLINTBEGIN(cppcoreguidelines-pro-bounds-constant-array-index)
      result += hex_alphabet[h];
      result += hex_alphabet[l];
      // NOLINTEND(cppcoreguidelines-pro-bounds-constant-array-index)
      continue;
    }
    result += c;
  }
  if (add_quotes) {
    result += '"';
  }
  // Should have calculated the exact amount of memory needed
  assert(required_length == result.size());
  return result;
}

inline int json_unescape(const char *s, size_t n, char *out) {
  int r = 0;
  if (*s++ != '"') {
    return -1;
  }
  while (n > 2) {
    char c = *s;
    if (c == '\\') {
      s++;
      n--;
      switch (*s) {
      case 'b':
        c = '\b';
        break;
      case 'f':
        c = '\f';
        break;
      case 'n':
        c = '\n';
        break;
      case 'r':
        c = '\r';
        break;
      case 't':
        c = '\t';
        break;
      case '\\':
        c = '\\';
        break;
      case '/':
        c = '/';
        break;
      case '\"':
        c = '\"';
        break;
      default: // TODO: support unicode decoding
        return -1;
      }
    }
    if (out != nullptr) {
      *out++ = c;
    }
    s++;
    n--;
    r++;
  }
  if (*s != '"') {
    return -1;
  }
  if (out != nullptr) {
    *out = '\0';
  }
  return r;
}

inline std::string json_parse(const std::string &s, const std::string &key,
                              const int index) {
  const char *value;
  size_t value_sz;
  if (key.empty()) {
    json_parse_c(s.c_str(), s.length(), nullptr, index, &value, &value_sz);
  } else {
    json_parse_c(s.c_str(), s.length(), key.c_str(), key.length(), &value,
                 &value_sz);
  }
  if (value != nullptr) {
    if (value[0] != '"') {
      return {value, value_sz};
    }
    int n = json_unescape(value, value_sz, nullptr);
    if (n > 0) {
      char *decoded = new char[n + 1];
      json_unescape(value, value_sz, decoded);
      std::string result(decoded, n);
      delete[] decoded;
      return result;
    }
  }
  return "";
}

} // namespace detail
} // namespace webview

#endif // defined(__cplusplus) && !defined(WEBVIEW_HEADER)
#endif // WEBVIEW_DETAIL_JSON_HH
// file end: include/webview/lib/json.hh
// file begin: include/webview/errors/exceptions.hh
#ifndef WEBVIEW_DETAIL_EXCEPTIONS_HH
#define WEBVIEW_DETAIL_EXCEPTIONS_HH

#if defined(__cplusplus) && !defined(WEBVIEW_HEADER)

#include <exception>

namespace webview {
namespace detail {

class bad_access : public std::exception {};

} // namespace detail
} // namespace webview

#endif // defined(__cplusplus) && !defined(WEBVIEW_HEADER)
#endif // WEBVIEW_DETAIL_EXCEPTIONS_HH
// file end: include/webview/errors/exceptions.hh
// file begin: include/webview/log/ansi_colours.hh
#ifndef WEBVIEW_LOG_ANSI_COLOURS_HH
#define WEBVIEW_LOG_ANSI_COLOURS_HH

#if defined(__cplusplus) && !defined(WEBVIEW_HEADER)
#include <string>

namespace webview {
namespace log {
namespace _classes {

class ansi_t {
public:
  ~ansi_t() = default;
  ansi_t() = default;
  struct ansi_colours_t {
    std::string yellow = yellow_s();
    std::string yellow_dim = yellow_dim_s();
    std::string green = green_s();
    std::string red = red_s();
    std::string blue = blue_s();
    std::string blue_dark = blue_dark_s();
    std::string magenta = magenta_s();
    std::string default_c = default_c_s();
    std::string bold = bold_s();
    std::string dim = dim_s();
  } ansi{};

private:
  static const std::string &yellow_s();
  static const std::string &yellow_dim_s();
  static const std::string &green_s();
  static const std::string &red_s();
  static const std::string &blue_s();
  static const std::string &blue_dark_s();
  static const std::string &magenta_s();
  static const std::string &default_c_s();
  static const std::string &bold_s();
  static const std::string &dim_s();

  static std::string to_ansi_string(std::initializer_list<int> codes) noexcept;
};

} // namespace _classes
} // namespace log
} // namespace webview

#endif // defined(__cplusplus) && !defined(WEBVIEW_HEADER)
#endif // WEBVIEW_LOG_ANSI_COLOURS_HH
// file end: include/webview/log/ansi_colours.hh
// file begin: include/webview/errors/errors.hh
#ifndef WEBVIEW_ERRORS_HH
#define WEBVIEW_ERRORS_HH

#if defined(__cplusplus) && !defined(WEBVIEW_HEADER)
#include <exception>
#include <string>

namespace webview {

class error_info {
public:
  error_info(webview_error_t code, const std::string &message = {}) noexcept
      : m_code{code}, m_message{message} {}
  error_info() = default;

  webview_error_t code() const { return m_code; }
  const std::string &message() const { return m_message; }

private:
  webview_error_t m_code{WEBVIEW_ERROR_UNSPECIFIED};
  std::string m_message;
};

class exception : public std::exception {
public:
  exception(webview_error_t code, const std::string &message,
            std::exception_ptr cause) noexcept
      : exception{error_info{code, message}, cause} {}

  exception(webview_error_t code, const std::string &message) noexcept
      : exception{error_info{code, message}} {}

  exception(const error_info &error, std::exception_ptr cause) noexcept
      : m_error{error},
        // NOLINTNEXTLINE(bugprone-throw-keyword-missing)
        m_cause{cause} {}

  exception(const error_info &error) noexcept : m_error{error} {}

  exception() = default;

  const error_info &error() const { return m_error; }
  std::exception_ptr cause() const { return m_cause; }

  const char *what() const noexcept override {
    return m_error.message().c_str();
  }

private:
  error_info m_error{WEBVIEW_ERROR_UNSPECIFIED};
  std::exception_ptr m_cause;
};

} // namespace webview

#endif // defined(__cplusplus) && !defined(WEBVIEW_HEADER)
#endif // WEBVIEW_ERRORS_HH
// file end: include/webview/errors/errors.hh
// file begin: include/webview/detail/platform/linux/gtk/compat.hh
#ifndef WEBVIEW_PLATFORM_LINUX_GTK_COMPAT_HH
#define WEBVIEW_PLATFORM_LINUX_GTK_COMPAT_HH

#if defined(__cplusplus) && !defined(WEBVIEW_HEADER)
#if defined(WEBVIEW_PLATFORM_LINUX) && defined(WEBVIEW_GTK)

#include <gtk/gtk.h>

#if GTK_MAJOR_VERSION >= 4

#ifdef GDK_WINDOWING_X11
#include <gdk/x11/gdkx.h>
#endif

#elif GTK_MAJOR_VERSION >= 3

#ifdef GDK_WINDOWING_X11
#include <gdk/gdkx.h>
#endif

#endif

namespace webview {
namespace detail {

/**
 * GTK compatibility helper class.
 */
class gtk_compat {
public:
  static gboolean init_check() {
#if GTK_MAJOR_VERSION >= 4
    return gtk_init_check();
#else
    return gtk_init_check(nullptr, nullptr);
#endif
  }

  static GtkWidget *window_new() {
#if GTK_MAJOR_VERSION >= 4
    return gtk_window_new();
#else
    return gtk_window_new(GTK_WINDOW_TOPLEVEL);
#endif
  }

  static void window_set_child(GtkWindow *window, GtkWidget *widget) {
#if GTK_MAJOR_VERSION >= 4
    gtk_window_set_child(window, widget);
#else
    gtk_container_add(GTK_CONTAINER(window), widget);
#endif
  }

  static void window_remove_child(GtkWindow *window, GtkWidget *widget) {
#if GTK_MAJOR_VERSION >= 4
    if (gtk_window_get_child(window) == widget) {
      gtk_window_set_child(window, nullptr);
    }
#else
    gtk_container_remove(GTK_CONTAINER(window), widget);
#endif
  }

  static void widget_set_visible(GtkWidget *widget, bool visible) {
#if GTK_MAJOR_VERSION >= 4
    gtk_widget_set_visible(widget, visible ? TRUE : FALSE);
#else
    if (visible) {
      gtk_widget_show(widget);
    } else {
      gtk_widget_hide(widget);
    }
#endif
  }

  static void window_set_size(GtkWindow *window, int width, int height) {
    // GTK 4 can set a default window size, but unlike GTK 3 it can't resize
    // the window after it has been set up.
#if GTK_MAJOR_VERSION >= 4
    gtk_window_set_default_size(window, width, height);
#else
    gtk_window_resize(window, width, height);
#endif
  }

  static void window_set_max_size(GtkWindow *window, int width, int height) {
// X11-specific features are available in GTK 3 but not GTK 4
#if GTK_MAJOR_VERSION < 4
    GdkGeometry g{};
    g.max_width = width;
    g.max_height = height;
    GdkWindowHints h = GDK_HINT_MAX_SIZE;
    gtk_window_set_geometry_hints(GTK_WINDOW(window), nullptr, &g, h);
#else
    // Avoid "unused parameter" warnings
    (void)window;
    (void)width;
    (void)height;
#endif
  }
};

} // namespace detail
} // namespace webview

#endif // defined(WEBVIEW_PLATFORM_LINUX) && defined(WEBVIEW_GTK)
#endif // defined(__cplusplus) && !defined(WEBVIEW_HEADER)
#endif // WEBVIEW_PLATFORM_LINUX_GTK_COMPAT_HH
// file end: include/webview/detail/platform/linux/gtk/compat.hh
// file begin: include/webview/detail/platform/linux/webkitgtk/compat.hh
#ifndef WEBVIEW_PLATFORM_LINUX_WEBKITGTK_COMPAT_HH
#define WEBVIEW_PLATFORM_LINUX_WEBKITGTK_COMPAT_HH

#if defined(__cplusplus) && !defined(WEBVIEW_HEADER)
#if defined(WEBVIEW_PLATFORM_LINUX) && defined(WEBVIEW_GTK)

#include <functional>
#include <string>

#include <gtk/gtk.h>

#if GTK_MAJOR_VERSION >= 4

#include <jsc/jsc.h>
#include <webkit/webkit.h>

#ifdef GDK_WINDOWING_X11
#include <gdk/x11/gdkx.h>
#endif

#elif GTK_MAJOR_VERSION >= 3

#include <JavaScriptCore/JavaScript.h>
#include <webkit2/webkit2.h>

#ifdef GDK_WINDOWING_X11
#include <gdk/gdkx.h>
#endif

#endif

namespace webview {
namespace detail {

/**
 * WebKitGTK compatibility helper class.
 */
class webkitgtk_compat {
public:
#if GTK_MAJOR_VERSION >= 4
  using wk_handler_js_value_t = JSCValue;
#else
  using wk_handler_js_value_t = WebKitJavascriptResult;
#endif

  using on_script_message_received_t =
      std::function<void(WebKitUserContentManager *, const std::string &)>;
  static void
  connect_script_message_received(WebKitUserContentManager *manager,
                                  const std::string &handler_name,
                                  on_script_message_received_t handler) {
    std::string signal_name = "script-message-received::";
    signal_name += handler_name;

    auto callback = +[](WebKitUserContentManager *manager,
                        wk_handler_js_value_t *r, gpointer arg) {
      auto *handler = static_cast<on_script_message_received_t *>(arg);
      (*handler)(manager, get_string_from_js_result(r));
    };

    auto deleter = +[](gpointer data, GClosure *) {
      delete static_cast<on_script_message_received_t *>(data);
    };

    g_signal_connect_data(manager, signal_name.c_str(), G_CALLBACK(callback),
                          new on_script_message_received_t{handler}, deleter,
                          static_cast<GConnectFlags>(0) /*G_CONNECT_DEFAULT*/);
  }

  static std::string get_string_from_js_result(JSCValue *r) {
    char *cs = jsc_value_to_string(r);
    std::string s{cs};
    g_free(cs);
    return s;
  }

#if GTK_MAJOR_VERSION < 4
  static std::string get_string_from_js_result(WebKitJavascriptResult *r) {
#if (WEBKIT_MAJOR_VERSION == 2 && WEBKIT_MINOR_VERSION >= 22) ||               \
    WEBKIT_MAJOR_VERSION > 2
    JSCValue *value = webkit_javascript_result_get_js_value(r);
    return get_string_from_js_result(value);
#else
    JSGlobalContextRef ctx = webkit_javascript_result_get_global_context(r);
    JSValueRef value = webkit_javascript_result_get_value(r);
    JSStringRef js = JSValueToStringCopy(ctx, value, nullptr);
    size_t n = JSStringGetMaximumUTF8CStringSize(js);
    char *cs = g_new(char, n);
    JSStringGetUTF8CString(js, cs, n);
    JSStringRelease(js);
    std::string s{cs};
    g_free(cs);
    return s;
#endif
  }
#endif

  static void user_content_manager_register_script_message_handler(
      WebKitUserContentManager *manager, const gchar *name) {
#if GTK_MAJOR_VERSION >= 4
    webkit_user_content_manager_register_script_message_handler(manager, name,
                                                                nullptr);
#else
    webkit_user_content_manager_register_script_message_handler(manager, name);
#endif
  }
};

} // namespace detail
} // namespace webview

#endif // defined(WEBVIEW_PLATFORM_LINUX) && defined(WEBVIEW_GTK)
#endif // defined(__cplusplus) && !defined(WEBVIEW_HEADER)
#endif // WEBVIEW_PLATFORM_LINUX_WEBKITGTK_COMPAT_HH
// file end: include/webview/detail/platform/linux/webkitgtk/compat.hh
// file begin: include/webview/detail/platform/linux/webkitgtk/dmabuf.hh
#ifndef WEBVIEW_BACKENDS_GTK_WEBKITGTK_DMABUF_HH
#define WEBVIEW_BACKENDS_GTK_WEBKITGTK_DMABUF_HH

#if defined(__cplusplus) && !defined(WEBVIEW_HEADER)
#if defined(WEBVIEW_PLATFORM_LINUX) && defined(WEBVIEW_GTK)

#include <cstdlib>
#include <string>

#include <gtk/gtk.h>

#if GTK_MAJOR_VERSION >= 4

#include <jsc/jsc.h>
#include <webkit/webkit.h>

#ifdef GDK_WINDOWING_X11
#include <gdk/x11/gdkx.h>
#endif

#elif GTK_MAJOR_VERSION >= 3

#include <JavaScriptCore/JavaScript.h>
#include <webkit2/webkit2.h>

#ifdef GDK_WINDOWING_X11
#include <gdk/gdkx.h>
#endif

#endif

#include <fcntl.h>
#include <sys/stat.h>

namespace webview {
namespace detail {

// Namespace containing workaround for WebKit 2.42 when using NVIDIA GPU
// driver.
// See WebKit bug: https://bugs.webkit.org/show_bug.cgi?id=261874
// Please remove all of the code in this namespace when it's no longer needed.
namespace webkit_dmabuf {

// Get environment variable. Not thread-safe.
static inline std::string get_env(const std::string &name) {
  auto *value = std::getenv(name.c_str());
  if (value) {
    return {value};
  }
  return {};
}

// Set environment variable. Not thread-safe.
static inline void set_env(const std::string &name, const std::string &value) {
  ::setenv(name.c_str(), value.c_str(), 1);
}

// Checks whether the NVIDIA GPU driver is used based on whether the kernel
// module is loaded.
static inline bool is_using_nvidia_driver() {
  struct ::stat buffer{};
  if (::stat("/sys/module/nvidia", &buffer) != 0) {
    return false;
  }
  return S_ISDIR(buffer.st_mode);
}

// Checks whether the windowing system is Wayland.
static inline bool is_wayland_display() {
  if (!get_env("WAYLAND_DISPLAY").empty()) {
    return true;
  }
  if (get_env("XDG_SESSION_TYPE") == "wayland") {
    return true;
  }
  if (get_env("DESKTOP_SESSION").find("wayland") != std::string::npos) {
    return true;
  }
  return false;
}

// Checks whether the GDK X11 backend is used.
// See: https://docs.gtk.org/gdk3/class.DisplayManager.html
static inline bool is_gdk_x11_backend() {
#ifdef GDK_WINDOWING_X11
  auto *gdk_display = gdk_display_get_default();
  return GDK_IS_X11_DISPLAY(gdk_display); // NOLINT(misc-const-correctness)
#else
  return false;
#endif
}

// Checks whether WebKit is affected by bug when using DMA-BUF renderer.
// Returns true if all of the following conditions are met:
//  - WebKit version is >= 2.42 (please narrow this down when there's a fix).
//  - Environment variables are empty or not set:
//    - WEBKIT_DISABLE_DMABUF_RENDERER
//  - Windowing system is not Wayland.
//  - GDK backend is X11.
//  - NVIDIA GPU driver is used.
static inline bool is_webkit_dmabuf_bugged() {
  auto wk_major = webkit_get_major_version();
  auto wk_minor = webkit_get_minor_version();
  // TODO: Narrow down affected WebKit version when there's a fixed version
  auto is_affected_wk_version = wk_major == 2 && wk_minor >= 42;
  if (!is_affected_wk_version) {
    return false;
  }
  if (!get_env("WEBKIT_DISABLE_DMABUF_RENDERER").empty()) {
    return false;
  }
  if (is_wayland_display()) {
    return false;
  }
  if (!is_gdk_x11_backend()) {
    return false;
  }
  if (!is_using_nvidia_driver()) {
    return false;
  }
  return true;
}

// Applies workaround for WebKit DMA-BUF bug if needed.
// See WebKit bug: https://bugs.webkit.org/show_bug.cgi?id=261874
static inline void apply_webkit_dmabuf_workaround() {
  if (!is_webkit_dmabuf_bugged()) {
    return;
  }
  set_env("WEBKIT_DISABLE_DMABUF_RENDERER", "1");
}

} // namespace webkit_dmabuf
} // namespace detail
} // namespace webview

#endif // defined(WEBVIEW_PLATFORM_LINUX) && defined(WEBVIEW_GTK)
#endif // defined(__cplusplus) && !defined(WEBVIEW_HEADER)
#endif // WEBVIEW_BACKENDS_GTK_WEBKITGTK_DMABUF_HH
// file end: include/webview/detail/platform/linux/webkitgtk/dmabuf.hh
// file begin: include/webview/lib/version.h
#ifndef WEBVIEW_VERSION_H
#define WEBVIEW_VERSION_H

/// @{

#ifndef WEBVIEW_VERSION_MAJOR
/// The current library major version.
#define WEBVIEW_VERSION_MAJOR 0
#endif

#ifndef WEBVIEW_VERSION_MINOR
/// The current library minor version.
#define WEBVIEW_VERSION_MINOR 12
#endif

#ifndef WEBVIEW_VERSION_PATCH
/// The current library patch version.
#define WEBVIEW_VERSION_PATCH 0
#endif

#ifndef WEBVIEW_VERSION_PRE_RELEASE
/// SemVer 2.0.0 pre-release labels prefixed with "-".
#define WEBVIEW_VERSION_PRE_RELEASE ""
#endif

#ifndef WEBVIEW_VERSION_BUILD_METADATA
/// SemVer 2.0.0 build metadata prefixed with "+".
#define WEBVIEW_VERSION_BUILD_METADATA ""
#endif

/// SemVer 2.0.0 version number in MAJOR.MINOR.PATCH format.
#define WEBVIEW_VERSION_NUMBER                                                 \
  WEBVIEW_EXPAND_AND_STRINGIFY(WEBVIEW_VERSION_MAJOR)                          \
  "." WEBVIEW_EXPAND_AND_STRINGIFY(                                            \
      WEBVIEW_VERSION_MINOR) "." WEBVIEW_EXPAND_AND_STRINGIFY(WEBVIEW_VERSION_PATCH)

/// @}

#endif // WEBVIEW_VERSION_H
// file end: include/webview/lib/version.h
// file begin: include/webview/api/api.h
#ifndef WEBVIEW_API_H
#define WEBVIEW_API_H
#ifdef __cplusplus
extern "C" {
#endif

/**
 * Creates a new webview instance.
 *
 * @param debug Enable developer tools if supported by the backend.
 * @param window Optional native window handle, i.e. @c GtkWindow pointer
 *        @c NSWindow pointer (Cocoa) or @c HWND (Win32). If non-null,
 *        the webview widget is embedded into the given window, and the
 *        caller is expected to assume responsibility for the window as
 *        well as application lifecycle. If the window handle is null,
 *        a new window is created and both the window and application
 *        lifecycle are managed by the webview instance.
 * @remark Win32: The function also accepts a pointer to @c HWND (Win32) in the
 *         window parameter for backward compatibility.
 * @remark Win32/WebView2: @c CoInitializeEx should be called with
 *         @c COINIT_APARTMENTTHREADED before attempting to call this function
 *         with an existing window. Omitting this step may cause WebView2
 *         initialization to fail.
 * @return @c NULL on failure. Creation can fail for various reasons such
 *         as when required runtime dependencies are missing or when window
 *         creation fails.
 * @retval WEBVIEW_ERROR_MISSING_DEPENDENCY
 *         May be returned if WebView2 is unavailable on Windows.
 */
WEBVIEW_API webview_t webview_create(int debug, void *window);

/**
 * Destroys a webview instance and closes the native window.
 *
 * @param w The webview instance.
 */
WEBVIEW_API webview_error_t webview_destroy(webview_t w);

/**
 * Runs the main loop until it's terminated.
 *
 * @param w The webview instance.
 */
WEBVIEW_API webview_error_t webview_run(webview_t w);

/**
 * Stops the main loop. It is safe to call this function from another other
 * background thread.
 *
 * @param w The webview instance.
 */
WEBVIEW_API webview_error_t webview_terminate(webview_t w);

/**
 * Schedules a function to be invoked on the thread with the run/event loop.
 *
 * Since library functions generally do not have thread safety guarantees,
 * this function can be used to schedule code to execute on the main/GUI
 * thread and thereby make that execution safe in multi-threaded applications.
 *
 * @param w The webview instance.
 * @param fn The function to be invoked.
 * @param arg An optional argument passed along to the callback function.
 */
WEBVIEW_API webview_error_t webview_dispatch(webview_t w,
                                             void (*fn)(webview_t w, void *arg),
                                             void *arg);

/**
 * Returns the native handle of the window associated with the webview instance.
 * The handle can be a @c GtkWindow pointer (GTK), @c NSWindow pointer (Cocoa)
 * or @c HWND (Win32).
 *
 * @param w The webview instance.
 * @return The handle of the native window.
 */
WEBVIEW_API void *webview_get_window(webview_t w);

/**
 * Get a native handle of choice.
 *
 * @param w The webview instance.
 * @param kind The kind of handle to retrieve.
 * @return The native handle or @c NULL.
 * @since 0.11
 */
WEBVIEW_API void *webview_get_native_handle(webview_t w,
                                            webview_native_handle_kind_t kind);

/**
 * Updates the title of the native window.
 *
 * @param w The webview instance.
 * @param title The new title.
 */
WEBVIEW_API webview_error_t webview_set_title(webview_t w, const char *title);

/**
 * Updates the size of the native window.
 *
 * Remarks:
 * - Using WEBVIEW_HINT_MAX for setting the maximum window size is not
 *   supported with GTK 4 because X11-specific functions such as
 *   gtk_window_set_geometry_hints were removed. This option has no effect
 *   when using GTK 4.
 * - GTK 4 can set a default/initial window size if done early enough;
 *   otherwise, this function has no effect. GTK 4 (unlike 3) can't resize
 *   a window after it has been set up.
 *
 * @param w The webview instance.
 * @param width New width.
 * @param height New height.
 * @param hints Size hints.
 */
WEBVIEW_API webview_error_t webview_set_size(webview_t w, int width, int height,
                                             webview_hint_t hints);

/**
 * Navigates webview to the given URL. URL may be a properly encoded data URI.
 *
 * Example:
 * @code{.c}
 * webview_navigate(w, "https://github.com/webview/webview");
 * webview_navigate(w, "data:text/html,%3Ch1%3EHello%3C%2Fh1%3E");
 * webview_navigate(w, "data:text/html;base64,PGgxPkhlbGxvPC9oMT4=");
 * @endcode
 *
 * @param w The webview instance.
 * @param url URL.
 */
WEBVIEW_API webview_error_t webview_navigate(webview_t w, const char *url);

/**
 * Load HTML content into the webview.
 *
 * Example:
 * @code{.c}
 * webview_set_html(w, "<h1>Hello</h1>");
 * @endcode
 *
 * @param w The webview instance.
 * @param html HTML content.
 */
WEBVIEW_API webview_error_t webview_set_html(webview_t w, const char *html);

/**
 * Injects JavaScript code to be executed immediately upon loading a page.
 * The code will be executed before @c window.onload.
 *
 * @param w The webview instance.
 * @param js JS content.
 */
WEBVIEW_API webview_error_t webview_init(webview_t w, const char *js);

/**
 * Evaluates arbitrary JavaScript code.
 *
 * Use bindings if you need to communicate the result of the evaluation.
 *
 * @param w The webview instance.
 * @param js JS content.
 */
WEBVIEW_API webview_error_t webview_eval(webview_t w, const char *js);

/**
 * Binds a function pointer to a new global JavaScript function.
 *
 * Internally, JS glue code is injected to create the JS function by the
 * given name. The callback function is passed a request identifier,
 * a request string and a user-provided argument. The request string is
 * a JSON array of the arguments passed to the JS function.
 *
 * @param w The webview instance.
 * @param name Name of the JS function.
 * @param fn Callback function.
 * @param arg User argument.
 * @retval WEBVIEW_ERROR_DUPLICATE
 *         A binding already exists with the specified name.
 */
WEBVIEW_API webview_error_t webview_bind(webview_t w, const char *name,
                                         void (*fn)(const char *id,
                                                    const char *req, void *arg),
                                         void *arg);

/**
 * Removes a binding created with webview_bind().
 *
 * @param w The webview instance.
 * @param name Name of the binding.
 * @retval WEBVIEW_ERROR_NOT_FOUND No binding exists with the specified name.
 */
WEBVIEW_API webview_error_t webview_unbind(webview_t w, const char *name);

/**
 * Responds to a binding call from the JS side.
 *
 * This function is safe to call from another thread.
 *
 * @param w The webview instance.
 * @param id The identifier of the binding call. Pass along the value received
 *           in the binding handler (see webview_bind()).
 * @param status A status of zero tells the JS side that the binding call was
 *               successful; any other value indicates an error.
 * @param result The result of the binding call to be returned to the JS side.
 *               This must either be a valid JSON value or an empty string for
 *               the primitive JS value @c undefined.
 */
WEBVIEW_API webview_error_t webview_return(webview_t w, const char *id,
                                           int status, const char *result);

/**
 * Get the library's version information.
 *
 * @since 0.10
 */
WEBVIEW_API const webview_version_info_t *webview_version(void);

#ifdef __cplusplus
}
#endif

#endif // WEBVIEW_API_H
// file end: include/webview/api/api.h
// file begin: include/webview/lib/json_deprecated.hh
#ifndef WEBVIEW_JSON_HH
#define WEBVIEW_JSON_HH

#if defined(__cplusplus) && !defined(WEBVIEW_HEADER)
namespace webview {

WEBVIEW_DEPRECATED_PRIVATE
inline int json_parse_c(const char *s, size_t sz, const char *key, size_t keysz,
                        const char **value, size_t *valuesz) {
  return detail::json_parse_c(s, sz, key, keysz, value, valuesz);
}

WEBVIEW_DEPRECATED_PRIVATE
inline std::string json_escape(const std::string &s) {
  return detail::json_escape(s);
}

WEBVIEW_DEPRECATED_PRIVATE
inline int json_unescape(const char *s, size_t n, char *out) {
  return detail::json_unescape(s, n, out);
}

WEBVIEW_DEPRECATED_PRIVATE
inline std::string json_parse(const std::string &s, const std::string &key,
                              const int index) {
  return detail::json_parse(s, key, index);
}

} // namespace webview

#endif // defined(__cplusplus) && !defined(WEBVIEW_HEADER)
#endif // WEBVIEW_JSON_HH
// file end: include/webview/lib/json_deprecated.hh
// file begin: include/webview/types/optional.hh
#ifndef WEBVIEW_DETAIL_OPTIONAL_HH
#define WEBVIEW_DETAIL_OPTIONAL_HH

#if defined(__cplusplus) && !defined(WEBVIEW_HEADER)
#include <new>
#include <type_traits>
#include <utility>

namespace webview {
namespace detail {

template <typename T> class optional {
public:
  // NOLINTNEXTLINE(cppcoreguidelines-pro-type-member-init, hicpp-member-init)
  optional() = default;

  optional(const T &other) noexcept : m_has_data{true} {
    new (&m_data) T{other};
  }

  optional(T &&other) noexcept : m_has_data{true} {
    new (&m_data) T{std::move(other)};
  }

  optional(const optional<T> &other) noexcept { *this = other; }

  optional &operator=(const optional<T> &other) noexcept {
    if (this == &other) {
      return *this;
    }
    m_has_data = other.has_value();
    if (m_has_data) {
      // NOLINTNEXTLINE(cppcoreguidelines-pro-type-reinterpret-cast)
      new (&m_data) T{*reinterpret_cast<const T *>(&other.m_data)};
    }
    return *this;
  }

  optional(optional<T> &&other) noexcept { *this = std::move(other); }

  optional &operator=(optional<T> &&other) noexcept {
    if (this == &other) {
      return *this;
    }
    m_has_data = other.has_value();
    if (m_has_data) {
      // NOLINTNEXTLINE(cppcoreguidelines-pro-type-reinterpret-cast)
      new (&m_data) T{std::move(*reinterpret_cast<T *>(&other.m_data))};
    }
    return *this;
  }

  ~optional() {
    if (m_has_data) {
      // NOLINTNEXTLINE(cppcoreguidelines-pro-type-reinterpret-cast)
      reinterpret_cast<T *>(&m_data)->~T();
    }
  }

  const T &get() const {
    if (!m_has_data) {
      throw bad_access{};
    }
    // NOLINTNEXTLINE(cppcoreguidelines-pro-type-reinterpret-cast)
    return *reinterpret_cast<const T *>(&m_data);
  }

  T &get() {
    if (!m_has_data) {
      throw bad_access{};
    }
    // NOLINTNEXTLINE(cppcoreguidelines-pro-type-reinterpret-cast)
    return *reinterpret_cast<T *>(&m_data);
  }

  bool has_value() const { return m_has_data; }

private:
  // NOLINTNEXTLINE(bugprone-sizeof-expression): pointer to aggregate is OK
  typename std::aligned_storage<sizeof(T), alignof(T)>::type m_data;
  bool m_has_data{};
};

template <> class optional<void> {};

} // namespace detail
} // namespace webview

#endif // defined(__cplusplus) && !defined(WEBVIEW_HEADER)
#endif // WEBVIEW_DETAIL_OPTIONAL_HH
// file end: include/webview/types/optional.hh
// file begin: include/webview/log/trace_log.hh
#ifndef WEBVIEW_LOG_TRACE_LOG_HH
#define WEBVIEW_LOG_TRACE_LOG_HH

#if defined(__cplusplus) && !defined(WEBVIEW_HEADER)
#include <chrono>
#include <mutex>
#include <string>

#if defined(_WIN32)
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif // WIN32_LEAN_AND_MEAN
#ifdef _MSC_VER
#ifndef _WIN32_WINNT
#define _WIN32_WINNT 0x0501 // Win XP
#endif                      // _WIN32_WINNT
#endif                      // _MSC_VER
#include <io.h>
#include <windows.h>
#endif

namespace webview {
namespace log {
namespace _structs {

using time_point_t = std::chrono::time_point<std::chrono::steady_clock>;
using string_t = const std::string &;

struct trace_tools_t : _classes::ansi_t {
protected:
  static std::mutex trace_mtx;

  time_point_t get_now() const;
  std::string get_ctx(char scp) const;
  long elapsed_ms(time_point_t start, time_point_t end) const;
  std::string bool_s(bool flag) const;
  std::string escape_s(string_t text) const;
  std::string num_s(size_t val) const;
  std::string bold(string_t this_col, string_t text) const;
  std::string dim(string_t this_col, string_t text) const;
  void print_ansi(string_t this_col, string_t message) const;
};

class print_here_t : protected trace_tools_t {
public:
  ~print_here_t() = default;
  print_here_t(string_t prefix, string_t postfix)
      : prefix(prefix), postfix(postfix) {}
  void print_here(string_t message) const;

private:
  std::string prefix;
  std::string postfix;
};

struct queue_print_t : public print_here_t {
public:
  queue_print_t(string_t prefix, string_t postfix)
      : print_here_t{prefix, postfix}, prefix{prefix}, postfix(postfix) {}
  void start(string_t name) const;
  void wait(string_t name) const;
  void done(bool done, string_t name) const;

private:
  std::string prefix;
  std::string postfix;
};

class queue_bind_t {
public:
  ~queue_bind_t() = default;
  queue_bind_t(string_t prefix, string_t postfix = "BIND")
      : bind{prefix, postfix} {};

  struct wrapper_t : public queue_print_t {
    ~wrapper_t() = default;
    wrapper_t(string_t prefix, string_t postfix)
        : queue_print_t{prefix, postfix} {};
  } bind;
};

class queue_unbind_t {
public:
  ~queue_unbind_t() = default;
  queue_unbind_t(string_t prefix, string_t postfix = "UNBIND")
      : unbind{prefix, postfix} {};

  struct wrapper_t : public queue_print_t {
    ~wrapper_t() = default;
    wrapper_t(string_t prefix, string_t postfix)
        : queue_print_t{prefix, postfix} {};
  } unbind;
};

class queue_eval_t {
public:
  ~queue_eval_t() = default;
  queue_eval_t(string_t prefix, string_t postfix = "EVAL")
      : eval{prefix, postfix} {};

  struct wrapper_t : public print_here_t {
    ~wrapper_t() = default;
    wrapper_t(string_t prefix, string_t postfix)
        : print_here_t(prefix, postfix), prefix(prefix), postfix(postfix) {}
    void start(string_t js) const;
    void done(bool done) const;

  private:
    std::string prefix;
    std::string postfix;
  } eval;
};

class queue_loop_t {
public:
  ~queue_loop_t() = default;
  queue_loop_t(string_t prefix, string_t postfix = "loop: ")
      : loop{prefix, postfix} {};

  struct wrapper_t : public print_here_t {
    ~wrapper_t() = default;
    wrapper_t(string_t prefix, string_t postfix)
        : print_here_t(prefix, postfix), prefix(prefix), postfix(postfix),
          process_ts(get_now()) {}
    void wait(size_t size, bool empty, bool dom_ready) const;
    void start(size_t size) const;
    void end() const;

  private:
    std::string prefix;
    std::string postfix;
    time_point_t process_ts;
    time_point_t mutable loop_wait_ts;
    time_point_t mutable loop_start_ts;
    time_point_t mutable loop_end_ts;

    void set_loop_wait_ts() const;
    void set_loop_start_ts() const;
    void set_loop_end_ts() const;
    std::string total_elapsed() const;
    std::string wait_elapsed() const;
    std::string loop_elapsed() const;

  } loop;
};

class queue_notify_t {
public:
  ~queue_notify_t() = default;
  queue_notify_t(string_t prefix, string_t postfix = "notify: ")
      : notify{prefix, postfix} {};

  struct wrapper_t : public print_here_t {
    ~wrapper_t() = default;
    wrapper_t(string_t prefix, string_t postfix)
        : print_here_t(prefix, postfix), prefix(prefix), postfix(postfix) {}
    void on_message(string_t method) const;

  private:
    std::string prefix;
    std::string postfix;
  } notify;
};

class queue_enqueue_t {
public:
  ~queue_enqueue_t() = default;
  queue_enqueue_t(string_t prefix, string_t postfix = "enqueue: ")
      : enqueue{prefix, postfix} {};

  struct wrapper_t : public print_here_t {
    ~wrapper_t() = default;
    wrapper_t(string_t prefix, string_t postfix)
        : print_here_t(prefix, postfix), prefix(prefix), postfix(postfix) {}
    void added(char scp, size_t size, string_t name_or_js) const;
    void added(char scp, size_t size) const;

  private:
    std::string prefix;
    std::string postfix;
  } enqueue;
};

class queue_t : public queue_bind_t,
                public queue_unbind_t,
                public queue_eval_t,
                public queue_loop_t,
                public queue_notify_t,
                public queue_enqueue_t,
                public print_here_t {
public:
  ~queue_t() = default;
  queue_t(string_t prefix, string_t postfix = "QUEUE: ")
      : queue_bind_t{prefix + postfix}, queue_unbind_t{prefix + postfix},
        queue_eval_t{prefix + postfix}, queue_loop_t{prefix + postfix},
        queue_notify_t{prefix + postfix}, queue_enqueue_t{prefix + postfix},
        print_here_t{prefix, postfix} {};
};

struct base_print_t : public print_here_t {
public:
  base_print_t(string_t prefix, string_t postfix)
      : print_here_t{prefix, postfix}, prefix{prefix}, postfix(postfix) {}
  void start(string_t name) const;
  void work(string_t name) const;
  void done(string_t name) const;

private:
  std::string prefix;
  std::string postfix;
};

class base_bind_t {
public:
  ~base_bind_t() = default;
  base_bind_t(string_t prefix, string_t postfix = "bind: ")
      : bind{prefix, postfix} {};

  struct wrapper_t : public base_print_t {
    ~wrapper_t() = default;
    wrapper_t(string_t prefix, string_t postfix)
        : base_print_t{prefix, postfix} {};
  } bind;
};

class base_unbind_t {
public:
  ~base_unbind_t() = default;
  base_unbind_t(string_t prefix, string_t postfix = "unbind: ")
      : unbind{prefix, postfix} {};

  struct wrapper_t : public base_print_t {
    ~wrapper_t() = default;
    wrapper_t(string_t prefix, string_t postfix)
        : base_print_t{prefix, postfix} {};
  } unbind;
};

class base_eval_t {
public:
  ~base_eval_t() = default;
  base_eval_t(string_t prefix, string_t postfix = "eval: ")
      : eval{prefix, postfix} {};

  struct wrapper_t : public print_here_t {
    ~wrapper_t() = default;
    wrapper_t(string_t prefix, string_t postfix)
        : print_here_t{prefix, postfix}, prefix(prefix), postfix(postfix) {}
    void start(string_t js, bool skip_queue) const;
    void work(string_t js) const;
    void done(bool done, string_t js) const;

  private:
    std::string prefix;
    std::string postfix;
  } eval;
};

class base_t : public base_bind_t,
               public base_unbind_t,
               public base_eval_t,
               public print_here_t {
public:
  ~base_t() = default;
  base_t(string_t prefix, string_t postfix = "BASE: ")
      : base_bind_t(prefix + postfix), base_unbind_t(prefix + postfix),
        base_eval_t(prefix + postfix), print_here_t(prefix, postfix) {}
};

} // namespace _structs

class trace {
  ~trace() = default;
#if defined(_WIN32)
  trace() {
    AttachConsole(ATTACH_PARENT_PROCESS);
    static_cast<void>(freopen("CONOUT$", "w", stdout));
  };
#else
  trace() = default;
#endif
public:
  static const _structs::base_t &base;
  static const _structs::queue_t &queue;

private:
  static const std::string &prefix() {
    static const std::string prefix_instance = "WEBVIEW: ";
    return prefix_instance;
  };
  static const _structs::base_t &get_base() noexcept {
    static const _structs::base_t base_instance = {prefix()};
    return base_instance;
  }
  static const _structs::queue_t &get_queue() noexcept {
    static const _structs::queue_t queue_instance = {prefix()};
    return queue_instance;
  }
};

} // namespace log
} // namespace webview

#endif // defined(__cplusplus) && !defined(WEBVIEW_HEADER)
#endif // WEBVIEW_LOG_TRACE_LOG_HH
// file end: include/webview/log/trace_log.hh
// file begin: src/log/ansi_colours.cc
#ifndef WEBVIEW_LOG_ANSI_COLOURS_CC
#define WEBVIEW_LOG_ANSI_COLOURS_CC

#if defined(__cplusplus) && !defined(WEBVIEW_HEADER)
namespace webview {
namespace log {
namespace _classes {

std::string ansi_t::to_ansi_string(std::initializer_list<int> codes) noexcept {
  std::string ansi_string;
  for (size_t i = 0; i < codes.size(); ++i) {
    auto code = std::to_string(*(codes.begin() + i));
    if (i == 0 && codes.size() == 1) {
      ansi_string += "\033[" + code;
    } else if (i == 0) {
      ansi_string += "\033[" + code + ";";
    } else if (i == codes.size() - 1) {
      ansi_string += code;
    } else {
      ansi_string += code + ";";
    }
  };
  ansi_string += "m";
  return ansi_string;
}

const std::string &ansi_t::yellow_s() {
  static const std::string instance = to_ansi_string({33});
  return instance;
}
const std::string &ansi_t::yellow_dim_s() {
  static const std::string instance = to_ansi_string({33, 2});
  return instance;
}
const std::string &ansi_t::green_s() {
  static const std::string instance = to_ansi_string({92});
  return instance;
}
const std::string &ansi_t::red_s() {
  static const std::string instance = to_ansi_string({91});
  return instance;
}
const std::string &ansi_t::blue_s() {
  static const std::string instance = to_ansi_string({94});
  return instance;
}
const std::string &ansi_t::blue_dark_s() {
  static const std::string instance = to_ansi_string({34});
  return instance;
}
const std::string &ansi_t::magenta_s() {
  static const std::string instance = to_ansi_string({95});
  return instance;
}
const std::string &ansi_t::default_c_s() {
  static const std::string instance = to_ansi_string({0});
  return instance;
}
const std::string &ansi_t::bold_s() {
  static const std::string instance = to_ansi_string({1});
  return instance;
}
const std::string &ansi_t::dim_s() {
  static const std::string instance = to_ansi_string({2});
  return instance;
}

} // namespace _classes
} // namespace log
} // namespace webview

#endif // defined(__cplusplus) && !defined(WEBVIEW_HEADER)
#endif // WEBVIEW_LOG_ANSI_COLOURS_CC
// file end: src/log/ansi_colours.cc
// file begin: include/webview/types/basic_result.hh
#ifndef WEBVIEW_DETAIL_BASIC_RESULT_HH
#define WEBVIEW_DETAIL_BASIC_RESULT_HH

#if defined(__cplusplus) && !defined(WEBVIEW_HEADER)
#include <utility>

namespace webview {
namespace detail {

template <typename Value, typename Error, typename Exception>
class basic_result {
public:
  using value_type = Value;
  using error_type = Error;
  using exception_type = Exception;

  basic_result() : basic_result(value_type{}) {}

  basic_result(const value_type &value) : m_value{value} {}
  basic_result(value_type &&value) : m_value{std::forward<value_type>(value)} {}

  basic_result(const error_type &error) : m_error{error} {}
  basic_result(error_type &&error) : m_error{std::forward<error_type>(error)} {}

  bool ok() const { return has_value() && !has_error(); }
  bool has_value() const { return m_value.has_value(); }
  bool has_error() const { return m_error.has_value(); }

  void ensure_ok() {
    if (!ok()) {
      throw exception_type{error()};
    }
  }

  const value_type &value() const {
    if (!has_value()) {
      throw bad_access{};
    }
    return m_value.get();
  }

  const error_type &error() const {
    if (!has_error()) {
      throw bad_access{};
    }
    return m_error.get();
  }

private:
  optional<value_type> m_value;
  optional<error_type> m_error;
};

template <typename Error, typename Exception>
class basic_result<void, Error, Exception> {
public:
  using value_type = void;
  using error_type = Error;
  using exception_type = Exception;

  basic_result() = default;

  basic_result(error_type &&error) : m_error{std::forward<error_type>(error)} {}

  bool ok() const { return !has_error(); }

  bool has_error() const { return m_error.has_value(); }

  void ensure_ok() {
    if (!ok()) {
      throw exception_type{error()};
    }
  }

  const error_type &error() const {
    if (!has_error()) {
      throw bad_access{};
    }
    return m_error.get();
  }

private:
  optional<error_type> m_error;
};

} // namespace detail
} // namespace webview

#endif // defined(__cplusplus) && !defined(WEBVIEW_HEADER)
#endif // WEBVIEW_DETAIL_BASIC_RESULT_HH
// file end: include/webview/types/basic_result.hh
// file begin: src/log/trace_log.cc
#ifndef WEBVIEW_LOG_TRACE_LOG_CC
#define WEBVIEW_LOG_TRACE_LOG_CC

#if defined(__cplusplus) && !defined(WEBVIEW_HEADER)
namespace webview {
namespace log {
namespace _structs {

std::mutex trace_tools_t::trace_mtx;

time_point_t trace_tools_t::get_now() const {
  return std::chrono::steady_clock::now();
}
std::string trace_tools_t::get_ctx(char scp) const {
  if (scp == 'b') {
    return "bind";
  }
  if (scp == 'u') {
    return "unbind";
  }
  return "eval";
}
long trace_tools_t::elapsed_ms(time_point_t start, time_point_t end) const {
  return static_cast<long int>(
      std::chrono::duration_cast<std::chrono::milliseconds>(end - start)
          .count());
};
std::string trace_tools_t::bool_s(bool flag) const {
  return flag ? "true" : "false";
}
std::string trace_tools_t::escape_s(string_t text) const {
  return "\"" + text + "\"";
}
std::string trace_tools_t::num_s(size_t val) const {
  return std::to_string(val);
}
std::string trace_tools_t::bold(string_t this_col, string_t text) const {
  return ansi.bold + text + ansi.default_c + this_col;
}
std::string trace_tools_t::dim(string_t this_col, string_t text) const {
  return ansi.dim + text + ansi.default_c + this_col;
}
void trace_tools_t::print_ansi(string_t this_col, string_t message) const {
  std::lock_guard<std::mutex> lock(trace_mtx);
  printf("%s%s%s\n", this_col.c_str(), message.c_str(), ansi.default_c.c_str());
};

void print_here_t::print_here(string_t message) const {
  auto this_c = ansi.magenta;
  auto here_m = bold(this_c, "here") + ": ";
  auto message_ = ansi.default_c + message;
  print_ansi(this_c, prefix + postfix + here_m + message_);
};

#if WEBVIEW_LOG_TRACE
void queue_print_t::start(string_t name) const {

  auto this_c = ansi.blue;
  auto postfix_m = bold(this_c, postfix) + ": ";
  auto start_m = bold(this_c, "START: ") + escape_s(name);
  print_ansi(this_c, prefix + postfix_m + start_m);
}
#else
void queue_print_t::start(string_t /**/) const {}
#endif
#if WEBVIEW_LOG_TRACE
void queue_print_t::wait(string_t name) const {
  auto this_c = ansi.yellow_dim;
  auto postfix_m = bold(this_c, postfix) + ": ";
  auto wait_m = bold(this_c, "WAIT: ") + escape_s(name);
  print_ansi(this_c, prefix + postfix_m + wait_m);
}
#else
void queue_print_t::wait(string_t /**/) const {}
#endif
#if WEBVIEW_LOG_TRACE
void queue_print_t::done(bool done, string_t name) const {
  auto this_c = ansi.blue;
  auto postfix_m = bold(this_c, postfix) + ": ";
  auto done_m = bold(this_c, "work_done: ") + bool_s(done) + ": ";
  auto name_m = escape_s(name);
  print_ansi(this_c, prefix + postfix_m + done_m + name_m);
}
#else
void queue_print_t::done(bool /**/, string_t /**/) const {}
#endif

#if WEBVIEW_LOG_TRACE
void queue_eval_t::wrapper_t::start(string_t js) const {
  auto this_c = ansi.blue;
  auto postfix_m = bold(this_c, postfix) + ": ";
  auto start_m = bold(this_c, "START") + " js ...\n";
  auto js_m = dim(this_c, js);
  print_ansi(this_c, prefix + postfix_m + start_m + js_m);
};
#else
void queue_eval_t::wrapper_t::start(string_t /**/) const {}
#endif
#if WEBVIEW_LOG_TRACE
void queue_eval_t::wrapper_t::done(bool done) const {
  auto this_c = ansi.blue;
  auto postfix_m = bold(this_c, postfix) + ": ";
  auto done_m = bold(this_c, "DONE: ") + bool_s(done);
  print_ansi(this_c, prefix + postfix_m + done_m);
}
#else
void queue_eval_t::wrapper_t::done(bool /**/) const {}
#endif
#if WEBVIEW_LOG_TRACE
void queue_loop_t::wrapper_t::wait(size_t size, bool empty,
                                   bool dom_ready) const {
  set_loop_wait_ts();
  auto this_col = ansi.green;
  auto waiting = bold(this_col, "WAITING ");
  auto size_s = "| queue size: " + std::to_string(size) + " ";
  auto empty_s = "| queue_empty: " + bool_s(empty) + " ";
  auto dom_s = "| dom_ready: " + bool_s(dom_ready) + " ";
  auto time_m = "| total process time: " + total_elapsed() + " ";

  print_ansi(this_col,
             prefix + postfix + waiting + time_m + size_s + empty_s + dom_s);
};
#else
void queue_loop_t::wrapper_t::wait(size_t /**/, bool /**/, bool /**/) const {}
#endif

#if WEBVIEW_LOG_TRACE
void queue_loop_t::wrapper_t::start(size_t size) const {
  set_loop_start_ts();
  auto this_col = ansi.green;
  auto start = bold(this_col, "START ");
  auto time_m = "| waited for: " + wait_elapsed() + " ";
  auto size_m = "| queue size: " + std::to_string(size);

  print_ansi(this_col, prefix + postfix + start + time_m + size_m + "\n");
};
#else
void queue_loop_t::wrapper_t::start(size_t /**/) const {}
#endif
void queue_loop_t::wrapper_t::end() const {
#if WEBVIEW_LOG_TRACE
  set_loop_end_ts();
  auto this_col = ansi.green;
  auto end = bold(this_col, "END ");
  auto time_m = "| loop process time: " + loop_elapsed();
  print_ansi(this_col, "\n" + prefix + postfix + end + time_m);
#endif
}
void queue_loop_t::wrapper_t::set_loop_wait_ts() const {
#if WEBVIEW_LOG_TRACE
  loop_wait_ts = get_now();
#endif
}
void queue_loop_t::wrapper_t::set_loop_start_ts() const {
#if WEBVIEW_LOG_TRACE
  loop_start_ts = get_now();
#endif
}
void queue_loop_t::wrapper_t::set_loop_end_ts() const {
#if WEBVIEW_LOG_TRACE
  loop_end_ts = get_now();
#endif
}
std::string queue_loop_t::wrapper_t::total_elapsed() const {
#if WEBVIEW_LOG_TRACE
  auto elapsed = elapsed_ms(process_ts, loop_wait_ts);
  return std::to_string(elapsed) + "ms";
#else
  return "";
#endif
}
std::string queue_loop_t::wrapper_t::wait_elapsed() const {
#if WEBVIEW_LOG_TRACE
  auto elapsed = elapsed_ms(loop_wait_ts, loop_start_ts);
  return std::to_string(elapsed) + "ms";
#else
  return "";
#endif
}
std::string queue_loop_t::wrapper_t::loop_elapsed() const {
#if WEBVIEW_LOG_TRACE
  auto elapsed = elapsed_ms(loop_start_ts, loop_end_ts);
  return std::to_string(elapsed) + "ms";
#else
  return "";
#endif
}

#if WEBVIEW_LOG_TRACE
void queue_notify_t::wrapper_t::on_message(string_t method) const {
  auto this_c = ansi.yellow_dim;
  auto mess_m = bold(this_c, "on_message") + ": ";
  auto method_m = escape_s(method);
  print_ansi(this_c, prefix + postfix + mess_m + method_m);
}
#else
void queue_notify_t::wrapper_t::on_message(string_t /**/) const {}
#endif

#if WEBVIEW_LOG_TRACE
void queue_enqueue_t::wrapper_t::added(char scp, size_t size,
                                       string_t name_or_js) const {
  auto this_c = ansi.default_c;
  auto ctx_m = get_ctx(scp);
  auto size_m = "queue size: " + num_s(size) + " | ";
  auto queued_m = bold(this_c, "queueing " + ctx_m) + " | ";
  auto name_or_js_m = scp == 'e' ? "got js ...\n" + dim(this_c, name_or_js)
                                 : escape_s(name_or_js);
  print_ansi(this_c, prefix + postfix + queued_m + size_m + name_or_js_m);
}
#else
void queue_enqueue_t::wrapper_t::added(char /**/, size_t /**/,
                                       string_t /**/) const {}
#endif
#if WEBVIEW_LOG_TRACE
void queue_enqueue_t::wrapper_t::added(char scp, size_t size) const {
  auto this_c = ansi.default_c;
  auto ctx_m = get_ctx(scp);
  auto size_m = "queue size: " + num_s(size) + " | ";
  auto queued_m = bold(this_c, "queueing " + ctx_m);
  print_ansi(this_c, prefix + postfix + size_m + queued_m);
}
#else
void queue_enqueue_t::wrapper_t::added(char /**/, size_t /**/) const {}
#endif

#if WEBVIEW_LOG_TRACE
void base_print_t::start(string_t name) const {
  auto this_c = ansi.default_c;
  auto name_m = escape_s(name);
  auto got_m = bold(this_c, "got") + ": ";
  print_ansi(this_c, prefix + postfix + got_m + name_m);
}
#else
void base_print_t::start(string_t /**/) const {}
#endif
#if WEBVIEW_LOG_TRACE
void base_print_t::work(string_t name) const {
  auto this_c = ansi.blue_dark;
  auto work_m = bold(this_c, "do_work: ");
  auto name_m = escape_s(name);
  print_ansi(this_c, prefix + postfix + work_m + name_m);
}
#else
void base_print_t::work(string_t /**/) const {}
#endif
#if WEBVIEW_LOG_TRACE
void base_print_t::done(string_t name) const {
  auto this_c = ansi.blue;
  auto done_m = bold(this_c, "work done") + ": ";
  auto name_m = escape_s(name);
  print_ansi(this_c, prefix + postfix + done_m + name_m);
}
#else
void base_print_t::done(string_t /**/) const {}
#endif

#if WEBVIEW_LOG_TRACE
void base_eval_t::wrapper_t::start(string_t js, bool skip_queue) const {
  auto this_c = ansi.default_c;
  auto skip_m = "skip queue: " + bool_s(skip_queue) + " | ";
  auto received_m = bold(this_c, "received js") + " ...\n";
  auto m = prefix + postfix + skip_m + received_m + dim(this_c, js);
  print_ansi(this_c, m);
};
#else
void base_eval_t::wrapper_t::start(string_t /**/, bool /**/) const {}
#endif
#if WEBVIEW_LOG_TRACE
void base_eval_t::wrapper_t::work(string_t js) const {
  auto this_c = ansi.blue_dark;
  auto work_m = bold(this_c, "do_work") + " js...\n";
  auto m = prefix + postfix + work_m + dim(this_c, js);
  print_ansi(this_c, m);
};
#else
void base_eval_t::wrapper_t::work(string_t /**/) const {}
#endif
#if WEBVIEW_LOG_TRACE
void base_eval_t::wrapper_t::done(bool done, string_t js) const {
  auto this_c = ansi.blue;
  auto done_m = bold(this_c, "work done") + ": " + bool_s(done) + " | js ...\n";
  auto js_m = dim(this_c, js);
  print_ansi(this_c, prefix + postfix + done_m + js_m);
}
#else
void base_eval_t::wrapper_t::done(bool /**/, string_t /**/) const {}
#endif

} // namespace _structs

const _structs::base_t &trace::base = get_base();
const _structs::queue_t &trace::queue = get_queue();

} // namespace log
} // namespace webview
#endif // defined(__cplusplus) && !defined(WEBVIEW_HEADER)
#endif // WEBVIEW_LOG_TRACE_LOG_CC
// file end: src/log/trace_log.cc
// file begin: include/webview/types/types.hh
#ifndef WEBVIEW_TYPES_HH
#define WEBVIEW_TYPES_HH

#if defined(__cplusplus) && !defined(WEBVIEW_HEADER)
#include <functional>

namespace webview {

enum context_t { bind_t = 'b', unbind_t = 'u', eval_t = 'e' };
struct action_ctx_t {
  context_t bind = context_t::bind_t;
  context_t unbind = context_t::unbind_t;
  context_t eval = context_t::eval_t;
};

template <typename T> struct nested_api_t {
  T *self;
  nested_api_t(T *self) : self(self) {}
};

using str_arg_t = const std::string &;

using do_work_t = std::function<void()>;

using dispatch_fn_t = std::function<void()>;

template <typename T>
using result = detail::basic_result<T, error_info, exception>;

using noresult = detail::basic_result<void, error_info, exception>;

} // namespace webview

#endif // defined(__cplusplus) && !defined(WEBVIEW_HEADER)
#endif // WEBVIEW_TYPES_HH
// file end: include/webview/types/types.hh
// file begin: src/log/_src_log.hh
#ifndef WEBVIEW_AMALGAMATED_SRC_LOG_HH
#define WEBVIEW_AMALGAMATED_SRC_LOG_HH

#if defined(__cplusplus) && !defined(WEBVIEW_HEADER)
#endif
#endif
// file end: src/log/_src_log.hh
// file begin: include/webview/detail/frontend/frontend_strings.hh
#ifndef WEBVIEW_UTIL_FRONTEND_STRINGS_HH
#define WEBVIEW_UTIL_FRONTEND_STRINGS_HH

#if defined(__cplusplus) && !defined(WEBVIEW_HEADER)
#define SYSTEM_NOTIFICATION_FLAG "_sysop_"
#define TEST_NOTIFICATION_FLAG "_tests_"
#define TOKEN_NAME "_name_"
#define TOKEN_ID "_id_"
#define TOKEN_STATUS "_status_"
#define TOKEN_RESULT "_result_"
#define TOKEN_POST_FN "_post_fn_"
#define TOKEN_JS_NAMES "_js_names_"
#define TOKEN_USER_JS "_user_js_"
#define TOKEN_WHAT "_what_"
#define TOKEN_VALUE "_value_"
#include <initializer_list>
#include <string>
#include <vector>

namespace webview {
namespace detail {

namespace _structs {
struct sys_flags_t {
  sys_flags_t() noexcept = default;
  std::string sysop = SYSTEM_NOTIFICATION_FLAG;
  std::string testop = TEST_NOTIFICATION_FLAG;
};

struct tokens_t {
  tokens_t() noexcept = default;
  std::string name = TOKEN_NAME;
  std::string id = TOKEN_ID;
  std::string status = TOKEN_STATUS;
  std::string result = TOKEN_RESULT;
  std::string post_fn = TOKEN_POST_FN;
  std::string js_names = TOKEN_JS_NAMES;
  std::string user_js = TOKEN_USER_JS;
  std::string what = TOKEN_WHAT;
  std::string value = TOKEN_VALUE;
};

struct tokenise_data_t {
  std::string token;
  std::string tkn_replcmnt;
};
} // namespace _structs

namespace str {

_structs::sys_flags_t const sys_flag{};
_structs::tokens_t const token{};

/// Performs string replacement for tokens.
/// @todo REGEX is probably going to be optimal for performance
std::string tokenise(str_arg_t tmplate, str_arg_t token,
                     str_arg_t tkn_replcmnt) {
  if (token.empty()) {
    return tmplate;
  }
  std::string tokenised_string = tmplate;
  size_t start_pos = 0;
  while ((start_pos = tokenised_string.find(token, start_pos)) !=
         std::string::npos) {
    tokenised_string.replace(start_pos, token.length(), tkn_replcmnt);
    start_pos += tkn_replcmnt.length();
  }

  return tokenised_string;
}
std::string
tokenise(std::string tmplate,
         std::initializer_list<_structs::tokenise_data_t> replacements) {
  for (auto &replacement : replacements) {
    tmplate = tokenise(tmplate, replacement.token, replacement.tkn_replcmnt);
  };
  return tmplate;
}

/// Parses a vector into a JSON array string.
std::string json_list(std::vector<std::string> &binding_names) {
  std::string json = "[";
  bool first = true;
  for (const auto &name : binding_names) {
    if (first) {
      first = false;
    } else {
      json += ",";
    }
    json += webview::detail::json_escape(name);
  }
  json += "]";

  return json;
}

} // namespace str
} // namespace detail
} // namespace webview

#endif // defined(__cplusplus) && !defined(WEBVIEW_HEADER)
#endif // WEBVIEW_UTIL_FRONTEND_STRINGS_HH
// file end: include/webview/detail/frontend/frontend_strings.hh
// file begin: include/webview/detail/threading/atomic_api.hh
#ifndef WEBVIEW_DETAIL_ATOMIC_API_HH
#define WEBVIEW_DETAIL_ATOMIC_API_HH

#if defined(__cplusplus) && !defined(WEBVIEW_HEADER)
namespace webview {
namespace detail {

class engine_queue;

namespace _struct {

struct atomic_dom_ready_t : nested_api_t<engine_queue> {
  atomic_dom_ready_t(engine_queue *self) : nested_api_t(self) {}
  /// Query if the backend is ready to do work.
  bool ready() const;
  /// Notify the queue that the backend is ready to receive work.
  void ready(bool flag);
};

struct atomic_queue_t : nested_api_t<engine_queue> {
  atomic_queue_t(engine_queue *self) : nested_api_t(self) {}
  /// Decrements the queue list and flags empty state.
  void update();
  /// Query if the queue is empty.
  bool empty() const;
  /// Set the queue_empty flag.
  void empty(bool val);
};

struct atomic_done_t : nested_api_t<engine_queue> {
  atomic_done_t(engine_queue *self) : nested_api_t(self) {}
  /// Gets the bind flag state
  bool bind() const;
  /// Sets the bind flag state
  void bind(bool val);
  /// Gets the unbind flag state
  bool unbind() const;
  /// Sets the unbind flag state
  void unbind(bool val);
  /// Gets the eval flag state
  bool eval() const;
  /// Sets the eval flag state
  void eval(bool val);
};

struct atomic_api_t : nested_api_t<engine_queue> {
  ~atomic_api_t() = default;
  atomic_api_t(engine_queue *self) : nested_api_t(self) {}

  atomic_dom_ready_t dom{this->self};
  atomic_queue_t queue{this->self};
  atomic_done_t done{this->self};
  bool terminating() const;
  bool AND(std::initializer_list<bool> flags) const;
};

} // namespace _struct
} // namespace detail
} // namespace webview

#endif // defined(__cplusplus) && !defined(WEBVIEW_HEADER)
#endif // WEBVIEW_DETAIL_ATOMIC_API_HH
// file end: include/webview/detail/threading/atomic_api.hh
// file begin: include/webview/detail/threading/threadsafe_lists.hh
#ifndef WEBVIEW_DETAIL_THREADSAFE_LISTS_HH
#define WEBVIEW_DETAIL_THREADSAFE_LISTS_HH

#if defined(__cplusplus) && !defined(WEBVIEW_HEADER)
#include <condition_variable>
#include <deque>
#include <list>
#include <map>
#include <mutex>
#include <string>
#include <unordered_map>
#include <vector>

namespace webview {
namespace detail {
class engine_base;

namespace _structs {

struct action_t {
  context_t ctx;
  do_work_t work_fn;
  std::string name_or_js;
};
struct indices_t {
  int bind_i;
  int unbind_i;
};

struct cv_api_t {
  std::condition_variable queue;
  std::condition_variable bind;
  std::condition_variable eval;
  std::condition_variable unbind;
  std::condition_variable unbind_timeout;
  std::condition_variable *all[5] = {&queue, &bind, &eval, &unbind,
                                     &unbind_timeout};
  void notify_all();
};

struct bindings_t {
public:
  size_t size() const;
  void get_names(std::vector<std::string> &bound_names);
  bool has_name(str_arg_t name) const;
  void emplace(str_arg_t name, binding_t fn, void *arg);
  void erase(str_arg_t name);
  size_t count(str_arg_t name) const;
  binding_ctx_t at(str_arg_t name) const;

private:
  std::map<std::string, binding_ctx_t> bindings_map;
  mutable std::mutex mtx;
};

struct user_scripts_t {
public:
  /// Adds a bound user function to Webview native code.
  user_script *add(str_arg_t js, engine_base *base);
  /// Replaces a bound user script in Webview native code.
  user_script *replace(const user_script &old_script, str_arg_t new_script_code,
                       engine_base *base);

private:
  /// A list of references to bound user scripts.
  std::list<user_script> m_user_scripts;
  mutable std::mutex mtx;
};

struct queue_t {
public:
  size_t size() const;
  action_t front() const;
  void pop_front();
  void push_back(context_t ctx, do_work_t work_fn, std::string name_or_js);
  void clear();
  bool empty() const;

private:
  std::deque<action_t> queue;
  mutable std::mutex mtx;
};

struct unres_promises_t {
public:
  void set(str_arg_t name, std::list<std::string> ids);
  std::list<std::string> get_copy(str_arg_t name) const;
  void remove_id(str_arg_t name, str_arg_t id);
  void add_id(str_arg_t name, str_arg_t id);
  void erase(str_arg_t name);
  bool empty(str_arg_t name) const;

private:
  std::unordered_map<std::string, std::list<std::string>> unres_promises;
  mutable std::mutex mtx;
};

struct id_name_map_t {
public:
  std::string get(str_arg_t id) const;
  void set(str_arg_t id, str_arg_t name);
  void erase(str_arg_t id);

private:
  std::unordered_map<std::string, std::string> id_name;
  mutable std::mutex mtx;
};

struct pending_t {
public:
  void pop_front();
  void push_back(str_arg_t name);
  indices_t indices(str_arg_t name) const;

private:
  std::deque<std::string> pending_bind_unbind;
  mutable std::mutex mtx;
};
} // namespace _structs

class engine_lists_t {
  struct list_t {
    /// Thread safe wrappers for the `std::map` of bindings.
    _structs::bindings_t bindings{};
    /// Thread safe wrappers for the `std::list` of user scripts.
    _structs::user_scripts_t m_user_scripts{};
    /// Thread safe wrappers for the `std::deque` of user actions.
    _structs::queue_t queue{};
    /// Thread safe wrappers for the `std::unordered_map` of unresolved
    /// promises.
    _structs::unres_promises_t unresolved_promises{};
    /// Thread safe wrappers for the `std::unordered_map` of promise ids to
    /// bound names.
    _structs::id_name_map_t id_name_map{};
    /// Thread safe wrappers for the `std::deque` of pending bind / unbind names
    _structs::pending_t pending{};
  };

protected:
  /// Grouping of condition variables
  _structs::cv_api_t cv{};
  /// Thread safe wrappers for list-like objects.
  list_t list{};
};

} // namespace detail
} // namespace webview

#endif // defined(__cplusplus) && !defined(WEBVIEW_HEADER)
#endif // WEBVIEW_DETAIL_THREADSAFE_LISTS_HH
// file end: include/webview/detail/threading/threadsafe_lists.hh
// file begin: include/webview/detail/frontend/engine_frontend.hh
#ifndef WEBVIEW_FRONTEND_ENGINE_FRONTEND_HH
#define WEBVIEW_FRONTEND_ENGINE_FRONTEND_HH

#if defined(__cplusplus) && !defined(WEBVIEW_HEADER)
#include <string>
#include <vector>

namespace webview {
namespace detail {

namespace _strings_sysop {
#define FRONTEND_DOM_READY "_dom_ready"
#define FRONTEND_BIND_DONE "_bind_done"
#define FRONTEND_UNBIND_DONE "_unbind_done"
#define FRONTEND_EVAL_READY "_frontend_eval_ready"
} // namespace _strings_sysop

namespace _strings_init_js {
#define WEVBIEW_INIT_JS                                                        \
  "(function() {\n"                                                            \
  "  'use strict';\n"                                                          \
  "  function generateId() {\n"                                                \
  "    var crypto = window.crypto || window.msCrypto;\n"                       \
  "    var bytes = new Uint8Array(16);\n"                                      \
  "    crypto.getRandomValues(bytes);\n"                                       \
  "    return Array.prototype.slice.call(bytes).map(function(n) {\n"           \
  "      var s = n.toString(16);\n"                                            \
  "      return ((s.length % 2) == 1 ? '0' : '') + s;\n"                       \
  "    }).join('');\n"                                                         \
  "  }\n"                                                                      \
  "  var Webview = (function() {\n"                                            \
  "    var _promises = {};\n"                                                  \
  "    function Webview_() {}\n"                                               \
  "    Webview_.prototype.post = function(message) {\n"                        \
  "      return (" TOKEN_POST_FN ")(message);\n"                               \
  "    };\n"                                                                   \
  "    Webview_.prototype.sysop = function(command) {\n"                       \
  "      this.post(JSON.stringify({\n"                                         \
  "        id: \"" SYSTEM_NOTIFICATION_FLAG "\",\n"                            \
  "        method: command,\n"                                                 \
  "        params: []\n"                                                       \
  "      }));\n"                                                               \
  "    };\n"                                                                   \
  "    Webview_.prototype.call = function(method) {\n"                         \
  "      var _id = generateId();\n"                                            \
  "      var _params = Array.prototype.slice.call(arguments, 1);\n "           \
  "      var promise = new Promise(function(resolve, reject) {\n "             \
  "        _promises[_id] = { resolve, reject };\n"                            \
  "      });\n"                                                                \
  "      this.post(JSON.stringify({\n"                                         \
  "        id: _id,\n"                                                         \
  "        method: method,\n"                                                  \
  "        params: _params\n"                                                  \
  "      }));\n"                                                               \
  "      return promise;\n"                                                    \
  "    };\n"                                                                   \
  "    Webview_.prototype.onReply = function(id, status, result) {\n"          \
  "      var promise = _promises[id];\n"                                       \
  "      if (result !== undefined) {\n"                                        \
  "        try {\n"                                                            \
  "          result = JSON.parse(result);\n"                                   \
  "        } catch (e) {\n"                                                    \
  "          promise.reject(new Error(\"Failed to parse binding result as "    \
  "JSON\"));\n"                                                                \
  "          return;\n"                                                        \
  "        }\n"                                                                \
  "      }\n"                                                                  \
  "      if (status === 0) {\n"                                                \
  "        promise.resolve(result);\n"                                         \
  "      } else {\n"                                                           \
  "        promise.reject(result);\n"                                          \
  "      }\n"                                                                  \
  "    };\n"                                                                   \
  "    Webview_.prototype.onBind = function(name) {\n"                         \
  "      if (window.hasOwnProperty(name)) {\n"                                 \
  "        throw new Error('Property \"' + name + '\" already exists');\n"     \
  "      }\n"                                                                  \
  "      window[name] = (function() {\n"                                       \
  "        var params = "                                                      \
  "[name].concat(Array.prototype.slice.call(arguments));\n"                    \
  "        return Webview_.prototype.call.apply(this, params);\n"              \
  "      }).bind(this);\n"                                                     \
  "    };\n"                                                                   \
  "    Webview_.prototype.onUnbind = function(name) {\n"                       \
  "      if (!window.hasOwnProperty(name)) {\n"                                \
  "        throw new Error('Property \"' + name + '\" does not exist');\n"     \
  "      }\n"                                                                  \
  "      delete window[name];\n"                                               \
  "    };\n"                                                                   \
  "    return Webview_;\n"                                                     \
  "  })();\n"                                                                  \
  "  window.__webview__ = new Webview();\n"                                    \
  "  window.__webview__.sysop(\"" FRONTEND_DOM_READY "\");\n"                  \
  "})()"
} // namespace _strings_init_js

namespace _strings_js_functions {
#define ON_BIND                                                                \
  "if (window.__webview__) {\n"                                                \
  "  try {\n"                                                                  \
  "    window.__webview__.onBind(" TOKEN_NAME ");\n"                           \
  "    window.__webview__.sysop(\"" FRONTEND_BIND_DONE "\");\n"                \
  "  } catch(err) {\n"                                                         \
  "    window.__webview__.sysop(\"" FRONTEND_BIND_DONE "\");\n"                \
  "  }\n"                                                                      \
  "}"
#define ON_UNBIND                                                              \
  "if (window.__webview__) {\n"                                                \
  "  try {\n"                                                                  \
  "    window.__webview__.onUnbind(" TOKEN_NAME ");\n"                         \
  "    window.__webview__.sysop(\"" FRONTEND_UNBIND_DONE "\");\n"              \
  "  } catch(err) {\n"                                                         \
  "    window.__webview__.sysop(\"" FRONTEND_UNBIND_DONE "\");\n"              \
  "  }\n"                                                                      \
  "}"
#define ON_REPLY                                                               \
  "window.__webview__.onReply(" TOKEN_ID ", " TOKEN_STATUS ", " TOKEN_RESULT ")"

#define BIND                                                                   \
  "(function() {\n"                                                            \
  " 'use strict';\n"                                                           \
  " var methods = " TOKEN_JS_NAMES ";\n"                                       \
  " methods.forEach(function(name) {\n"                                        \
  "   window.__webview__.onBind(name);\n"                                      \
  " });\n"                                                                     \
  "})()"

#define EVAL_WRAPPER                                                           \
  "try {\n"                                                                    \
  "  setTimeout(() => {\n"                                                     \
  "    window.__webview__.sysop(\"" FRONTEND_EVAL_READY "\");\n"               \
  "  });\n" TOKEN_USER_JS "\n"                                                 \
  "} catch (err) {\n"                                                          \
  "  window.__webview__.sysop(\"" FRONTEND_EVAL_READY "\");\n"                 \
  "  console.error(err);\n"                                                    \
  "}"
} // namespace _strings_js_functions

namespace _strings_error_messages {
#define REJECT_UNBOUND_M                                                       \
  "Promise id " TOKEN_ID " was rejected because function \"" TOKEN_NAME        \
  "\" was unbound."

#define UNCAUGHT_EXP_M                                                         \
  "Uncaught exception from native user callback function \"" TOKEN_NAME        \
  "\":\n" TOKEN_WHAT

#define WEBVIEW_TERMINATED_M                                                   \
  "\nNative user callback function \"" TOKEN_NAME                              \
  "\" failed because Webview terminated before it could complete.\n\n"
} // namespace _strings_error_messages

namespace _structs {
struct templates_t {
  templates_t() noexcept = default;
  std::string init = WEVBIEW_INIT_JS;
  std::string onbind = ON_BIND;
  std::string onunbind = ON_UNBIND;
  std::string onreply = ON_REPLY;
  std::string bind = BIND;
  std::string eval_wrapper = EVAL_WRAPPER;
};

struct error_messages_t {
  error_messages_t() noexcept = default;
  std::string reject_unbound = REJECT_UNBOUND_M;
  std::string uncaught_exp = UNCAUGHT_EXP_M;
  std::string terminated = WEBVIEW_TERMINATED_M;
};

struct sysops_t {
  sysops_t() noexcept = default;
  std::string dom_ready = FRONTEND_DOM_READY;
  std::string bind_done = FRONTEND_BIND_DONE;
  std::string unbind_done = FRONTEND_UNBIND_DONE;
  std::string js_eval_start = FRONTEND_EVAL_READY;
};
} // namespace _structs

namespace str {
_structs::templates_t const js{};
_structs::error_messages_t const error{};
} // namespace str

namespace _structs {
struct front_end_t {

  struct js_string_t {
    /// Returns a tokenised JS function string for `unbind` which notifies that
    /// a binding was destroyed after the init script has already set things up.
    std::string onunbind(str_arg_t name) const {
      return str::tokenise(str::js.onunbind, str::token.name,
                           json_escape(name));
    }

    /// Returns a tokenised JS function string for `bind` which notifies that
    /// a binding was created after the init script has already set things up.
    std::string onbind(str_arg_t name) const {
      return str::tokenise(str::js.onbind, str::token.name, json_escape(name));
    }

    /// Returns a tokenised JS function string for a promise resolve/reject.
    std::string onreply(std::string id, int status,
                        std::string escaped_result) const {
      return str::tokenise(str::js.onreply,
                           {{str::token.id, json_escape(id)},
                            {str::token.status, std::to_string(status)},
                            {str::token.result, escaped_result}});
    }

    /// Returns a tokenised JS string for the Webview frontend init function.
    std::string init(str_arg_t post_fn) const {
      return str::tokenise(str::js.init, str::token.post_fn, post_fn);
    }

    /// Returns a tokenised JS string for the Webview frontend `bind` functions.
    std::string bind(std::vector<std::string> &bound_names) const {
      auto js_names = str::json_list(bound_names);
      return str::tokenise(str::js.bind, str::token.js_names, js_names);
    }

    /// Wraps user JS to notify the native code when eval is ready.
    std::string eval_wrapper(std::string user_js) const {
      return str::tokenise(str::js.eval_wrapper, str::token.user_js, user_js);
    }
  } js{};

  struct error_message_t {
    /// Returns a tokenised error string for rejecting a promise if a callback
    /// binding was unbound.
    std::string reject_unbound(std::string id, std::string name) const {
      return str::tokenise(str::error.reject_unbound,
                           {{str::token.id, id}, {str::token.name, name}});
    }

    /// Returns a tokenised error string for rejecting a promise if a native
    /// callback has an uncaught exception.
    std::string uncaught_exception(std::string name, std::string what) const {
      return str::tokenise(str::error.uncaught_exp,
                           {{str::token.name, name}, {str::token.what, what}});
    }

    /// Returns a tokenised error string for native callbacks in detached
    /// threads after webview terminates.
    std::string webview_terminated(std::string name) const {
      return str::tokenise(str::error.terminated, str::token.name, name);
    }

  } err_message{};
  /// Frontend to backend system operation signals.
  _structs::sysops_t sysops;
};
} // namespace _structs

/// API for HTML and JS related strings.
static const _structs::front_end_t frontend{};

} // namespace detail
} // namespace webview

#endif // defined(__cplusplus) && !defined(WEBVIEW_HEADER)
#endif // WEBVIEW_FRONTEND_ENGINE_FRONTEND_HH
// file end: include/webview/detail/frontend/engine_frontend.hh
// file begin: include/webview/detail/engine_queue.hh
#ifndef WEBVIEW_ENGINE_QUEUE_HH
#define WEBVIEW_ENGINE_QUEUE_HH

#if defined(__cplusplus) && !defined(WEBVIEW_HEADER)
#ifndef WEBVIEW_UNBIND_TIMEOUT
#define WEBVIEW_UNBIND_TIMEOUT 20
#endif
#include <atomic>
#include <thread>

namespace webview {
namespace detail {

class engine_queue : protected engine_lists_t {
public:
  ~engine_queue() = default;
  engine_queue();

  /* ************************************************************************
   * API for `engine_queue`.
   *
   * Note:
   * This is a constrained public interface to private internal functionality,
   * which is acceptable and idiomatic in C++ for API design.
   **************************************************************************/

  struct bind_api_t : nested_api_t<engine_queue> {
    ~bind_api_t() = default;
    bind_api_t(engine_queue *self) : nested_api_t(self) {}
    /// Puts a user `bind` work unit onto the queue.
    noresult enqueue(do_work_t fn, str_arg_t name) const;
    /// Indicates if adding a `bind` to the queue is an error, eg. duplicate
    /// name.
    bool is_duplicate(str_arg_t name) const;
  };
  struct unbind_api_t : nested_api_t<engine_queue> {
    ~unbind_api_t() = default;
    unbind_api_t(engine_queue *self) : nested_api_t(self) {}
    /// Puts a user `unbind` work unit onto the queue.
    noresult enqueue(do_work_t fn, str_arg_t name) const;
    /// Indicates if adding an `unbind` to the queue is an error, eg. bind
    /// doesn't exist.
    bool not_found(str_arg_t name) const;
  };
  struct eval_api_t : nested_api_t<engine_queue> {
    ~eval_api_t() = default;
    eval_api_t(engine_queue *self) : nested_api_t(self) {}
    /// Puts a user `eval` work unit onto the queue.
    noresult enqueue(do_work_t fn, str_arg_t js) const;
  };
  struct promise_api_t : nested_api_t<engine_queue> {
    ~promise_api_t() = default;
    promise_api_t(engine_queue *self) : nested_api_t(self) {}
    /// Initialises an empty list for promise id's of a given bind name
    // void list_init(str_arg_t name) const;
    /// Takes queue action for a resolved promise
    void resolved(str_arg_t id) const;
    /// Sends the native work unit of a promise to a concurrent thread.
    void resolve(str_arg_t name, str_arg_t id, str_arg_t args,
                 engine_base *wv) const;
    /// Relays notifications from the frontend to relevant queue methods.
    bool is_system_message(str_arg_t id, str_arg_t method);
  };
  struct bindings_api_t : nested_api_t<engine_queue> {
    bindings_api_t(engine_queue *self) : nested_api_t(self) {}
    /// Sets the bindings map locked state
    void locked(bool val) const;
  };
  struct public_api_t : nested_api_t<engine_queue> {
    ~public_api_t() = default;
    public_api_t(engine_queue *self) : nested_api_t{self} {};
    bind_api_t bind{this->self};
    unbind_api_t unbind{this->self};
    eval_api_t eval{this->self};
    promise_api_t promises{this->self};
    bindings_api_t bindings{this->self};

    /// Initialises the user work unit queue thread;
    void init(engine_base *wv) const;
    /// Releases and shuts down the queue thread.
    void shutdown() const;
    bool shutting_down() const;
  };

  // NOLINTBEGIN(cppcoreguidelines-non-private-member-variables-in-classes)

  /// Public API for the engine_queue class instance.
  public_api_t queue;

  // NOLINTEND(cppcoreguidelines-non-private-member-variables-in-classes)

private:
  friend struct _struct::atomic_dom_ready_t;
  friend struct _struct::atomic_queue_t;
  friend struct _struct::atomic_done_t;
  friend struct _struct::atomic_api_t;

  /// Container for user work operation tags, ie. `bind`, `unbind`, `eval`
  action_ctx_t const ctx{};

  /// @brief Constructs a thread to choreograph execution of `bind`, `unbind` or
  /// `eval` user work units.
  ///
  /// We do not want user work to stall the main / app thread.
  /// We need user work units to execute sequentially and atomically:
  /// - work must automatically queue until the DOM `__webview__` method is
  /// ready,
  /// - `unbind` must only execute after preceding `bind` work is completely
  /// registered,
  /// - JS promises must not be left indefinitely in a waiting state,
  /// - `eval` must wait for potentially included bound JS promises to register.
  /// - `unbind` must wait for resolution of relevant JS promises before
  /// rejecting them, and for this we set a sane timeout,
  void queue_thread_constructor(engine_base *wv_instance);

  /// @brief Constructs a child thread for each native work unit of a bound JS
  /// promise.
  ///
  /// We want native promise work units to run concurrently.
  /// We do not want native promise work to stall execution of the main / app
  /// thread.
  /// @todo hardware concurrency limit queue.
  void resolve_thread_constructor(str_arg_t name, str_arg_t id, str_arg_t args,
                                  engine_base *wv);

  /// Adds `bind`, `unbind` or `eval` user work unit to the queue.
  noresult queue_work(str_arg_t name_or_js, do_work_t fn, context_t fn_ctx);

  /// Sends a native promise work unit to a concurrent detached thread.
  void resolve_work(engine_base *wv, str_arg_t msg, str_arg_t id);

  /// API to query and set various flags atomically
  _struct::atomic_api_t atomic;

  std::atomic_bool is_dom_ready{};
  std::atomic_bool queue_empty{};
  std::atomic_size_t queue_size{};
  std::atomic_bool unbind_done{};
  std::atomic_bool unbind_can_proceed{};
  std::atomic_bool bind_done{};
  std::atomic_bool eval_done{};
  std::atomic_bool is_terminating{};

  /// A thread to concurrently choreograph user work queueing.
  std::thread queue_thread;
};

} // namespace detail
} // namespace webview
#endif // defined(__cplusplus) && !defined(WEBVIEW_HEADER)
#endif // WEBVIEW_ENGINE_QUEUE_HH
// file end: include/webview/detail/engine_queue.hh
// file begin: include/webview/detail/engine_base.hh
#ifndef WEBVIEW_DETAIL_ENGINE_BASE_HH
#define WEBVIEW_DETAIL_ENGINE_BASE_HH

#if defined(__cplusplus) && !defined(WEBVIEW_HEADER)
#include <atomic>
#include <list>

namespace webview {
namespace detail {

/// Internally used callback function type for messaging in the promise
/// resolution native / JS round trip
using sync_binding_t = std::function<std::string(std::string)>;

/// Common internal API methods for all three Webview platform classes:
/// - cocoa_webkit
/// - gtk_webkitgtk
/// - win32_edge
class engine_base : public engine_queue {

public:
  virtual ~engine_base() = default;
  engine_base(bool owns_window);

  /// Internal API implementation of public \ref webview_navigate
  noresult navigate(str_arg_t url);
  /// Internal API implementation of public \ref webview_bind (synchronous)
  noresult bind(str_arg_t name, sync_binding_t fn);
  /// Internal API implementation of public \ref webview_bind (asynchronous)
  noresult bind(str_arg_t name, binding_t fn, void *arg);
  /// Internal API implementation of public \ref webview_unbind
  noresult unbind(str_arg_t name);
  /// Internal API implementation of public \ref webview_return
  noresult resolve(str_arg_t id, int status, str_arg_t result);
  /// Helper to reject a promise through \ref resolve
  noresult reject(str_arg_t id, str_arg_t err);
  /// Internal API implementation of public \ref webview_get_window
  result<void *> window();
  /// Internal API implementation part of public \ref webview_get_native_handle
  result<void *> widget();
  /// Internal API implementation part of public \ref webview_get_native_handle
  result<void *> browser_controller();
  /// Internal API implementation of public \ref webview_run
  noresult run();
  /// Internal API implementation of public \ref webview_terminate
  noresult terminate();
  /// Internal API implementation of public \ref webview_dispatch
  noresult dispatch(std::function<void()> f);
  /// Internal API implementation of public \ref webview_set_title
  noresult set_title(str_arg_t title);
  /// Internal API implementation of public \ref webview_set_size
  noresult set_size(int width, int height, webview_hint_t hints);
  /// Internal API implementation of public \ref webview_set_html
  noresult set_html(str_arg_t html);
  /// Internal API implementation of public \ref webview_init
  noresult init(str_arg_t js);
  /// Internal API implementation of public \ref webview_eval
  noresult eval(str_arg_t js);

protected:
  friend struct _structs::user_scripts_t;
  /// Platform specific implementation for \ref navigate
  virtual noresult navigate_impl(str_arg_t url) = 0;
  /// Platform specific implementation for \ref window
  virtual result<void *> window_impl() = 0;
  /// Platform specific implementation for \ref widget
  virtual result<void *> widget_impl() = 0;
  /// Platform specific implementation for \ref browser_controller
  virtual result<void *> browser_controller_impl() = 0;
  /// Platform specific implementation for \ref run
  virtual noresult run_impl() = 0;
  /// Platform specific implementation for \ref terminate
  virtual noresult terminate_impl() = 0;
  /// Platform specific implementation for \ref dispatch
  virtual noresult dispatch_impl(std::function<void()> f) = 0;
  /// Platform specific implementation for \ref set_title
  virtual noresult set_title_impl(str_arg_t title) = 0;
  /// Platform specific implementation for \ref set_size
  virtual noresult set_size_impl(int width, int height,
                                 webview_hint_t hints) = 0;
  /// Platform specific implementation for \ref set_html
  virtual noresult set_html_impl(str_arg_t html) = 0;
  /// Platform specific implementation for \ref eval
  virtual noresult eval_impl(str_arg_t js) = 0;

  /// Adds a bound user function to Webview native code.
  // virtual user_script *add_user_script(str_arg_t js);
  /// Platform specific implementation to add a bound user JS function.
  virtual user_script add_user_script_impl(str_arg_t js) = 0;
  /// Platform specific implementation to remove all bound JS user functions
  /// from the Webview script.
  virtual void
  remove_all_user_scripts_impl(const std::list<user_script> &scripts) = 0;
  /// Platform specific method to compare equality of two bound user functions.
  virtual bool are_user_scripts_equal_impl(const user_script &first,
                                           const user_script &second) = 0;
  /// Replaces a bound user script in Webview native code.
  // virtual user_script *replace_user_script(const user_script &old_script,
  //                                         str_arg_t new_script_code);
  /// Updates the JS `bind` script in the frontend window.
  void replace_bind_script();
  /// Adds the JS Webview script to the frontend window
  void add_init_script(str_arg_t post_fn);
  // Creates a `bind` JS script string for the frontend window.
  std::string create_bind_script();
  /// Handler for messages from the frontend window to the native Webview
  /// process.
  virtual void on_message(const std::string &msg);
  /// Handler to increment the browser window count
  virtual void on_window_created();
  /// Handler to decrement the browser window count
  virtual void on_window_destroyed(bool skip_termination = false);
  /// Various internal backend scenarios require platform event queue depletion.
  /// This spins the platform event loop until queued events have been
  /// processed.
  void deplete_run_loop_event_queue();
  /// Various internal backend sccenarios require partial platform event queue
  /// depletion. This spins the platform event loop until the callback function
  /// returns true.
  virtual void run_event_loop_while(std::function<bool()> fn) = 0;
  /// Adds the default window size event to the platform event queue.
  /// This is guarded from execution if the user sets a preferred size.
  void dispatch_size_default();
  /// Explicitly toggle the guard for the setting of the default window size.
  /// This may be needed to moderate behaviour during premature platform event
  /// queue depletion.
  void set_default_size_guard(bool guarded);
  /// Gets a flag for whether the Webview window is embedded, or is owned by the
  /// user process.
  bool owns_window() const;

private:
  /// Keeps track of the number of platform window instances.
  static std::atomic_uint &window_ref_count();
  /// Increments the reference number of platform window instances.
  static unsigned int inc_window_count();
  /// Decrements the reference number of platform window instances.
  static unsigned int dec_window_count();
  /// A reference to the currently active JS bound script in the platform
  /// window.
  user_script *m_bind_script{};
  /// Flags if the JS Webview code has been sent to the platform window yet.
  bool m_is_init_script_sent{};
  /// Flags if the initial platform window size has been set.
  /// This acts as a guard against the default size overriding a user set size
  bool m_is_size_set{};
  /// Flags whether the Webview window is embedded or owned by the user process.
  bool m_owns_window{};
  /// The default platform window width
  static const int m_initial_width = 640;
  /// The default platform window height
  static const int m_initial_height = 480;
  /// Flag that indicates if a Webview work unit should skip the user work queue
  /// and execute directly
  bool skip_queue{};
};

} // namespace detail
} // namespace webview

#endif // defined(__cplusplus) && !defined(WEBVIEW_HEADER)
#endif // WEBVIEW_DETAIL_ENGINE_BASE_HH
// file end: include/webview/detail/engine_base.hh
// file begin: src/detail/threading/atomic_api.cc
#ifndef WEBVIEW_DETAIL_ATOMIC_API_CC
#define WEBVIEW_DETAIL_ATOMIC_API_CC

#if defined(__cplusplus) && !defined(WEBVIEW_HEADER)
namespace webview {
namespace detail {
namespace _struct {

bool atomic_done_t::bind() const { return self->bind_done.load(); }
void atomic_done_t::bind(bool val) {
  self->bind_done.store(val);
  self->cv.bind.notify_one();
}
bool atomic_done_t::unbind() const { return self->unbind_done.load(); }
void atomic_done_t::unbind(bool val) {
  self->unbind_done.store(val);
  self->cv.unbind.notify_one();
}
bool atomic_done_t::eval() const { return self->eval_done.load(); }
void atomic_done_t::eval(bool val) {
  self->eval_done.store(val);
  self->cv.eval.notify_one();
}

bool atomic_dom_ready_t::ready() const { return self->is_dom_ready.load(); };
void atomic_dom_ready_t::ready(bool flag) {
  self->is_dom_ready.store(flag);
  self->cv.queue.notify_one();
};

void atomic_queue_t::update() {
  if (self->list.queue.size() > 1) {
    self->list.queue.pop_front();
  } else {
    self->list.queue.clear();
  }
  self->atomic.queue.empty(self->list.queue.empty());
}

bool atomic_queue_t::empty() const { return self->queue_empty.load(); };
void atomic_queue_t::empty(bool val) {
  self->queue_empty.store(val);
  self->cv.queue.notify_one();
}

bool atomic_api_t::AND(std::initializer_list<bool> flags) const {
  if (self->atomic.terminating()) {
    return true;
  };
  auto res = true;
  for (auto &flag : flags) {
    if (!flag) {
      res = false;
      break;
    }
  }
  return res;
};
bool atomic_api_t::terminating() const { return self->is_terminating.load(); };

} // namespace _struct
} // namespace detail
} // namespace webview

#endif // defined(__cplusplus) && !defined(WEBVIEW_HEADER)
#endif // WEBVIEW_DETAIL_ATOMIC_API_CC
// file end: src/detail/threading/atomic_api.cc
// file begin: include/webview/detail/backends/gtk_webkitgtk.hh
#ifndef WEBVIEW_BACKENDS_GTK_WEBKITGTK_HH
#define WEBVIEW_BACKENDS_GTK_WEBKITGTK_HH

#if defined(__cplusplus) && !defined(WEBVIEW_HEADER)
#if defined(WEBVIEW_PLATFORM_LINUX) && defined(WEBVIEW_GTK)

//
// ====================================================================
//
// This implementation uses webkit2gtk backend. It requires GTK and
// WebKitGTK libraries. Proper compiler flags can be retrieved via:
//
//   pkg-config --cflags --libs gtk4 webkitgtk-6.0
//   pkg-config --cflags --libs gtk+-3.0 webkit2gtk-4.1
//   pkg-config --cflags --libs gtk+-3.0 webkit2gtk-4.0
//
// ====================================================================
//
#include <functional>
#include <list>
#include <memory>
#include <string>

#include <gtk/gtk.h>

#if GTK_MAJOR_VERSION >= 4

#include <jsc/jsc.h>
#include <webkit/webkit.h>

#elif GTK_MAJOR_VERSION >= 3

#include <JavaScriptCore/JavaScript.h>
#include <webkit2/webkit2.h>

#endif

#include <fcntl.h>
#include <sys/stat.h>

namespace webview {
namespace detail {

class user_script::impl {
public:
  impl(WebKitUserScript *script) : m_script{script} {
    webkit_user_script_ref(script);
  }

  ~impl() { webkit_user_script_unref(m_script); }

  impl(const impl &) = delete;
  impl &operator=(const impl &) = delete;
  impl(impl &&) = delete;
  impl &operator=(impl &&) = delete;

  WebKitUserScript *get_native() const { return m_script; }

private:
  WebKitUserScript *m_script{};
};

class gtk_webkit_engine : public engine_base {
public:
  gtk_webkit_engine(bool debug, void *window) : engine_base{!window} {
    queue.init(this);
    window_init(window);
    window_settings(debug);
    dispatch_size_default();
  }

  gtk_webkit_engine(const gtk_webkit_engine &) = delete;
  gtk_webkit_engine &operator=(const gtk_webkit_engine &) = delete;
  gtk_webkit_engine(gtk_webkit_engine &&) = delete;
  gtk_webkit_engine &operator=(gtk_webkit_engine &&) = delete;

  virtual ~gtk_webkit_engine() {
    queue.shutdown();
    if (m_window) {
      if (owns_window()) {
        // Disconnect handlers to avoid callbacks invoked during destruction.
        g_signal_handlers_disconnect_by_data(GTK_WINDOW(m_window), this);
        gtk_window_close(GTK_WINDOW(m_window));
        on_window_destroyed(true);
      } else {
        gtk_compat::window_remove_child(GTK_WINDOW(m_window),
                                        GTK_WIDGET(m_webview));
      }
    }
    if (m_webview) {
      g_object_unref(m_webview);
    }
    if (owns_window()) {
      // Needed for the window to close immediately.
      deplete_run_loop_event_queue();
    }
  }

protected:
  result<void *> window_impl() override {
    if (m_window) {
      return m_window;
    }
    return error_info{WEBVIEW_ERROR_INVALID_STATE};
  }

  result<void *> widget_impl() override {
    if (m_webview) {
      return m_webview;
    }
    return error_info{WEBVIEW_ERROR_INVALID_STATE};
  }

  result<void *> browser_controller_impl() override {
    if (m_webview) {
      return m_webview;
    }
    return error_info{WEBVIEW_ERROR_INVALID_STATE};
  }

  noresult run_impl() override {
    m_stop_run_loop = false;
    while (!m_stop_run_loop) {
      g_main_context_iteration(nullptr, TRUE);
    }
    return {};
  }

  noresult terminate_impl() override {
    return dispatch_impl([&] { m_stop_run_loop = true; });
  }

  noresult dispatch_impl(std::function<void()> f) override {
    g_idle_add_full(G_PRIORITY_HIGH_IDLE, (GSourceFunc)([](void *fn) -> int {
                      (*static_cast<dispatch_fn_t *>(fn))();
                      return G_SOURCE_REMOVE;
                    }),
                    new std::function<void()>(f),
                    [](void *fn) { delete static_cast<dispatch_fn_t *>(fn); });
    return {};
  }

  noresult set_title_impl(const std::string &title) override {
    gtk_window_set_title(GTK_WINDOW(m_window), title.c_str());
    return {};
  }

  noresult set_size_impl(int width, int height, webview_hint_t hints) override {
    gtk_window_set_resizable(GTK_WINDOW(m_window), hints != WEBVIEW_HINT_FIXED);
    if (hints == WEBVIEW_HINT_NONE) {
      gtk_compat::window_set_size(GTK_WINDOW(m_window), width, height);
    } else if (hints == WEBVIEW_HINT_FIXED || hints == WEBVIEW_HINT_MIN) {
      gtk_widget_set_size_request(m_window, width, height);
    } else if (hints == WEBVIEW_HINT_MAX) {
      gtk_compat::window_set_max_size(GTK_WINDOW(m_window), width, height);
    } else {
      return error_info{WEBVIEW_ERROR_INVALID_ARGUMENT, "Invalid hint"};
    }
    return window_show();
  }

  noresult navigate_impl(const std::string &url) override {
    webkit_web_view_load_uri(WEBKIT_WEB_VIEW(m_webview), url.c_str());
    return {};
  }

  noresult set_html_impl(const std::string &html) override {
    webkit_web_view_load_html(WEBKIT_WEB_VIEW(m_webview), html.c_str(),
                              nullptr);
    return {};
  }

  noresult eval_impl(const std::string &js) override {
    // URI is null before content has begun loading.
    if (!webkit_web_view_get_uri(WEBKIT_WEB_VIEW(m_webview))) {
      return {};
    }
#if (WEBKIT_MAJOR_VERSION == 2 && WEBKIT_MINOR_VERSION >= 40) ||               \
    WEBKIT_MAJOR_VERSION > 2
    webkit_web_view_evaluate_javascript(WEBKIT_WEB_VIEW(m_webview), js.c_str(),
                                        static_cast<gssize>(js.size()), nullptr,
                                        nullptr, nullptr, nullptr, nullptr);
#else
    webkit_web_view_run_javascript(WEBKIT_WEB_VIEW(m_webview), js.c_str(),
                                   nullptr, nullptr, nullptr);
#endif
    return {};
  }

  user_script add_user_script_impl(const std::string &js) override {
    auto *wk_script = webkit_user_script_new(
        js.c_str(), WEBKIT_USER_CONTENT_INJECT_TOP_FRAME,
        WEBKIT_USER_SCRIPT_INJECT_AT_DOCUMENT_START, nullptr, nullptr);
    webkit_user_content_manager_add_script(m_user_content_manager, wk_script);
    user_script script{
        js, user_script::impl_ptr{new user_script::impl{wk_script},
                                  [](user_script::impl *p) { delete p; }}};
    webkit_user_script_unref(wk_script);
    return script;
  }

  void remove_all_user_scripts_impl(
      const std::list<user_script> & /*scripts*/) override {
    webkit_user_content_manager_remove_all_scripts(m_user_content_manager);
  }

  bool are_user_scripts_equal_impl(const user_script &first,
                                   const user_script &second) override {
    auto *wk_first = first.get_impl().get_native();
    auto *wk_second = second.get_impl().get_native();
    return wk_first == wk_second;
  }

private:
#if GTK_MAJOR_VERSION >= 4
  static char *get_string_from_js_result(JSCValue *r) {
    return jsc_value_to_string(r);
  }
#else
  static char *get_string_from_js_result(WebKitJavascriptResult *r) {
    char *s;
#if (WEBKIT_MAJOR_VERSION == 2 && WEBKIT_MINOR_VERSION >= 22) ||               \
    WEBKIT_MAJOR_VERSION > 2
    JSCValue *value = webkit_javascript_result_get_js_value(r);
    s = jsc_value_to_string(value);
#else
    JSGlobalContextRef ctx = webkit_javascript_result_get_global_context(r);
    JSValueRef value = webkit_javascript_result_get_value(r);
    JSStringRef js = JSValueToStringCopy(ctx, value, nullptr);
    size_t n = JSStringGetMaximumUTF8CStringSize(js);
    s = g_new(char, n);
    JSStringGetUTF8CString(js, s, n);
    JSStringRelease(js);
#endif
    return s;
  }
#endif

  void window_init(void *window) {
    m_window = static_cast<GtkWidget *>(window);
    if (owns_window()) {
      if (!gtk_compat::init_check()) {
        throw exception{WEBVIEW_ERROR_UNSPECIFIED, "GTK init failed"};
      }
      m_window = gtk_compat::window_new();
      on_window_created();
      auto on_window_destroy = +[](GtkWidget *, gpointer arg) {
        auto *w = static_cast<gtk_webkit_engine *>(arg);
        w->m_window = nullptr;
        w->on_window_destroyed();
      };
      g_signal_connect(G_OBJECT(m_window), "destroy",
                       G_CALLBACK(on_window_destroy), this);
    }
    webkit_dmabuf::apply_webkit_dmabuf_workaround();
    // Initialize webview widget
    m_webview = webkit_web_view_new();
    g_object_ref_sink(m_webview);
    WebKitUserContentManager *manager = m_user_content_manager =
        webkit_web_view_get_user_content_manager(WEBKIT_WEB_VIEW(m_webview));
    webkitgtk_compat::connect_script_message_received(
        manager, "__webview__",
        [this](WebKitUserContentManager *, const std::string &r) {
          on_message(r);
        });
    webkitgtk_compat::user_content_manager_register_script_message_handler(
        manager, "__webview__");
    add_init_script("function(message) {\n\
  return window.webkit.messageHandlers.__webview__.postMessage(message);\n\
}");
  }

  void window_settings(bool debug) {
    WebKitSettings *settings =
        webkit_web_view_get_settings(WEBKIT_WEB_VIEW(m_webview));
    webkit_settings_set_javascript_can_access_clipboard(settings, true);
    if (debug) {
      webkit_settings_set_enable_write_console_messages_to_stdout(settings,
                                                                  true);
      webkit_settings_set_enable_developer_extras(settings, true);
    }
  }

  noresult window_show() {
    if (m_is_window_shown) {
      return {};
    }
    gtk_compat::window_set_child(GTK_WINDOW(m_window), GTK_WIDGET(m_webview));
    gtk_compat::widget_set_visible(GTK_WIDGET(m_webview), true);

    if (owns_window()) {
      gtk_widget_grab_focus(GTK_WIDGET(m_webview));
      gtk_compat::widget_set_visible(GTK_WIDGET(m_window), true);
    }
    m_is_window_shown = true;
    return {};
  }

  void run_event_loop_while(std::function<bool()> fn) override {
    while (fn()) {
      g_main_context_iteration(nullptr, TRUE);
    }
  }

  GtkWidget *m_window{};
  GtkWidget *m_webview{};
  WebKitUserContentManager *m_user_content_manager{};
  bool m_stop_run_loop{};
  bool m_is_window_shown{};
};

} // namespace detail

using browser_engine = detail::gtk_webkit_engine;

} // namespace webview

#endif // defined(WEBVIEW_PLATFORM_LINUX) && defined(WEBVIEW_GTK)
#endif // defined(__cplusplus) && !defined(WEBVIEW_HEADER)
#endif // WEBVIEW_BACKENDS_GTK_WEBKITGTK_HH
// file end: include/webview/detail/backends/gtk_webkitgtk.hh
// file begin: src/detail/engine_queue.cc
#ifndef WEBVIEW_ENGINE_QUEUE_CC
#define WEBVIEW_ENGINE_QUEUE_CC

#if defined(__cplusplus) && !defined(WEBVIEW_HEADER)
#include <cstdio>
#include <mutex>

namespace webview {
detail::engine_queue::engine_queue() : queue{this}, atomic{this} {}

// PUBLIC API implementation
namespace detail {

using public_api_t = engine_queue::public_api_t;
using bind_api_t = engine_queue::bind_api_t;
using unbind_api_t = engine_queue::unbind_api_t;
using promise_api_t = engine_queue::promise_api_t;
using eval_api_t = engine_queue::eval_api_t;

noresult bind_api_t::enqueue(do_work_t fn, str_arg_t name) const {
  return self->queue_work(name, fn, self->ctx.bind);
};
bool bind_api_t::is_duplicate(str_arg_t name) const {
  auto i = self->list.pending.indices(name);
  auto will_be_bound = i.bind_i > -1 && i.bind_i > i.unbind_i;
  return self->list.bindings.count(name) > 0 || will_be_bound;
};

noresult unbind_api_t::enqueue(do_work_t fn, str_arg_t name) const {
  return self->queue_work(name, fn, self->ctx.unbind);
};
bool unbind_api_t::not_found(str_arg_t name) const {
  auto i = self->list.pending.indices(name);
  auto will_be_bound = i.bind_i > -1 && i.bind_i > i.unbind_i;
  return self->list.bindings.count(name) == 0 && !will_be_bound;
};

noresult eval_api_t::enqueue(do_work_t fn, str_arg_t js) const {
  return self->queue_work(js, fn, self->ctx.eval);
};

void promise_api_t::resolved(str_arg_t id) const {
  auto name = self->list.id_name_map.get(id);
  if (name == "") {
    return;
  };
  self->list.id_name_map.erase(id);
  self->list.unresolved_promises.remove_id(name, id);
  if (self->list.unresolved_promises.empty(name)) {
    self->cv.unbind_timeout.notify_one();
    self->list.unresolved_promises.erase(name);
  }
};
void promise_api_t::resolve(str_arg_t name, str_arg_t id, str_arg_t args,
                            engine_base *wv) const {
  self->list.id_name_map.set(id, name);
  self->list.unresolved_promises.add_id(name, id);
  self->cv.unbind_timeout.notify_one();
  {
    std::mutex mtx;
    std::lock_guard<std::mutex> lock(mtx);
    std::thread resolver = std::thread(
        &engine_queue::resolve_thread_constructor, self, name, id, args, wv);
    resolver.detach();
  }
}
bool promise_api_t::is_system_message(str_arg_t id, str_arg_t method) {
  if (id != SYSTEM_NOTIFICATION_FLAG) {
    return false;
  };
  if (method == frontend.sysops.dom_ready) {
    log::trace::queue.notify.on_message(method);
    self->atomic.dom.ready(true);
  }
  if (method == frontend.sysops.bind_done) {
    log::trace::queue.notify.on_message(method);
    self->atomic.done.bind(true);
  }
  if (method == frontend.sysops.unbind_done) {
    log::trace::queue.notify.on_message(method);
    self->atomic.done.unbind(true);
  }
  if (method == frontend.sysops.js_eval_start) {
    log::trace::queue.notify.on_message(method);
    self->atomic.done.eval(true);
  }
  return true;
}

void public_api_t::init(engine_base *wv_instance) const {
  self->queue_thread =
      std::thread(&engine_queue::queue_thread_constructor, self, wv_instance);
}
void public_api_t::shutdown() const {
  self->is_terminating.store(true);
  self->queue_empty.store(false);
  self->is_dom_ready.store(true);
  self->atomic.done.bind(true);
  self->atomic.done.unbind(true);
  self->atomic.done.eval(true);
  self->cv.notify_all();
  self->queue_thread.join();
}
bool public_api_t::shutting_down() const { return self->is_terminating.load(); }

noresult engine_queue::queue_work(str_arg_t name_or_js, do_work_t fn,
                                  context_t fn_ctx) {
  const auto &name = name_or_js;
  if (fn_ctx == ctx.bind) {
    list.pending.push_back("bind-" + name);
  }
  if (fn_ctx == ctx.unbind) {
    list.pending.push_back("unbind-" + name);
  }
  list.queue.push_back(fn_ctx, fn, name_or_js);
  log::trace::queue.enqueue.added(char(fn_ctx), list.queue.size(), name_or_js);
  atomic.queue.empty(false);
  return {};
};

} // namespace detail
} // namespace webview

#endif // defined(__cplusplus) && !defined(WEBVIEW_HEADER)
#endif // WEBVIEW_ENGINE_QUEUE_CC
// file end: src/detail/engine_queue.cc
// file begin: src/detail/threading/thread_queue.cc
#ifndef WEBVIEW_ENGINE_THREAD_QUEUE_CC
#define WEBVIEW_ENGINE_THREAD_QUEUE_CC

#if defined(__cplusplus) && !defined(WEBVIEW_HEADER)
#include <mutex>

namespace webview {
namespace detail {

void engine_queue::queue_thread_constructor(engine_base *wv_instance) {
  std::mutex queue_thread_mtx;
  std::unique_lock<std::mutex> lock(queue_thread_mtx);
  while (true) {
    log::trace::queue.loop.wait(list.queue.size(), atomic.queue.empty(),
                                atomic.dom.ready());
    cv.queue.wait(lock, [this] {
      return atomic.AND({atomic.dom.ready(), !atomic.queue.empty()});
    });
    if (atomic.terminating()) {
      break;
    }
    log::trace::queue.loop.start(list.queue.size());
    auto action = list.queue.front();
    context_t work_ctx = action.ctx;
    std::string name = action.name_or_js;
    std::string js = action.name_or_js;
    auto work_fn = action.work_fn;

    // `bind` user work unit
    if (work_ctx == ctx.bind) {
      log::trace::queue.bind.start(name);
      wv_instance->dispatch(work_fn);
      cv.bind.wait(lock, [this] { return atomic.AND({atomic.done.bind()}); });
      if (atomic.terminating()) {
        break;
      }
      list.pending.pop_front();
      log::trace::queue.bind.done(atomic.done.bind(), name);
      atomic.done.bind(false);
    }

    // `unbind` user work unit
    if (work_ctx == ctx.unbind) {
      log::trace::queue.unbind.wait(name);
      auto timeout = std::chrono::milliseconds(WEBVIEW_UNBIND_TIMEOUT);
      cv.unbind_timeout.wait_for(lock, timeout, [this, &name] {
        return atomic.AND({list.unresolved_promises.empty(name)});
      });
      if (atomic.terminating()) {
        break;
      }
      log::trace::queue.unbind.start(name);
      auto promises = list.unresolved_promises.get_copy(name);
      for (auto &id : promises) {
        auto err = frontend.err_message.reject_unbound(id, name);
        wv_instance->reject(id, err);
      }

      wv_instance->dispatch(work_fn);
      cv.unbind.wait(lock,
                     [this] { return atomic.AND({atomic.done.unbind()}); });
      list.pending.pop_front();
      log::trace::queue.unbind.done(atomic.done.unbind(), name);
      atomic.done.unbind(false);
    }

    // `eval` user work unit
    if (work_ctx == ctx.eval) {
      log::trace::queue.eval.start(js);
      wv_instance->dispatch(work_fn);
      cv.eval.wait(lock, [this] { return atomic.AND({atomic.done.eval()}); });
      if (atomic.terminating()) {
        break;
      }
      log::trace::queue.eval.done(atomic.done.eval());
      atomic.done.eval(false);
    }

    atomic.queue.update();
    log::trace::queue.loop.end();
  }
}

} // namespace detail
} // namespace webview

#endif // defined(__cplusplus) && !defined(WEBVIEW_HEADER)
#endif // WEBVIEW_ENGINE_THREAD_QUEUE_CC
// file end: src/detail/threading/thread_queue.cc
// file begin: src/detail/threading/thread_resolve.cc
#ifndef WEBVIEW_ENGINE_THREAD_RESOLVE_CC
#define WEBVIEW_ENGINE_THREAD_RESOLVE_CC

#if defined(__cplusplus) && !defined(WEBVIEW_HEADER)
namespace webview {
namespace detail {

void engine_queue::resolve_thread_constructor(str_arg_t name, str_arg_t id,
                                              str_arg_t args, engine_base *wv) {
  if (atomic.terminating()) {
    return;
  }
  try {
    list.bindings.at(name).call(id, args);
  } catch (const std::exception &err_) {
    auto err = frontend.err_message.uncaught_exception(name, err_.what());
    wv->reject(id, err);
  } catch (...) {
    perror(frontend.err_message.webview_terminated(name).c_str());
  };
}

} // namespace detail
} // namespace webview
#endif // defined(__cplusplus) && !defined(WEBVIEW_HEADER)
#endif // WEBVIEW_ENGINE_THREAD_RESOLVE_CC
// file end: src/detail/threading/thread_resolve.cc
// file begin: src/detail/threading/threadsafe_lists.cc
#ifndef WEBVIEW_DETAIL_THREADSAFE_LISTS_CC
#define WEBVIEW_DETAIL_THREADSAFE_LISTS_CC

#if defined(__cplusplus) && !defined(WEBVIEW_HEADER)
#include <algorithm>
#include <iterator>

namespace webview {
namespace detail {
namespace _structs {

size_t bindings_t::size() const {
  std::lock_guard<std::mutex> lock(mtx);
  return bindings_map.size();
}
void bindings_t::get_names(std::vector<std::string> &bound_names) {
  std::lock_guard<std::mutex> lock(mtx);
  bound_names.reserve(bindings_map.size());
  std::transform(bindings_map.begin(), bindings_map.end(),
                 std::back_inserter(bound_names),
                 [](const std::pair<std::string, binding_ctx_t> &pair) {
                   return pair.first;
                 });
}
bool bindings_t::has_name(str_arg_t name) const {
  std::lock_guard<std::mutex> lock(mtx);
  bool is_found{true};
  auto found = bindings_map.find(name);
  if (found == bindings_map.end()) {
    is_found = false;
  }
  found = bindings_map.end();
  return is_found;
}
void bindings_t::emplace(str_arg_t name, binding_t fn, void *arg) {
  std::lock_guard<std::mutex> lock(mtx);
  bindings_map.emplace(name, binding_ctx_t(fn, arg));
}
void bindings_t::erase(str_arg_t name) {
  std::lock_guard<std::mutex> lock(mtx);
  bindings_map.erase(name);
}
size_t bindings_t::count(str_arg_t name) const {
  std::lock_guard<std::mutex> lock(mtx);
  return bindings_map.count(name);
}
binding_ctx_t bindings_t::at(str_arg_t name) const {
  std::lock_guard<std::mutex> lock(mtx);
  return bindings_map.at(name);
}

user_script *user_scripts_t::add(str_arg_t js, engine_base *base) {
  std::lock_guard<std::mutex> lock(mtx);
  return std::addressof(*m_user_scripts.emplace(
      m_user_scripts.end(), base->add_user_script_impl(js)));
}
user_script *user_scripts_t::replace(const user_script &old_script,
                                     str_arg_t new_script_code,
                                     engine_base *base) {
  std::lock_guard<std::mutex> lock(mtx);
  base->remove_all_user_scripts_impl(m_user_scripts);
  user_script *old_script_ptr{};
  for (auto &script : m_user_scripts) {
    auto is_old_script = base->are_user_scripts_equal_impl(script, old_script);
    script = base->add_user_script_impl(is_old_script ? new_script_code
                                                      : script.get_code());
    if (is_old_script) {
      old_script_ptr = std::addressof(script);
    }
  }
  return old_script_ptr;
}

action_t queue_t::front() const {
  std::lock_guard<std::mutex> lock(mtx);
  return queue.front();
}
size_t queue_t::size() const {
  std::lock_guard<std::mutex> lock(mtx);
  return queue.size();
}
void queue_t::pop_front() {
  std::lock_guard<std::mutex> lock(mtx);
  queue.pop_front();
}
void queue_t::push_back(context_t ctx, do_work_t work_fn,
                        std::string name_or_js) {
  std::lock_guard<std::mutex> lock(mtx);
  queue.push_back({ctx, work_fn, name_or_js});
}
void queue_t::clear() {
  std::lock_guard<std::mutex> lock(mtx);
  queue.clear();
}
bool queue_t::empty() const {
  std::lock_guard<std::mutex> lock(mtx);
  return queue.empty();
}

void unres_promises_t::set(str_arg_t name, std::list<std::string> ids) {
  std::lock_guard<std::mutex> lock(mtx);
  unres_promises[name] = std::move(ids);
}
std::list<std::string> unres_promises_t::get_copy(str_arg_t name) const {
  std::lock_guard<std::mutex> lock(mtx);
  auto found = unres_promises.find(name);
  if (found == unres_promises.end()) {
    return {};
  }
  std::list<std::string> list_copy = found->second;
  found = unres_promises.end();
  return list_copy;
}
void unres_promises_t::remove_id(str_arg_t name, str_arg_t id) {
  std::lock_guard<std::mutex> lock(mtx);
  auto found = unres_promises.find(name);
  if (found == unres_promises.end()) {
    return;
  }
  found->second.remove(id);
  found = unres_promises.end();
}
void unres_promises_t::add_id(str_arg_t name, str_arg_t id) {
  std::lock_guard<std::mutex> lock(mtx);
  auto found = unres_promises.find(name);
  if (found == unres_promises.end()) {
    unres_promises.emplace(name, std::list<std::string>{id});
  } else {
    found->second.push_back(id);
  }
  found = unres_promises.end();
}
void unres_promises_t::erase(str_arg_t name) {
  std::lock_guard<std::mutex> lock(mtx);
  unres_promises.erase(name);
}
bool unres_promises_t::empty(str_arg_t name) const {
  std::lock_guard<std::mutex> lock(mtx);
  auto found = unres_promises.find(name);
  if (found == unres_promises.end()) {
    return true;
  }
  auto empty = found->second.empty();
  found = unres_promises.end();
  return empty;
}

std::string id_name_map_t::get(str_arg_t id) const {
  std::lock_guard<std::mutex> lock(mtx);
  auto found = id_name.find(id);
  if (found == id_name.end()) {
    return "";
  }
  auto name = found->second;
  found = id_name.end();
  return name;
}
void id_name_map_t::set(str_arg_t id, str_arg_t name) {
  std::lock_guard<std::mutex> lock(mtx);
  id_name[id] = name;
}
void id_name_map_t::erase(str_arg_t id) {
  std::lock_guard<std::mutex> lock(mtx);
  id_name.erase(id);
}

void pending_t::pop_front() {
  std::lock_guard<std::mutex> lock(mtx);
  pending_bind_unbind.pop_front();
}
void pending_t::push_back(str_arg_t name) {
  std::lock_guard<std::mutex> lock(mtx);
  pending_bind_unbind.push_back(name);
}
indices_t pending_t::indices(str_arg_t name) const {
  std::lock_guard<std::mutex> lock(mtx);
  auto len = int(pending_bind_unbind.size());
  int bind_i = -1;
  int unbind_i = -1;
  for (int i = 0; i < len; i++) {
    auto val = pending_bind_unbind.at(i);
    if (val == "bind-" + name) {
      bind_i = i;
    }
    if (val == "unbind-" + name) {
      unbind_i = i;
    }
  }
  return {bind_i, unbind_i};
}

void cv_api_t::notify_all() {
  for (auto &this_cv : all) {
    this_cv->notify_all();
  }
}

} // namespace _structs
} // namespace detail
} // namespace webview

#endif // defined(__cplusplus) && !defined(WEBVIEW_HEADER)
#endif // WEBVIEW_DETAIL_THREADSAFE_LISTS_CC
// file end: src/detail/threading/threadsafe_lists.cc
// file begin: tests/include/test_helper.hh
#ifndef WEBVIEW_TEST_HELPER_HH
#define WEBVIEW_TEST_HELPER_HH

#if defined(__cplusplus) && !defined(WEBVIEW_HEADER)
#include <atomic>
#include <chrono>
#include <condition_variable>

namespace webview {
using namespace webview::detail;

namespace test {
namespace _templates {

#define TEST_VALUE_WRAPPER                                                     \
  "window.__webview__.post(\n"                                                 \
  "  JSON.stringify({\n"                                                       \
  "    id: \"" TEST_NOTIFICATION_FLAG "\",\n"                                  \
  "    method: " TOKEN_VALUE "\n"                                              \
  "  })\n"                                                                     \
  ");"

} // namespace _templates

/// Static test utilities class
class tester {
public:
  static std::condition_variable &cv() {
    static std::condition_variable cv_;
    return cv_;
  }

  /// Sets the returned test value.
  static void set_value(const std::string &val) {
    string_value() = val;
    compare_values();
  };

  /// Sets the expected returned test value.
  static void expect_value(const std::string &value) {
    string_expected_value() = value;
  };

  /// Flag for evaluating if the returned value matches the expected value.
  static bool values_match() {
    compare_values();
    return worker_proceed().load();
  };

  /// Wraps a string value in evaluable JS
  static std::string get_value_js(const std::string &value) {
    return str::tokenise(TEST_VALUE_WRAPPER, str::token.value, value);
  }

  /// dispatch -> wv.eval of a tokenised `window.__webview__.post` string with a
  /// test value
  static void post_value(const std::string &value, engine_base *wv) {
    auto js = get_value_js(value);
    auto do_work = [=] { wv->eval(js); };
    wv->dispatch(do_work);
  }

  /// Wraps wv.terminate in dispatch
  static void terminate(engine_base *wv) {
    wv->dispatch([=] { wv->terminate(); });
  }

  /// Returns the given timespan in std::chrono::seconds
  static std::chrono::seconds seconds(int seconds = 1) {
    return std::chrono::seconds(seconds);
  };

private:
  static std::string &string_value() {
    static std::string val;
    return val;
  };
  static std::string &string_expected_value() {
    static std::string val;
    return val;
  };
  static std::atomic_bool &worker_proceed() {
    static std::atomic_bool val{};
    return val;
  }

  static void compare_values() {
    worker_proceed().store(string_expected_value() == string_value());
    cv().notify_one();
  }
};
} // namespace test
} // namespace webview

#endif // defined(__cplusplus) && !defined(WEBVIEW_HEADER)
#endif // WEBVIEW_TEST_HELPER_HH
// file end: tests/include/test_helper.hh
// file begin: include/webview/detail/backends/backends.hh
#ifndef WEBVIEW_BACKENDS_HH
#define WEBVIEW_BACKENDS_HH

#if defined(__cplusplus) && !defined(WEBVIEW_HEADER)

// #include "cocoa_webkit.hh"

namespace webview {
using webview = browser_engine;
}

#endif // defined(__cplusplus) && !defined(WEBVIEW_HEADER)
#endif // WEBVIEW_BACKENDS_HH
// file end: include/webview/detail/backends/backends.hh
// file begin: src/detail/threading/_src_threading.hh
#ifndef WEBVIEW_AMALGAMATED_SRC_THREADING_HH
#define WEBVIEW_AMALGAMATED_SRC_THREADING_HH

#if defined(__cplusplus) && !defined(WEBVIEW_HEADER)
#endif
#endif
// file end: src/detail/threading/_src_threading.hh
// file begin: src/detail/engine_base.cc
#ifndef WEBVIEW_DETAIL_ENGINE_BASE_CC
#define WEBVIEW_DETAIL_ENGINE_BASE_CC

#if defined(__cplusplus) && !defined(WEBVIEW_HEADER)
namespace webview {
using namespace webview::test;
namespace detail {

engine_base::engine_base(bool owns_window) : m_owns_window{owns_window} {}

noresult engine_base::navigate(str_arg_t url) {
  if (url.empty()) {
    return navigate_impl("about:blank");
  }
  return navigate_impl(url);
}

noresult engine_base::bind(str_arg_t name, sync_binding_t fn) {
  auto wrapper = [this, fn](str_arg_t id, str_arg_t req, void * /*arg*/) {
    resolve(id, 0, fn(req));
  };
  skip_queue = true;
  auto res = bind(name, wrapper, nullptr);
  skip_queue = false;
  return res;
}

noresult engine_base::bind(str_arg_t name, binding_t fn, void *arg) {
  log::trace::base.bind.start(name);
  do_work_t do_work = [this, name, fn, arg] {
    log::trace::base.bind.work(name);
    list.bindings.emplace(name, fn, arg);
    replace_bind_script();
    skip_queue = true;
    eval(frontend.js.onbind(name));
    skip_queue = false;
  };
  if (queue.bind.is_duplicate(name)) {
    return error_info{WEBVIEW_ERROR_DUPLICATE};
  }
  if (!skip_queue) {
    return queue.bind.enqueue(do_work, name);
  }
  do_work();
  return {};
}

noresult engine_base::unbind(str_arg_t name) {
  log::trace::base.unbind.start(name);
  do_work_t do_work = [this, name]() {
    log::trace::base.unbind.work(name);
    skip_queue = true;
    eval(frontend.js.onunbind(name));
    skip_queue = false;
    list.bindings.erase(name);
    replace_bind_script();
  };
  if (queue.unbind.not_found(name)) {
    return error_info{WEBVIEW_ERROR_NOT_FOUND};
  }
  return queue.unbind.enqueue(do_work, name);
}

noresult engine_base::resolve(str_arg_t id, int status, str_arg_t result) {
  auto escaped_result = result.empty() ? "undefined" : json_escape(result);
  auto promised_js = frontend.js.onreply(id, status, escaped_result);

  return dispatch([this, promised_js, id] {
    skip_queue = true;
    eval(promised_js);
    skip_queue = false;
    queue.promises.resolved(id);
  });
}

noresult engine_base::reject(str_arg_t id, str_arg_t err) {
  return resolve(id, 1, json_escape(err));
}

result<void *> engine_base::window() { return window_impl(); }

result<void *> engine_base::widget() { return widget_impl(); }

result<void *> engine_base::browser_controller() {
  return browser_controller_impl();
}

noresult engine_base::run() { return run_impl(); }

noresult engine_base::terminate() { return terminate_impl(); }

noresult engine_base::dispatch(std::function<void()> f) {
  return dispatch_impl(f);
}

noresult engine_base::set_title(str_arg_t title) {
  return set_title_impl(title);
}

noresult engine_base::set_size(int width, int height, webview_hint_t hints) {
  auto res = set_size_impl(width, height, hints);
  m_is_size_set = true;
  return res;
}

noresult engine_base::set_html(str_arg_t html) { return set_html_impl(html); }

noresult engine_base::init(str_arg_t js) {
  list.m_user_scripts.add(js, this);
  return {};
}

noresult engine_base::eval(str_arg_t js) {
  log::trace::base.eval.start(js, skip_queue);
  do_work_t do_work = [this, js] {
    if (queue.shutting_down()) {
      return;
    }
    auto wrapped_js = frontend.js.eval_wrapper(js);
    log::trace::base.eval.work(wrapped_js);
    eval_impl(wrapped_js);
  };

  if (!skip_queue) {
    return queue.eval.enqueue(do_work, js);
  }
  log::trace::base.eval.work(js);
  eval_impl(js);
  return {};
}

void engine_base::replace_bind_script() {
  auto replacement_js = create_bind_script();
  if (m_bind_script) {
    m_bind_script =
        list.m_user_scripts.replace(*m_bind_script, replacement_js, this);
  } else {
    m_bind_script = list.m_user_scripts.add(replacement_js, this);
  }
}

void engine_base::add_init_script(str_arg_t post_fn) {
  auto init_js = frontend.js.init(post_fn);
  list.m_user_scripts.add(init_js, this);
  m_is_init_script_sent = true;
}

std::string engine_base::create_bind_script() {
  std::vector<std::string> bound_names;
  list.bindings.get_names(bound_names);
  return frontend.js.bind(bound_names);
}

void engine_base::on_message(str_arg_t msg) {
  auto id = json_parse(msg, "id", 0);
  if (id == TEST_NOTIFICATION_FLAG) {
    auto test_value = json_parse(msg, "method", 0);
    tester::set_value(test_value);
    return;
  }
  auto name = json_parse(msg, "method", 0);
  if (queue.promises.is_system_message(id, name)) {
    return;
  }
  if (!list.bindings.has_name(name)) {
    auto message = frontend.err_message.reject_unbound(id, name);
    reject(id, message);
    return;
  }
  auto args = json_parse(msg, "params", 0);
  queue.promises.resolve(name, id, args, this);
}

void engine_base::on_window_created() { inc_window_count(); }

void engine_base::on_window_destroyed(bool skip_termination) {
  if (dec_window_count() <= 0) {
    if (!skip_termination) {
      terminate();
    }
  }
}

void engine_base::deplete_run_loop_event_queue() {
  bool done{};
  dispatch([&] { done = true; });
  run_event_loop_while([&] { return !done; });
}

void engine_base::dispatch_size_default() {
  if (!owns_window() || !m_is_init_script_sent) {
    return;
  };
  dispatch([this]() {
    if (!m_is_size_set) {
      set_size(m_initial_width, m_initial_height, WEBVIEW_HINT_NONE);
    }
  });
}

void engine_base::set_default_size_guard(bool guarded) {
  m_is_size_set = guarded;
}

bool engine_base::owns_window() const { return m_owns_window; }

std::atomic_uint &engine_base::window_ref_count() {
  static std::atomic_uint ref_count{0};
  return ref_count;
}

unsigned int engine_base::inc_window_count() { return ++window_ref_count(); }

unsigned int engine_base::dec_window_count() {
  auto &count = window_ref_count();
  if (count > 0) {
    return --count;
  }
  return 0;
}

} // namespace detail
} // namespace webview
#endif // defined(__cplusplus) && !defined(WEBVIEW_HEADER)
#endif // WEBVIEW_DETAIL_ENGINE_BASE_CC
// file end: src/detail/engine_base.cc
// file begin: include/webview/api/c_api_impl.hh
#ifndef WEBVIEW_C_API_IMPL_HH
#define WEBVIEW_C_API_IMPL_HH

#if defined(__cplusplus) && !defined(WEBVIEW_HEADER)
namespace webview {
namespace detail {

// The library's version information.
constexpr const webview_version_info_t library_version_info{
    {WEBVIEW_VERSION_MAJOR, WEBVIEW_VERSION_MINOR, WEBVIEW_VERSION_PATCH},
    WEBVIEW_VERSION_NUMBER,
    WEBVIEW_VERSION_PRE_RELEASE,
    WEBVIEW_VERSION_BUILD_METADATA};

template <typename WorkFn, typename ResultFn>
webview_error_t api_filter(WorkFn &&do_work, ResultFn &&put_result) noexcept {
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
webview_error_t api_filter(WorkFn &&do_work) noexcept {
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

inline webview *cast_to_webview(void *w) {
  if (!w) {
    throw exception{WEBVIEW_ERROR_INVALID_ARGUMENT,
                    "Cannot cast null pointer to webview instance"};
  }
  return static_cast<webview *>(w);
}

} // namespace detail
} // namespace webview

WEBVIEW_API webview_t webview_create(int debug, void *wnd) {
  using namespace webview::detail;
  webview::webview *w{};
  auto err = api_filter(
      [=]() -> webview::result<webview::webview *> {
        return new webview::webview{static_cast<bool>(debug), wnd};
      },
      [&](webview::webview *w_) { w = w_; });
  if (err == WEBVIEW_ERROR_OK) {
    return w;
  }
  return nullptr;
}

WEBVIEW_API webview_error_t webview_destroy(webview_t w) {
  using namespace webview::detail;
  return api_filter([=]() -> webview::noresult {
    delete cast_to_webview(w);
    return {};
  });
}

WEBVIEW_API webview_error_t webview_run(webview_t w) {
  using namespace webview::detail;
  return api_filter([=] { return cast_to_webview(w)->run(); });
}

WEBVIEW_API webview_error_t webview_terminate(webview_t w) {
  using namespace webview::detail;
  return api_filter([=] { return cast_to_webview(w)->terminate(); });
}

WEBVIEW_API webview_error_t webview_dispatch(webview_t w,
                                             void (*fn)(webview_t, void *),
                                             void *arg) {
  using namespace webview::detail;
  if (!fn) {
    return WEBVIEW_ERROR_INVALID_ARGUMENT;
  }
  return api_filter(
      [=] { return cast_to_webview(w)->dispatch([=]() { fn(w, arg); }); });
}

WEBVIEW_API void *webview_get_window(webview_t w) {
  using namespace webview::detail;
  void *window = nullptr;
  auto err = api_filter([=] { return cast_to_webview(w)->window(); },
                        [&](void *value) { window = value; });
  if (err == WEBVIEW_ERROR_OK) {
    return window;
  }
  return nullptr;
}

WEBVIEW_API void *webview_get_native_handle(webview_t w,
                                            webview_native_handle_kind_t kind) {
  using namespace webview::detail;
  void *handle{};
  auto err = api_filter(
      [=]() -> webview::result<void *> {
        auto *w_ = cast_to_webview(w);
        switch (kind) {
        case WEBVIEW_NATIVE_HANDLE_KIND_UI_WINDOW:
          return w_->window();
        case WEBVIEW_NATIVE_HANDLE_KIND_UI_WIDGET:
          return w_->widget();
        case WEBVIEW_NATIVE_HANDLE_KIND_BROWSER_CONTROLLER:
          return w_->browser_controller();
        default:
          return webview::error_info{WEBVIEW_ERROR_INVALID_ARGUMENT};
        }
      },
      [&](void *handle_) { handle = handle_; });
  if (err == WEBVIEW_ERROR_OK) {
    return handle;
  }
  return nullptr;
}

WEBVIEW_API webview_error_t webview_set_title(webview_t w, const char *title) {
  using namespace webview::detail;
  if (!title) {
    return WEBVIEW_ERROR_INVALID_ARGUMENT;
  }
  return api_filter([=] { return cast_to_webview(w)->set_title(title); });
}

WEBVIEW_API webview_error_t webview_set_size(webview_t w, int width, int height,
                                             webview_hint_t hints) {
  using namespace webview::detail;
  return api_filter(
      [=] { return cast_to_webview(w)->set_size(width, height, hints); });
}

WEBVIEW_API webview_error_t webview_navigate(webview_t w, const char *url) {
  using namespace webview::detail;
  if (!url) {
    return WEBVIEW_ERROR_INVALID_ARGUMENT;
  }
  return api_filter([=] { return cast_to_webview(w)->navigate(url); });
}

WEBVIEW_API webview_error_t webview_set_html(webview_t w, const char *html) {
  using namespace webview::detail;
  if (!html) {
    return WEBVIEW_ERROR_INVALID_ARGUMENT;
  }
  return api_filter([=] { return cast_to_webview(w)->set_html(html); });
}

WEBVIEW_API webview_error_t webview_init(webview_t w, const char *js) {
  using namespace webview::detail;
  if (!js) {
    return WEBVIEW_ERROR_INVALID_ARGUMENT;
  }
  return api_filter([=] { return cast_to_webview(w)->init(js); });
}

WEBVIEW_API webview_error_t webview_eval(webview_t w, const char *js) {
  using namespace webview::detail;
  if (!js) {
    return WEBVIEW_ERROR_INVALID_ARGUMENT;
  }
  return api_filter([=] { return cast_to_webview(w)->eval(js); });
}

WEBVIEW_API webview_error_t webview_bind(webview_t w, const char *name,
                                         void (*fn)(const char *id,
                                                    const char *req, void *arg),
                                         void *arg) {
  using namespace webview::detail;
  if (!name || !fn) {
    return WEBVIEW_ERROR_INVALID_ARGUMENT;
  }
  return api_filter([=] {
    return cast_to_webview(w)->bind(
        name,
        [=](const std::string &seq, const std::string &req, void *arg_) {
          fn(seq.c_str(), req.c_str(), arg_);
        },
        arg);
  });
}

WEBVIEW_API webview_error_t webview_unbind(webview_t w, const char *name) {
  using namespace webview::detail;
  if (!name) {
    return WEBVIEW_ERROR_INVALID_ARGUMENT;
  }
  return api_filter([=] { return cast_to_webview(w)->unbind(name); });
}

WEBVIEW_API webview_error_t webview_return(webview_t w, const char *id,
                                           int status, const char *result) {
  using namespace webview::detail;
  if (!id || !result) {
    return WEBVIEW_ERROR_INVALID_ARGUMENT;
  }
  return api_filter(
      [=] { return cast_to_webview(w)->resolve(id, status, result); });
}

WEBVIEW_API const webview_version_info_t *webview_version(void) {
  return &webview::detail::library_version_info;
}

#endif // defined(__cplusplus) && !defined(WEBVIEW_HEADER)
#endif // WEBVIEW_C_API_IMPL_HH
// file end: include/webview/api/c_api_impl.hh
// file begin: src/detail/_src_detail.hh
#ifndef WEBVIEW_AMALGAMATED_SRC_DETAIL_HH
#define WEBVIEW_AMALGAMATED_SRC_DETAIL_HH

#if defined(__cplusplus) && !defined(WEBVIEW_HEADER)
#endif
#endif
// file end: src/detail/_src_detail.hh
// file begin: include/webview/amalgamate_src.hh
#ifndef WEBVIEW_AMALGAMATED_SRC_HH
#define WEBVIEW_AMALGAMATED_SRC_HH

#if defined(__cplusplus) && !defined(WEBVIEW_HEADER)
#endif
#endif
// file end: include/webview/amalgamate_src.hh
// file begin: include/webview/webview.h
#ifndef WEBVIEW_H
#define WEBVIEW_H
#endif // WEBVIEW_H
// file end: include/webview/webview.h
// file begin: src/webview.cc

// file end: src/webview.cc
