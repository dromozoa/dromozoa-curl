// Copyright (C) 2017 Tomoyuki Fujimori <moyu@dromozoa.com>
//
// This file is part of dromozoa-curl.
//
// dromozoa-curl is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// dromozoa-curl is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with dromozoa-curl.  If not, see <http://www.gnu.org/licenses/>.

#include "common.hpp"

namespace dromozoa {
  namespace {
    CURLM* check_multi(lua_State* L, int arg) {
      return check_multi_handle(L, arg)->get();
    }

    void impl_gc(lua_State* L) {
      check_multi_handle(L, 1)->~multi_handle();
    }

    void impl_call(lua_State* L) {
      if (CURLM* handle = curl_multi_init()) {
        luaX_new<multi_handle>(L, handle);
        luaX_set_metatable(L, "dromozoa.curl.multi");
      } else {
        push_error(L, CURLE_FAILED_INIT);
      }
    }

    void impl_cleanup(lua_State* L) {
      CURLMcode result = check_multi_handle(L, 1)->cleanup();
      if (result == CURLM_OK) {
        luaX_push_success(L);
      } else {
        push_error(L, result);
      }
    }

    void impl_add_handle(lua_State* L) {
      CURLMcode result = curl_multi_add_handle(check_multi(L, 1), check_easy(L, 2));
      if (result == CURLM_OK) {
        luaX_push_success(L);
      } else {
        push_error(L, result);
      }
    }

    void impl_socket_action(lua_State* L) {
      curl_socket_t sockfd = luaX_check_integer<curl_socket_t>(L, 2);
      int ev_bitmask = luaX_opt_integer<int>(L, 3, 0);
      int running_handles = 0;
      CURLMcode result = curl_multi_socket_action(check_multi(L, 1), sockfd, ev_bitmask, &running_handles);
      if (result == CURLM_OK) {
        luaX_push(L, running_handles);
      } else {
        push_error(L, result);
      }
    }

    void impl_remove_handle(lua_State* L) {
      CURLMcode result = curl_multi_remove_handle(check_multi(L, 1), check_easy(L, 2));
      if (result == CURLM_OK) {
        luaX_push_success(L);
      } else {
        push_error(L, result);
      }
    }

    int socket_callback(CURL* easy, curl_socket_t s, int what, void* userdata, void*) {
      luaX_reference& ref = *static_cast<luaX_reference*>(userdata);
      lua_State* L = ref.state();
      int top = lua_gettop(L);
      ref.get_field();
      new_easy_ref(L, easy);
      luaX_push(L, s);
      luaX_push(L, what);
      int result = 0;
      int r = lua_pcall(L, 3, 1, 0);
      if (r == 0) {
        if (luaX_is_integer(L, -1)) {
          result = lua_tointeger(L, -1);
        }
      } else {
        DROMOZOA_UNEXPECTED(lua_tostring(L, 1));
      }
      lua_settop(L, top);
      return result;
    }

    void impl_setopt_socket_function(lua_State* L) {
      multi_handle* self = check_multi_handle(L, 1);
      luaX_reference& ref = self->socket_function();
      lua_pushvalue(L, 2);
      luaX_reference(L).swap(ref);
      curl_multi_setopt(self->get(), CURLMOPT_SOCKETDATA, &ref);
      CURLMcode result = curl_multi_setopt(self->get(), CURLMOPT_SOCKETFUNCTION, &socket_callback);
      if (result == CURLM_OK) {
        luaX_push_success(L);
      } else {
        push_error(L, result);
      }
    }

    int timer_callback(CURLM* multi, long timeout_ms, void* userdata) {
      luaX_reference& ref = *static_cast<luaX_reference*>(userdata);
      lua_State* L = ref.state();
      int top = lua_gettop(L);
      ref.get_field();
      new_multi_ref(L, multi);
      luaX_push(L, timeout_ms);
      int result = 0;
      int r = lua_pcall(L, 2, 1, 0);
      if (r == 0) {
        if (luaX_is_integer(L, -1)) {
          result = lua_tointeger(L, -1);
        }
      } else {
        DROMOZOA_UNEXPECTED(lua_tostring(L, 1));
      }
      lua_settop(L, top);
      return result;
    }

    void impl_setopt_timer_function(lua_State* L) {
      multi_handle* self = check_multi_handle(L, 1);
      luaX_reference& ref = self->timer_function();
      lua_pushvalue(L, 2);
      luaX_reference(L).swap(ref);
      curl_multi_setopt(self->get(), CURLMOPT_TIMERDATA, &ref);
      CURLMcode result = curl_multi_setopt(self->get(), CURLMOPT_TIMERFUNCTION, &timer_callback);
      if (result == CURLM_OK) {
        luaX_push_success(L);
      } else {
        push_error(L, result);
      }
    }
  }

  multi_handle* check_multi_handle(lua_State* L, int arg) {
    return luaX_check_udata<multi_handle>(L, arg, "dromozoa.curl.multi_ref", "dromozoa.curl.multi");
  }

  void new_multi_ref(lua_State* L, CURLM* handle) {
    luaX_new<multi_handle>(L, handle);
    luaX_set_metatable(L, "dromozoa.curl.multi_ref");
  }

  void initialize_multi(lua_State* L) {
    lua_newtable(L);
    {
      luaL_newmetatable(L, "dromozoa.curl.multi_ref");
      lua_pushvalue(L, -2);
      luaX_set_field(L, -2, "__index");
      lua_pop(L, 1);

      luaL_newmetatable(L, "dromozoa.curl.multi");
      lua_pushvalue(L, -2);
      luaX_set_field(L, -2, "__index");
      luaX_set_field(L, -1, "__gc", impl_gc);
      lua_pop(L, 1);

      luaX_set_metafield(L, -1, "__call", impl_call);
      luaX_set_field(L, -1, "cleanup", impl_cleanup);
      luaX_set_field(L, -1, "add_handle", impl_add_handle);
      luaX_set_field(L, -1, "remove_handle", impl_remove_handle);
      luaX_set_field(L, -1, "socket_action", impl_socket_action);
      luaX_set_field(L, -1, "setopt_socket_function", impl_setopt_socket_function);
      luaX_set_field(L, -1, "setopt_timer_function", impl_setopt_timer_function);
    }
    luaX_set_field(L, -2, "multi");
  }
}
