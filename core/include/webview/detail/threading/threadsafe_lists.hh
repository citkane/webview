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

#ifndef WEBVIEW_DETAIL_THREADSAFE_LISTS_HH
#define WEBVIEW_DETAIL_THREADSAFE_LISTS_HH

#if defined(__cplusplus) && !defined(WEBVIEW_HEADER)
#include "webview/detail/user/user_script.hh"
#include "webview/types/types.hh"
#include <condition_variable>
#include <deque>
#include <list>
#include <map>
#include <mutex>
#include <string>
#include <unordered_map>
#include <vector>

using namespace webview::types;
using namespace webview::detail::user;
namespace webview {
namespace detail {

class engine_base;

namespace threading {
namespace _lib {

struct action_t {
  context_t ctx;
  dispatch_fn_t work_fn;
  std::string name_or_js;
};
struct indices_t {
  int bind_i;
  int unbind_i;
};

struct bindings_t {
public:
  size_t size() const;
  void get_names(std::vector<std::string> &bound_names);
  bool has_name(const_str_ref name) const;
  void emplace(const_str_ref name, binding_t fn, void *arg);
  void erase(const_str_ref name);
  size_t count(const_str_ref name) const;
  binding_ctx_t at(const_str_ref name) const;

private:
  std::map<std::string, binding_ctx_t> bindings_map;
  std::mutex mutable mtx;
};

struct user_scripts_t {
public:
  /// Adds a bound user function to Webview native code.
  user_script *add(const_str_ref js, engine_base *base);
  /// Replaces a bound user script in Webview native code.
  user_script *replace(const user_script &old_script,
                       const_str_ref new_script_code, engine_base *base);

private:
  /// A list of references to bound user scripts.
  std::list<user_script> m_user_scripts;
  std::mutex mutable mtx;
};

struct queue_t {
public:
  size_t size() const;
  action_t front();
  void pop_front();
  void push_back(context_t ctx, dispatch_fn_t work_fn, std::string name_or_js);
  void clear();
  bool empty() const;

private:
  std::deque<action_t> queue;
  std::mutex mutable mtx;
};

struct unres_promises_t {
public:
  void set(const_str_ref name, std::list<std::string> ids);
  std::list<std::string> get_copy(const_str_ref name) const;
  void remove_id(const_str_ref name, const_str_ref id);
  void add_id(const_str_ref name, const_str_ref id);
  void erase(const_str_ref name);
  bool empty(const_str_ref name) const;

private:
  std::unordered_map<std::string, std::list<std::string>> unres_promises;
  std::mutex mutable mtx;
};

struct id_name_map_t {
public:
  std::string get(const_str_ref id) const;
  void set(const_str_ref id, const_str_ref name);
  void erase(const_str_ref id);

private:
  std::unordered_map<std::string, std::string> id_name;
  std::mutex mutable mtx;
};

struct pending_t {
public:
  void pop_front();
  void push_back(const_str_ref name);
  indices_t indices(const_str_ref name) const;

private:
  std::deque<std::string> pending_bind_unbind;
  std::mutex mutable mtx;
};
} // namespace _lib

class engine_lists_t {
  struct cv_api_t {
    cv_api_t();
    std::condition_variable queue;
    std::condition_variable bind;
    std::condition_variable eval;
    std::condition_variable unbind;
    std::condition_variable unbind_timeout;
    std::condition_variable *all[5];
    void notify_all();
  };
  struct list_t {
    /// Thread safe wrappers for the `std::map` of bindings.
    _lib::bindings_t bindings{};
    /// Thread safe wrappers for the `std::list` of user scripts.
    _lib::user_scripts_t m_user_scripts{};
    /// Thread safe wrappers for the `std::deque` of user actions.
    _lib::queue_t queue{};
    /// Thread safe wrappers for the `std::unordered_map` of unresolved promises.
    _lib::unres_promises_t unresolved_promises{};
    /// Thread safe wrappers for the `std::unordered_map` of promise ids to bound names.
    _lib::id_name_map_t id_name_map{};
    /// Thread safe wrappers for the `std::deque` of pending bind / unbind names
    _lib::pending_t pending{};
  };

protected:
  /// Grouping of condition variables
  cv_api_t cv{};
  /// Thread safe wrappers for list-like objects.
  list_t list{};
};

} // namespace threading
} // namespace detail
} // namespace webview

#endif // defined(__cplusplus) && !defined(WEBVIEW_HEADER)
#endif // WEBVIEW_DETAIL_THREADSAFE_LISTS_HH