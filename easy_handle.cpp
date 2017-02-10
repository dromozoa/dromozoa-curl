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
  easy_handle::easy_handle(CURL* handle) : handle_(handle) {}

  easy_handle::~easy_handle() {
    if (handle_) {
      cleanup();
    }
  }

  void easy_handle::cleanup() {
    CURL* handle = handle_;
    handle_ = 0;
    curl_easy_cleanup(handle);
  }

  CURL* easy_handle::get() const {
    return handle_;
  }

  luaX_reference& easy_handle::write_function() {
    return write_function_;
  }

  luaX_reference& easy_handle::read_function() {
    return read_function_;
  }

  luaX_reference& easy_handle::header_function() {
    return header_function_;
  }

  string_list& easy_handle::connect_to() {
    return connect_to_;
  }

  string_list& easy_handle::resolve() {
    return resolve_;
  }

  string_list& easy_handle::http_header() {
    return http_header_;
  }

  string_list& easy_handle::proxy_header() {
    return proxy_header_;
  }

  string_list& easy_handle::http_200_aliases() {
    return http_200_aliases_;
  }
}
