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
    void impl_gc(lua_State* L) {
      check_easy_handle(L, 1)->~easy_handle();
    }

    void impl_call(lua_State* L) {
      if (CURL* handle = curl_easy_init()) {
        luaX_new<easy_handle>(L, handle);
        luaX_set_metatable(L, "dromozoa.curl.easy");
      } else {
        push_error(L, CURLE_FAILED_INIT);
      }
    }

    void impl_cleanup(lua_State* L) {
      check_easy_handle(L, 1)->cleanup();
      luaX_push_success(L);
    }

    void impl_reset(lua_State* L) {
      curl_easy_reset(check_easy(L, 1));
      luaX_push_success(L);
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
        case CURLOPT_FILETIME:
        case CURLOPT_SSL_VERIFYPEER:
          setopt_integer<long>(L, option);
          return;
        case CURLOPT_URL:
        case CURLOPT_USERAGENT:
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

    void impl_perform(lua_State* L) {
      CURLcode result = curl_easy_perform(check_easy(L, 1));
      if (result == CURLE_OK) {
        luaX_push_success(L);
      } else {
        push_error(L, result);
      }
    }

    template <class T>
    void getinfo(lua_State* L, CURLINFO info) {
      T value = 0;
      CURLcode result = curl_easy_getinfo(check_easy(L, 1), info, &value);
      if (result == CURLE_OK) {
        luaX_push(L, value);
      } else {
        push_error(L, result);
      }
    }

    void getinfo_slist(lua_State* L, CURLINFO info) {
      struct curl_slist* slist = 0;
      CURLcode result = curl_easy_getinfo(check_easy(L, 1), info, &slist);
      if (result == CURLE_OK) {
        struct curl_slist* head = slist;
        lua_newtable(L);
        for (int i = 1; head; ++i) {
          luaX_set_field(L, -1, i, head->data);
          head = head->next;
        }
        curl_slist_free_all(slist);
      } else {
        push_error(L, result);
      }
    }

    void impl_getinfo(lua_State* L) {
      CURLINFO info = luaX_check_enum<CURLINFO>(L, 2);
      switch (info) {
        case CURLINFO_RESPONSE_CODE:
        case CURLINFO_FILETIME:
        case CURLINFO_REDIRECT_COUNT:
        case CURLINFO_HEADER_SIZE:
        case CURLINFO_REQUEST_SIZE:
        case CURLINFO_SSL_VERIFYRESULT:
        case CURLINFO_HTTPAUTH_AVAIL:
        case CURLINFO_PROXYAUTH_AVAIL:
        case CURLINFO_OS_ERRNO:
        case CURLINFO_NUM_CONNECTS:
        case CURLINFO_PRIMARY_PORT:
        case CURLINFO_LOCAL_PORT:
          getinfo<long>(L, info);
          return;
        case CURLINFO_TOTAL_TIME:
        case CURLINFO_NAMELOOKUP_TIME:
        case CURLINFO_CONNECT_TIME:
        case CURLINFO_APPCONNECT_TIME:
        case CURLINFO_PRETRANSFER_TIME:
        case CURLINFO_STARTTRANSFER_TIME:
        case CURLINFO_REDIRECT_TIME:
        case CURLINFO_SIZE_UPLOAD:
        case CURLINFO_SIZE_DOWNLOAD:
        case CURLINFO_SPEED_DOWNLOAD:
        case CURLINFO_SPEED_UPLOAD:
        case CURLINFO_CONTENT_LENGTH_DOWNLOAD:
        case CURLINFO_CONTENT_LENGTH_UPLOAD:
          getinfo<double>(L, info);
          return;
        case CURLINFO_EFFECTIVE_URL:
        case CURLINFO_REDIRECT_URL:
        case CURLINFO_CONTENT_TYPE:
        case CURLINFO_PRIMARY_IP:
        case CURLINFO_LOCAL_IP:
          getinfo<const char*>(L, info);
          return;
        case CURLINFO_SSL_ENGINES:
        case CURLINFO_COOKIELIST:
          getinfo_slist(L, info);
          return;
        default:
          push_error(L, CURLE_UNKNOWN_OPTION);
      }
    }
  }

  easy_handle* check_easy_handle(lua_State* L, int arg) {
    return luaX_check_udata<easy_handle>(L, arg, "dromozoa.curl.easy_ref", "dromozoa.curl.easy");
  }

  CURL* check_easy(lua_State* L, int arg) {
    return check_easy_handle(L, arg)->get();
  }

  void new_easy_ref(lua_State* L, CURL* handle) {
    luaX_new<easy_handle>(L, handle);
    luaX_set_metatable(L, "dromozoa.curl.easy_ref");
  }

  void initialize_easy(lua_State* L) {
    lua_newtable(L);
    {
      luaL_newmetatable(L, "dromozoa.curl.easy_ref");
      lua_pushvalue(L, -2);
      luaX_set_field(L, -2, "__index");
      lua_pop(L, 1);

      luaL_newmetatable(L, "dromozoa.curl.easy");
      lua_pushvalue(L, -2);
      luaX_set_field(L, -2, "__index");
      luaX_set_field(L, -1, "__gc", impl_gc);
      lua_pop(L, 1);

      luaX_set_metafield(L, -1, "__call", impl_call);
      luaX_set_field(L, -1, "cleanup", impl_cleanup);
      luaX_set_field(L, -1, "reset", impl_reset);
      luaX_set_field(L, -1, "setopt", impl_setopt);
      luaX_set_field(L, -1, "setopt_write_function", impl_setopt_write_function);
      luaX_set_field(L, -1, "setopt_header_function", impl_setopt_header_function);
      luaX_set_field(L, -1, "perform", impl_perform);
      luaX_set_field(L, -1, "getinfo", impl_getinfo);
    }
    luaX_set_field(L, -2, "easy");
  }
}
