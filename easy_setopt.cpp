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

    void setopt_slist(lua_State* L, CURLoption option, string_list& list) {
      if (lua_istable(L, 2)) {
        string_list().swap(list);
        for (int i = 1; ; ++i) {
          luaX_get_field(L, 2, i);
          if (const char* p = lua_tostring(L, -1)) {
            list.append(p);
            lua_pop(L, 1);
          } else {
            lua_pop(L, 1);
            break;
          }
        }
        CURLcode result = curl_easy_setopt(check_easy(L, 1), option, list.get());
        if (result == CURLE_OK) {
          luaX_push_success(L);
        } else {
          push_error(L, result);
        }
      }
    }

    void impl_setopt(lua_State* L) {
      CURLoption option = luaX_check_enum<CURLoption>(L, 2);
      switch (option) {
        case CURLOPT_VERBOSE:
        case CURLOPT_HEADER:
        case CURLOPT_NOPROGRESS:
        case CURLOPT_NOSIGNAL:
        case CURLOPT_FAILONERROR:
#if CURL_AT_LEAST_VERSION(7,51,0)
        case CURLOPT_KEEP_SENDING_ON_ERROR:
#endif
        case CURLOPT_PATH_AS_IS:
        case CURLOPT_PROTOCOLS:
        case CURLOPT_REDIR_PROTOCOLS:
        case CURLOPT_PROXYPORT:
        case CURLOPT_PROXYTYPE:
        case CURLOPT_HTTPPROXYTUNNEL:
        case CURLOPT_SOCKS5_GSSAPI_NEC:
        case CURLOPT_LOCALPORT:
        case CURLOPT_LOCALPORTRANGE:
        case CURLOPT_DNS_CACHE_TIMEOUT:
        case CURLOPT_DNS_USE_GLOBAL_CACHE:
        case CURLOPT_BUFFERSIZE:
        case CURLOPT_PORT:
#if CURL_AT_LEAST_VERSION(7,49,0)
        case CURLOPT_TCP_FASTOPEN:
#endif
        case CURLOPT_TCP_NODELAY:
        case CURLOPT_ADDRESS_SCOPE:
        case CURLOPT_TCP_KEEPALIVE:
        case CURLOPT_TCP_KEEPIDLE:
        case CURLOPT_TCP_KEEPINTVL:
        case CURLOPT_NETRC:
        case CURLOPT_HTTPAUTH:
        case CURLOPT_PROXYAUTH:
        case CURLOPT_SASL_IR:
        case CURLOPT_AUTOREFERER:
        case CURLOPT_FOLLOWLOCATION:
        case CURLOPT_UNRESTRICTED_AUTH:
        case CURLOPT_MAXREDIRS:
        case CURLOPT_POSTREDIR:
        case CURLOPT_PUT:
        case CURLOPT_POST:
        case CURLOPT_POSTFIELDSIZE:
        case CURLOPT_HEADEROPT:
        case CURLOPT_COOKIESESSION:
        case CURLOPT_HTTPGET:
        case CURLOPT_HTTP_VERSION:
        case CURLOPT_IGNORE_CONTENT_LENGTH:
        case CURLOPT_HTTP_CONTENT_DECODING:
        case CURLOPT_HTTP_TRANSFER_DECODING:
        case CURLOPT_EXPECT_100_TIMEOUT_MS:
        case CURLOPT_PIPEWAIT:
#if CURL_AT_LEAST_VERSION(7,46,0)
        case CURLOPT_STREAM_WEIGHT:
#endif

        case CURLOPT_FILETIME:
        case CURLOPT_SSL_VERIFYPEER:
        case CURLOPT_CERTINFO:
        case CURLOPT_UPLOAD:
          setopt_integer<long>(L, option);
          return;

        case CURLOPT_POSTFIELDSIZE_LARGE:
          setopt_integer<curl_off_t>(L, option);
          return;

        case CURLOPT_URL:
#if CURL_AT_LEAST_VERSION(7,45,0)
        case CURLOPT_DEFAULT_PROTOCOL:
#endif
        case CURLOPT_PROXY:
#if CURL_AT_LEAST_VERSION(7,52,0)
        case CURLOPT_PRE_PROXY:
#endif
        case CURLOPT_NOPROXY:
        case CURLOPT_SOCKS5_GSSAPI_SERVICE:
        case CURLOPT_PROXY_SERVICE_NAME:
        case CURLOPT_SERVICE_NAME:
        case CURLOPT_INTERFACE:
        case CURLOPT_UNIX_SOCKET_PATH:
#if CURL_AT_LEAST_VERSION(7,53,0)
        case CURLOPT_ABSTRACT_UNIX_SOCKET:
#endif
        case CURLOPT_NETRC_FILE:
        case CURLOPT_USERPWD:
        case CURLOPT_PROXYUSERPWD:
        case CURLOPT_USERNAME:
        case CURLOPT_PASSWORD:
        case CURLOPT_LOGIN_OPTIONS:
        case CURLOPT_PROXYUSERNAME:
        case CURLOPT_PROXYPASSWORD:
        case CURLOPT_TLSAUTH_USERNAME:
        case CURLOPT_TLSAUTH_PASSWORD:
#if CURL_AT_LEAST_VERSION(7,52,0)
        case CURLOPT_PROXY_TLSAUTH_PASSWORD:
#endif
        case CURLOPT_TLSAUTH_TYPE:
        case CURLOPT_XOAUTH2_BEARER:
        case CURLOPT_ACCEPT_ENCODING:
        case CURLOPT_TRANSFER_ENCODING:
        // case CURLOPT_POSTFIELDS: // [TODO] does not copy...
        case CURLOPT_COPYPOSTFIELDS:
        case CURLOPT_REFERER:
        case CURLOPT_USERAGENT:
        case CURLOPT_COOKIE:
        case CURLOPT_COOKIEFILE:
        case CURLOPT_COOKIEJAR:
        case CURLOPT_COOKIELIST:
          setopt_string(L, option);
          return;

#if CURL_AT_LEAST_VERSION(7,49,0)
        case CURLOPT_CONNECT_TO:
          setopt_slist(L, option, check_easy_handle(L, 1)->connect_to());
          return;
#endif
        case CURLOPT_RESOLVE:
          setopt_slist(L, option, check_easy_handle(L, 1)->resolve());
          return;
        case CURLOPT_HTTPHEADER:
          setopt_slist(L, option, check_easy_handle(L, 1)->http_header());
          return;
        case CURLOPT_PROXYHEADER:
          setopt_slist(L, option, check_easy_handle(L, 1)->proxy_header());
          return;
        case CURLOPT_HTTP200ALIASES:
          setopt_slist(L, option, check_easy_handle(L, 1)->http_200_aliases());
          return;

        // [TODO] impl
        case CURLOPT_HTTPPOST:
          push_error(L, CURLE_UNKNOWN_OPTION);
          return;

#if CURL_AT_LEAST_VERSION(7,46,0)
        // [TODO] impl
        case CURLOPT_STREAM_DEPENDS:
        case CURLOPT_STREAM_DEPENDS_E:
          push_error(L, CURLE_UNKNOWN_OPTION);
          return;
#endif

        default:
          push_error(L, CURLE_UNKNOWN_OPTION);
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
