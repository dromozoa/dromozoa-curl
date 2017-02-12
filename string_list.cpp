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
  string_list::string_list(struct curl_slist* slist) : slist_(slist) {}

  string_list::~string_list() {
    if (slist_) {
      curl_slist_free_all(slist_);
    }
  }

  struct curl_slist* string_list::get() const {
    return slist_;
  }

  struct curl_slist* string_list::release() {
    curl_slist* slist = slist_;
    slist_ = 0;
    return slist;
  }

  void string_list::append(const char* string) {
    slist_ = curl_slist_append(slist_, string);
  }

  void string_list::swap(string_list& that) {
    struct curl_slist* slist = slist_;
    slist_ = that.slist_;
    that.slist_ = slist;
  }
}
