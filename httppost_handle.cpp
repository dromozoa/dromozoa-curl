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

#include <iostream>
#include <vector>

#include "common.hpp"

namespace dromozoa {
  httppost_handle::httppost_handle() : first_(), last_() {}

  httppost_handle::~httppost_handle() {
    if (first_) {
      free();
    }
  }

  void httppost_handle::free() {
    struct curl_httppost* first = first_;
    first_ = 0;
    last_ = 0;
    curl_formfree(first);
  }

  CURLFORMcode httppost_handle::add(lua_State* L) {
    int top = lua_gettop(L);

    std::vector<struct curl_forms> forms;

    for (int arg = 2; arg <= top; arg += 2) {
      CURLformoption option = luaX_check_enum<CURLformoption>(L, arg);
      if (option == CURLFORM_END) {
        break;
      }

      size_t length = 0;
      switch (option) {
        case CURLFORM_COPYNAME:
          if (const char* p = lua_tolstring(L, arg + 1, &length)) {
            struct curl_forms f = {};
            f.option = option;
            f.value = const_cast<char*>(p);
            forms.push_back(f);
            f.option = CURLFORM_NAMELENGTH;
            f.value = reinterpret_cast<char*>(length);
            forms.push_back(f);
          } else {
            return CURL_FORMADD_NULL;
          }
          break;
        case CURLFORM_COPYCONTENTS:
          if (const char* p = lua_tolstring(L, arg + 1, &length)) {
            struct curl_forms f = {};
            f.option = option;
            f.value = const_cast<char*>(p);
            forms.push_back(f);
#if CURL_AT_LEAST_VERSION(7, 46, 0)
            f.option = CURLFORM_CONTENTLEN;
#else
            f.option = CURLFORM_CONTENTS_LENGTH;
#endif
            f.value = reinterpret_cast<char*>(length);
            forms.push_back(f);
          } else {
            return CURL_FORMADD_NULL;
          }
          break;
        default:
          return CURL_FORMADD_UNKNOWN_OPTION;
      }
    }

    struct curl_forms f = {};
    f.option = CURLFORM_END;
    forms.push_back(f);

    return curl_formadd(&first_, &last_, CURLFORM_ARRAY, &forms[0], CURLFORM_END);
  }

  struct curl_httppost* httppost_handle::get() const {
    return first_;
  }
}
