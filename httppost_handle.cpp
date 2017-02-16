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
    for (int arg = 2; arg <= top; arg += 2) {
      CURLformoption option = luaX_check_enum<CURLformoption>(L, arg);
      std::cout << option << std::endl;
    }

/*
    size_t size = 0;
    switch (option) {
      case CURLFORM_COPYNAME:
        if (const char* name = lua_tolstring(L, index, &size)) {
          return curl_formadd(&first_, &last_,
              option, name,
              CURLFORM_NAMELENGTH, size,
              CURLFORM_END);
        } else {
          return 
        }
        break;
    }
*/
    return CURL_FORMADD_OK;
  }

  struct curl_httppost* httppost_handle::get() const {
    return first_;
  }
}
