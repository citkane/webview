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

/// Structure to hold information about a queued action.
struct action_t {
  context_t ctx;
  dispatch_fn_t work_fn;
  std::string name_or_js;
};
/// Structure to hold index positions of bind / unbind operations
struct indices_t {
  int bind_i;
  int unbind_i;
};

/// Thread-safe operations on a name map of active bindings
struct bindings_t {
public:
  /// Get the size of bound functions
  size_t size() const;
  /// Get a vector<string> of bound names
  void get_names(std::vector<std::string> &bound_names);
  /// Check if the bindings map has a name
  bool has_name(cnst_str_r name) const;
  /// Emplace a new binding
  void emplace(cnst_str_r name, binding_t fn, void *arg);
  /// Erase a binding
  void erase(cnst_str_r name);
  /// Count how many instances of a name are in the map.
  /// Should only ever be 0 or 1
  size_t count(cnst_str_r name) const;
  //Retrieve a binding for the given name
  binding_ctx_t at(cnst_str_r name) const;

private:
  std::map<std::string, binding_ctx_t> bindings_map;
  std::mutex mutable mtx;
};

/// Thread-safe operations for adding and replacing user functions and scripts
struct user_scripts_t {
public:
  /// Adds a bound user function to Webview native code.
  user_script *add(cnst_str_r js, engine_base *base);
  /// Replaces a bound user script in Webview native code.
  user_script *replace(const user_script &old_script,
                       cnst_str_r new_script_code, engine_base *base);

private:
  /// A list of references to bound user scripts.
  std::list<user_script> m_user_scripts;
  std::mutex mutable mtx;
};

/// Thread-safe operations for the user actions queue
struct queue_t {
public:
  /// Get the size of the queue
  size_t size() const;
  /// Get the front of the queue
  action_t front();
  /// Pop the back of the queue
  void pop_front();
  /// Add a new user action to the back of the queue
  void push_back(context_t ctx, dispatch_fn_t work_fn, std::string name_or_js);
  /// Empty the queue
  void clear();
  /// Check if the queue is empty
  bool empty() const;

private:
  std::deque<action_t> queue;
  std::mutex mutable mtx;
};

/// Thread-safe operations on a map of unresolved promises
struct unres_promises_t {
public:
  /// Create a new list of promise id's for a given binding name
  void set(cnst_str_r name, std::list<std::string> ids);
  /// Get a copy of the list of pending promise id's for the given binding name
  std::list<std::string> get_copy(cnst_str_r name) const;
  /// Remove a pending promise id from the list at the given name;
  void remove_id(cnst_str_r name, cnst_str_r id);
  /// Add a pending promise id to the list at the given name;
  void add_id(cnst_str_r name, cnst_str_r id);
  /// Erase the mapped binding name and it's list
  void erase(cnst_str_r name);
  /// Query if the list of promises for the given name is empty
  bool empty(cnst_str_r name) const;

private:
  std::unordered_map<std::string, std::list<std::string>> unres_promises;
  std::mutex mutable mtx;
};

/// Thread-safe operations on a map of promise id's to binding name.
/// Used to retrieve the binding name of a promise when it is resolved.
struct id_name_map_t {
public:
  /// Get the binding name for a given promise id
  std::string get(cnst_str_r id) const;
  /// Set the binding name for a given promise id
  void set(cnst_str_r id, cnst_str_r name);
  /// Erase the map entry for the given promise
  void erase(cnst_str_r id);

private:
  std::unordered_map<std::string, std::string> id_name;
  std::mutex mutable mtx;
};

/// Thread-safe operations on an ordered list of pending bind / unbind operations in the queue.
/// Used to accurately predict if a name will be bound / unbound in the future.
struct pending_t {
public:
  /// Pop the front of the queue of pending bind / unbinds
  void pop_front();
  /// Add a pending bind / unbind to the back of the queue
  void push_back(cnst_str_r name);
  /// Get the queue inices of pending bind / unbind operations for the given binding name
  indices_t indices(cnst_str_r name) const;

private:
  std::deque<std::string> pending_bind_unbind;
  std::mutex mutable mtx;
};
} // namespace _lib

class engine_lists_t {
  /// Condition variables for various stages in the event loop lifecycle
  struct cv_api_t {
    cv_api_t();
    /// Controls the main queue loop
    std::condition_variable queue;
    /// Controls the bind section of the event loop
    std::condition_variable bind;
    /// Controls the eval section of the event loop
    std::condition_variable eval;
    /// Controls the unbind section of the event loop
    std::condition_variable unbind;
    /// Controls the unbind timeout section of the event loop
    std::condition_variable unbind_timeout;
    /// An array of all the event loop condition variables
    std::condition_variable *all[5];
    /// Calls `notify_all` on all event loop condition variables
    void notify_all();
  };
  /// The root API for working with thread-safe list like containers.
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