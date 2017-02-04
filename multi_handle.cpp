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
  multi_handle::multi_handle(CURLM* handle) : handle_(handle) {}

  multi_handle::~multi_handle() {
    if (handle_) {
      cleanup();
    }
  }

  CURLMcode multi_handle::cleanup() {
    CURLM* handle = handle_;
    handle_ = 0;
    return curl_multi_cleanup(handle);
  }

  CURLM* multi_handle::get() const {
    return handle_;
  }

  luaX_reference& multi_handle::socket_function() {
    return socket_function_;
  }

  luaX_reference& multi_handle::timer_function() {
    return timer_function_;
  }
}
