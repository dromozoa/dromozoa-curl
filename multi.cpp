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

    void impl_remove_handle(lua_State* L) {
      CURLMcode result = curl_multi_remove_handle(check_multi(L, 1), check_easy(L, 2));
      if (result == CURLM_OK) {
        luaX_push_success(L);
      } else {
        push_error(L, result);
      }
    }
  }

  multi_handle* check_multi_handle(lua_State* L, int arg) {
    return luaX_check_udata<multi_handle>(L, arg, "dromozoa.curl.multi");
  }

  void initialize_multi(lua_State* L) {
    lua_newtable(L);
    {
      luaL_newmetatable(L, "dromozoa.curl.multi");
      lua_pushvalue(L, -2);
      luaX_set_field(L, -2, "__index");
      luaX_set_field(L, -1, "__gc", impl_gc);
      lua_pop(L, 1);

      luaX_set_metafield(L, -1, "__call", impl_call);
      luaX_set_field(L, -1, "cleanup", impl_cleanup);
      luaX_set_field(L, -1, "add_handle", impl_add_handle);
      luaX_set_field(L, -1, "remove_handle", impl_remove_handle);
    }
    luaX_set_field(L, -2, "multi");
  }
}
