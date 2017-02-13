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
#include "symbols.hpp"

namespace dromozoa {
  namespace {
    void setopt_string(lua_State* L, CURLoption option) {
      CURLcode result = curl_easy_setopt(check_easy(L, 1), option, luaL_checkstring(L, 3));
      if (result == CURLE_OK) {
        luaX_push_success(L);
      } else {
        push_error(L, result);
      }
    }

    template <class T>
    void setopt_integer(lua_State* L, CURLoption option) {
      CURLcode result = curl_easy_setopt(check_easy(L, 1), option, luaX_check_integer<T>(L, 3));
      if (result == CURLE_OK) {
        luaX_push_success(L);
      } else {
        push_error(L, result);
      }
    }

    void setopt_slist(lua_State* L, CURLoption option) {
      if (lua_istable(L, 3)) {
        string_list list;
        for (int i = 1; ; ++i) {
          luaX_get_field(L, 3, i);
          if (const char* p = lua_tostring(L, -1)) {
            list.append(p);
            lua_pop(L, 1);
          } else {
            lua_pop(L, 1);
            break;
          }
        }
        easy_handle* self = check_easy_handle(L, 1);
        self->set_slist(option, list.get());
        CURLcode result = curl_easy_setopt(self->get(), option, list.release());
        if (result == CURLE_OK) {
          luaX_push_success(L);
        } else {
          push_error(L, result);
        }
      } else {
        push_error(L, CURLE_UNKNOWN_OPTION);
      }
    }

    void impl_setopt(lua_State* L) {
      CURLoption option = luaX_check_enum<CURLoption>(L, 2);
      switch (easy_setopt_param(option)) {
        case easy_setopt_param_char_p:
          switch (option) {
            case CURLOPT_POSTFIELDS:
              setopt_string(L, CURLOPT_COPYPOSTFIELDS);
              return;
            default:
              setopt_string(L, option);
              return;
          }
          return;
        case easy_setopt_param_long:
          setopt_integer<long>(L, option);
          return;
        case easy_setopt_param_curl_off_t:
          setopt_integer<curl_off_t>(L, option);
          return;
        case easy_setopt_param_struct_curl_slist_p:
          setopt_slist(L, option);
          return;
        default:
          push_error(L, CURLE_UNKNOWN_OPTION);
          return;
      }
    }

    template <class T>
    void setopt_function(lua_State* L, easy_handle* self, luaX_reference& ref, CURLoption option_data, CURLoption option_function, const T& callback) {
      lua_pushvalue(L, 2);
      luaX_reference(L).swap(ref);
      curl_easy_setopt(self->get(), option_data, &ref);
      CURLcode result = curl_easy_setopt(self->get(), option_function, callback);
      if (result == CURLE_OK) {
        luaX_push_success(L);
      } else {
        push_error(L, result);
      }
    }

    size_t write_callback(char* ptr, size_t size, size_t nmemb, void* userdata) {
      size_t n = size * nmemb;
      luaX_reference& ref = *static_cast<luaX_reference*>(userdata);
      lua_State* L = ref.state();
      int top = lua_gettop(L);
      ref.get_field();
      lua_pushlstring(L, ptr, n);
      size_t result = 0;
      int r = lua_pcall(L, 1, 1, 0);
      if (r == 0) {
        if (luaX_is_integer(L, -1)) {
          result = lua_tointeger(L, -1);
        } else {
          result = n;
        }
      } else {
        DROMOZOA_UNEXPECTED(lua_tostring(L, -1));
      }
      lua_settop(L, top);
      return result;
    }

    void impl_setopt_write_function(lua_State* L) {
      easy_handle* self = check_easy_handle(L, 1);
      setopt_function(L, self, self->write_function(), CURLOPT_WRITEDATA, CURLOPT_WRITEFUNCTION, &write_callback);
    }

    size_t read_callback(char* buffer, size_t size, size_t nmemb, void* userdata) {
      size_t n = size * nmemb;
      luaX_reference& ref = *static_cast<luaX_reference*>(userdata);
      lua_State* L = ref.state();
      int top = lua_gettop(L);
      ref.get_field();
      luaX_push(L, n);
      size_t result = CURLE_ABORTED_BY_CALLBACK;
      int r = lua_pcall(L, 1, 1, 0);
      if (r == 0) {
        result = 0;
        if (const char* data = lua_tolstring(L, -1, &result)) {
          if (result > n) {
            result = n;
          }
          memcpy(buffer, data, result);
        }
      } else {
        DROMOZOA_UNEXPECTED(lua_tostring(L, -1));
      }
      lua_settop(L, top);
      return result;
    }

    void impl_setopt_read_function(lua_State* L) {
      easy_handle* self = check_easy_handle(L, 1);
      setopt_function(L, self, self->read_function(), CURLOPT_READDATA, CURLOPT_READFUNCTION, &read_callback);
    }

    void impl_setopt_header_function(lua_State* L) {
      easy_handle* self = check_easy_handle(L, 1);
      setopt_function(L, self, self->header_function(), CURLOPT_HEADERDATA, CURLOPT_HEADERFUNCTION, &write_callback);
    }
  }

  void initialize_easy_setopt(lua_State* L) {
    luaX_set_field(L, -1, "setopt", impl_setopt);
    luaX_set_field(L, -1, "setopt_write_function", impl_setopt_write_function);
    luaX_set_field(L, -1, "setopt_read_function", impl_setopt_read_function);
    luaX_set_field(L, -1, "setopt_header_function", impl_setopt_header_function);
  }
}
