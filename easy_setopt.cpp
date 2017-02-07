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
    template <class T>
    void setopt_integer(lua_State* L, CURLoption option) {
      CURLcode result = curl_easy_setopt(check_easy(L, 1), option, luaX_check_integer<T>(L, 3));
      if (result == CURLE_OK) {
        luaX_push_success(L);
      } else {
        push_error(L, result);
      }
    }

    void setopt_string(lua_State* L, CURLoption option) {
      CURLcode result = curl_easy_setopt(check_easy(L, 1), option, luaL_checkstring(L, 3));
      if (result == CURLE_OK) {
        luaX_push_success(L);
      } else {
        push_error(L, result);
      }
    }

    void impl_setopt(lua_State* L) {
      CURLoption option = luaX_check_enum<CURLoption>(L, 2);
      switch (option) {
        case CURLOPT_VERBOSE:
        case CURLOPT_NOPROGRESS:
        case CURLOPT_FILETIME:
        case CURLOPT_SSL_VERIFYPEER:
        case CURLOPT_FOLLOWLOCATION:
          setopt_integer<long>(L, option);
          return;
        case CURLOPT_URL:
        case CURLOPT_USERAGENT:
        case CURLOPT_REFERER:
          setopt_string(L, option);
          return;
        default:
          push_error(L, CURLE_UNKNOWN_OPTION);
      }
    }

    size_t write_callback(char* ptr, size_t size, size_t nmemb, void* userdata) {
      luaX_reference& ref = *static_cast<luaX_reference*>(userdata);
      lua_State* L = ref.state();
      int top = lua_gettop(L);
      ref.get_field();
      lua_pushlstring(L, ptr, size * nmemb);
      size_t result = 0;
      int r = lua_pcall(L, 1, 1, 0);
      if (r == 0) {
        if (luaX_is_integer(L, -1)) {
          result = lua_tointeger(L, -1);
        } else {
          result = size * nmemb;
        }
      } else {
        DROMOZOA_UNEXPECTED(lua_tostring(L, -1));
      }
      lua_settop(L, top);
      return result;
    }

    void impl_setopt_write_function(lua_State* L) {
      easy_handle* self = check_easy_handle(L, 1);
      luaX_reference& ref = self->write_function();
      lua_pushvalue(L, 2);
      luaX_reference(L).swap(ref);
      curl_easy_setopt(self->get(), CURLOPT_WRITEDATA, &ref);
      CURLcode result = curl_easy_setopt(self->get(), CURLOPT_WRITEFUNCTION, &write_callback);
      if (result == CURLE_OK) {
        luaX_push_success(L);
      } else {
        push_error(L, result);
      }
    }

    size_t header_callback(char* ptr, size_t size, size_t nmemb, void* userdata) {
      luaX_reference& ref = *static_cast<luaX_reference*>(userdata);
      lua_State* L = ref.state();
      int top = lua_gettop(L);
      ref.get_field();
      lua_pushlstring(L, ptr, size * nmemb);
      size_t result = 0;
      int r = lua_pcall(L, 1, 1, 0);
      if (r == 0) {
        if (luaX_is_integer(L, -1)) {
          result = lua_tointeger(L, -1);
        } else {
          result = size * nmemb;
        }
      } else {
        DROMOZOA_UNEXPECTED(lua_tostring(L, -1));
      }
      lua_settop(L, top);
      return result;
    }

    void impl_setopt_header_function(lua_State* L) {
      easy_handle* self = check_easy_handle(L, 1);
      luaX_reference& ref = self->header_function();
      lua_pushvalue(L, 2);
      luaX_reference(L).swap(ref);
      curl_easy_setopt(self->get(), CURLOPT_HEADERDATA, &ref);
      CURLcode result = curl_easy_setopt(self->get(), CURLOPT_HEADERFUNCTION, &header_callback);
      if (result == CURLE_OK) {
        luaX_push_success(L);
      } else {
        push_error(L, result);
      }
    }
  }

  void initialize_easy_setopt(lua_State* L) {
    luaX_set_field(L, -1, "setopt", impl_setopt);
    luaX_set_field(L, -1, "setopt_write_function", impl_setopt_write_function);
    luaX_set_field(L, -1, "setopt_header_function", impl_setopt_header_function);
  }
}
