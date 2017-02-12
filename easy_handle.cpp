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

    std::map<CURLoption, curl_slist*>::iterator i = slists_.begin();
    std::map<CURLoption, curl_slist*>::iterator end = slists_.end();
    for (; i != end; ++i) {
      curl_slist_free_all(i->second);
    }
    slists_.clear();
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

  void easy_handle::set_slist(CURLoption option, curl_slist* slist) {
    std::map<CURLoption, curl_slist*>::iterator i = slists_.find(option);
    if (i == slists_.end()) {
      slists_.insert(std::make_pair(option, slist));
    } else {
      curl_slist_free_all(i->second);
      i->second = slist;
    }
  }
}
