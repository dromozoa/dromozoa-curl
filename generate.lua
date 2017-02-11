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

local source_dir = ...

--[[
https://curl.haxx.se/libcurl/c/curl_easy_setopt.html
> Before version 7.17.0, strings were not copied. Instead the user was forced
> keep them available until libcurl no longer needed them.
]]
local version_min = curl_version_bits("7.17.0")

local symbols_file = source_dir .. "/docs/libcurl/symbols-in-versions"
local symbols_name = assert(symbols_file:match("(curl%-%d+%.%d+%.%d+/.*)"))

local ignore_symbols = {
  CURL_DID_MEMORY_FUNC_TYPEDEFS = true;
  CURL_STRICTER = true;
}

local out = assert(io.open("symbols.cpp", "w"))

out:write(([[
// generated from %s

#include "common.hpp"

namespace dromozoa {
  void initialize_symbols(lua_State* L) {
]]):format(symbols_name))

for line in io.lines(symbols_file) do
  if line:match("^CURL") then
    local data = split(line, "%s+")
    local name, introduced, deprecated, removed = unpack(data)
    if not ignore_symbols[name] then
      introduced = assert(curl_version_bits(introduced))
      removed = curl_version_bits(removed)
      if removed == nil or removed > version_min then





        if introduced >= version_min then
          if removed ~= nil then
            out:write(([[
#if 0x%06x <= LIBCURL_VERSION_NUM && LIBCURL_VERSION_NUM < 0x%06x
    luaX_set_field<lua_Integer>(L, -1, "%s", %s);
#endif
]]):format(introduced, removed, name, name))
          else
            out:write(([[
#if 0x%06x <= LIBCURL_VERSION_NUM
    luaX_set_field<lua_Integer>(L, -1, "%s", %s);
#endif
]]):format(introduced, name, name))
          end
        else
          if removed ~= nil then
            out:write(([[
#if LIBCURL_VERSION_NUM < 0x%06x
    luaX_set_field<lua_Integer>(L, -1, "%s", %s);
#endif
]]):format(removed, name, name))
          else
            out:write(([[
    luaX_set_field<lua_Integer>(L, -1, "%s", %s);
]]):format(name, name))
          end
        end
      end
    end
  end
end

out:write([[
  }
}
]])
out:close()
