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

#ifndef WEBVIEW_DETAIL_ENGINE_LISTS_HH
#define WEBVIEW_DETAIL_ENGINE_LISTS_HH

#if defined(__cplusplus) && !defined(WEBVIEW_HEADER)
#include "user_script.hh"
#include "webview/log/trace_log.hh"
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
using str_arg_t = const std::string &;
using do_work_t = std::function<void()>;
using str_arg_t = const std::string &;

class engine_base;
struct cv_api_t {
  std::condition_variable mutable queue;
  std::condition_variable mutable bind;
  std::condition_variable mutable eval;
  std::condition_variable mutable unbind;
  std::condition_variable mutable unbind_timeout;
  std::condition_variable *all[5] = {&queue, &bind, &eval, &unbind,
                                     &unbind_timeout};
  void notify_all() const;
};
enum context_t { bind_t = 'b', unbind_t = 'u', eval_t = 'e' };
struct action_ctx_t {
  context_t bind = context_t::bind_t;
  context_t unbind = context_t::unbind_t;
  context_t eval = context_t::eval_t;
};
struct action_t {
  context_t ctx;
  do_work_t work_fn;
  std::string name_or_js;
};
struct indices_t {
  int bind_i;
  int unbind_i;
};

struct bindings_t {
public:
  size_t size() const;
  void get_names(std::vector<std::string> &bound_names) const;
  bool has_name(str_arg_t name) const;
  void emplace(str_arg_t name, binding_t fn, void *arg);
  void erase(str_arg_t name);
  size_t count(str_arg_t name) const;
  binding_ctx_t &at(str_arg_t name) const;

private:
  mutable std::map<std::string, binding_ctx_t> bindings_map;
  mutable std::mutex mtx;
};

struct user_scripts_t {
public:
  /// Adds a bound user function to Webview native code.
  user_script *add(str_arg_t js, engine_base *base) const;
  /// Replaces a bound user script in Webview native code.
  user_script *replace(const user_script &old_script, str_arg_t new_script_code,
                       engine_base *base) const;

private:
  /// A list of references to bound user scripts.
  mutable std::list<user_script> m_user_scripts;
  mutable std::mutex mtx;
};

struct queue_t {
public:
  size_t size() const;
  action_t &front() const;
  void pop_front();
  void push_back(context_t ctx, do_work_t work_fn, std::string name_or_js);
  void clear();
  bool empty();

private:
  mutable std::deque<action_t> queue;
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
  mutable std::unordered_map<std::string, std::list<std::string>>
      unres_promises;
  mutable std::mutex mtx;
};

struct id_name_map_t {
public:
  std::string get(str_arg_t id) const;
  void set(str_arg_t id, str_arg_t name);
  void erase(str_arg_t id);

private:
  mutable std::unordered_map<std::string, std::string> id_name;
  mutable std::mutex mtx;
};

struct pending_t {
public:
  void pop_front();
  void push_back(str_arg_t name);
  indices_t indices(str_arg_t name) const;

private:
  mutable std::deque<std::string> pending_bind_unbind;
  mutable std::mutex mtx;
};

class engine_lists_t {
  struct list_t {
    /// Thread safe wrappers for the `std::map` of bindings.
    bindings_t bindings{};
    /// Thread safe wrappers for the `std::list` of user scripts.
    user_scripts_t m_user_scripts{};
    /// Thread safe wrappers for the `std::deque` of user actions.
    queue_t queue{};
    /// Thread safe wrappers for the `std::unordered_map` of unresolved promises.
    unres_promises_t unresolved_promises{};
    /// Thread safe wrappers for the `std::unordered_map` of promise ids to bound names.
    id_name_map_t id_name_map{};
    /// Thread safe wrappers for the `std::deque` of pending bind / unbind names
    pending_t pending{};
  };

protected:
  /// Grouping of condition variables
  cv_api_t cv{};
  /// Thread safe wrappers for list-like objects.
  list_t list{};
};

} // namespace detail
} // namespace webview

#endif // defined(__cplusplus) && !defined(WEBVIEW_HEADER)
#endif // WEBVIEW_DETAIL_ENGINE_LISTS_HH