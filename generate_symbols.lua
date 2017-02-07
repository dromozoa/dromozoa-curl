-- Copyright (C) 2017 Tomoyuki Fujimori <moyu@dromozoa.com>
--
-- This file is part of dromozoa-curl.
--
-- dromozoa-curl is free software: you can redistribute it and/or modify
-- it under the terms of the GNU General Public License as published by
-- the Free Software Foundation, either version 3 of the License, or
-- (at your option) any later version.
--
-- dromozoa-curl is distributed in the hope that it will be useful,
-- but WITHOUT ANY WARRANTY; without even the implied warranty of
-- MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
-- GNU General Public License for more details.
--
-- You should have received a copy of the GNU General Public License
-- along with dromozoa-curl.  If not, see <http://www.gnu.org/licenses/>.

local split = require "dromozoa.commons.split"
local unpack = require "dromozoa.commons.unpack"

local function curl_version_bits(s)
  if s == nil or s == "-" then
    return nil
  else
    local data = split(s, "%.")
    if data[3] == nil then
      data[3] = 0
    end
    local major, minor, patch = assert(tonumber(data[1])), assert(tonumber(data[2])), assert(tonumber(data[3]))
    return major * 65536 + minor * 256 + patch
  end
end

local source_file = ...
local version_min = curl_version_bits("7.14.0")

local source_name = assert(source_file:match("(curl%-%d+%.%d+%.%d+/.*)"))
io.write(([[
// generated from %s

#include "common.hpp"

namespace dromozoa {
  void initialize_symbols(lua_State* L) {
]]):format(source_name))

for line in io.lines(source_file) do
  if line:match("^CURL") then
    local data = split(line, "%s+")
    local name, introduced, deprecated, removed = unpack(data)
    introduced = assert(curl_version_bits(introduced))
    removed = curl_version_bits(removed)
    if removed == nil or removed > version_min then
      if introduced >= version_min then
        if removed ~= nil then
          io.write(([[
#if 0x%06x <= LIBCURL_VERSION_NUM && LIBCURL_VERSION_NUM < 0x%06x
    luaX_set_field<lua_Integer>(L, -1, "%s", %s);
#endif
]]):format(introduced, removed, name, name))
        else
          io.write(([[
#if 0x%06x <= LIBCURL_VERSION_NUM
    luaX_set_field<lua_Integer>(L, -1, "%s", %s);
#endif
]]):format(introduced, name, name))
        end
      else
        if removed ~= nil then
          io.write(([[
#if LIBCURL_VERSION_NUM < 0x%06x
    luaX_set_field<lua_Integer>(L, -1, "%s", %s);
#endif
]]):format(removed, name, name))
        else
          io.write(([[
    luaX_set_field<lua_Integer>(L, -1, "%s", %s);
]]):format(name, name))
        end
      end
    end
  end
end

io.write([[
  }
}
]])
