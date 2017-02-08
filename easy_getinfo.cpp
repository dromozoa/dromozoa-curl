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
      struct curl_slist* list = 0;
      CURLcode result = curl_easy_getinfo(check_easy(L, 1), info, &list);
      if (result == CURLE_OK) {
        struct curl_slist* item = list;
        lua_newtable(L);
        for (int i = 1; item; ++i) {
          luaX_set_field(L, -1, i, item->data);
          item = item->next;
        }
        curl_slist_free_all(list);
      } else {
        push_error(L, result);
      }
    }

    void impl_getinfo(lua_State* L) {
      CURLINFO info = luaX_check_enum<CURLINFO>(L, 2);
      switch (info) {
        case CURLINFO_RESPONSE_CODE:
        case CURLINFO_HTTP_CONNECTCODE:
#if CURL_AT_LEAST_VERSION(7,50,0)
        case CURLINFO_HTTP_VERSION:
#endif
        case CURLINFO_FILETIME:
        case CURLINFO_REDIRECT_COUNT:
        case CURLINFO_HEADER_SIZE:
        case CURLINFO_REQUEST_SIZE:
        case CURLINFO_SSL_VERIFYRESULT:
#if CURL_AT_LEAST_VERSION(7,52,0)
        case CURLINFO_PROXY_SSL_VERIFYRESULT:
#endif
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

  void initialize_easy_getinfo(lua_State* L) {
    luaX_set_field(L, -1, "getinfo", impl_getinfo);
  }
}
