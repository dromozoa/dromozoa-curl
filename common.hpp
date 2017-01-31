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

#ifndef DROMOZOA_COMMON_HPP
#define DROMOZOA_COMMON_HPP

#include <curl/curl.h>

#include <dromozoa/bind.hpp>

namespace dromozoa {
  class easy_handle {
  public:
    explicit easy_handle(CURL* handle);
    ~easy_handle();
    void cleanup();
    CURL* get() const;
  private:
    CURL* handle_;
  };

  easy_handle* check_easy_handle(lua_State* L, int arg);

  void push_error(lua_State* L, CURLcode code);
  void push_error_not_implemented(lua_State* L);
}

#endif
