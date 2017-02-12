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

local read_file = require "dromozoa.commons.read_file"
local sequence = require "dromozoa.commons.sequence"
local split = require "dromozoa.commons.split"
local string_matcher = require "dromozoa.commons.string_matcher"
local unpack = require "dromozoa.commons.unpack"

local source_dir = ...

local ignore_symbols = {
  CURL_DID_MEMORY_FUNC_TYPEDEFS = true;
  CURL_STRICTER = true;
  CURLOPT_WRITEINFO = true;
}

local alias_symbols = {
  CURLOPT_ENCODING = "CURLOPT_ACCEPT_ENCODING";
  CURLOPT_POST301 = "CURLOPT_POSTREDIR";
  CURLOPT_RTSPHEADER = "CURLOPT_HTTPHEADER";
  CURLOPT_SERVER_RESPONSE_TIMEOUT = "CURLOPT_FTP_RESPONSE_TIMEOUT";
  CURLOPT_WRITEHEADER = "CURLOPT_HEADERDATA";
}

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

local function check_option(name)
  local alias_name

  local doc = read_file(("%s/docs/libcurl/opts/%s.3"):format(source_dir, name))
  if doc == nil then
    alias_name = alias_symbols[name]
    doc = read_file(("%s/docs/libcurl/opts/%s.3"):format(source_dir, alias_name))
    assert(doc, "not found symbol " .. name)
  end

  local params
  if name:match("^CURLOPT_") then
    params = assert(doc:match("CURLcode%s+curl_easy_setopt%((.-)%)"))
  elseif name:match("^CURLMOPT_") then
    params = assert(doc:match("CURLMcode%s+curl_multi_setopt%((.-)%)"))
  end
  local params = split(params, ",%s*")
  assert(params[2] == name or params[2] == alias_name)

  local param_type
  local param_name

  local matcher = string_matcher((params[3]:gsub("%s+$", ""):gsub("%s+", " ")))
  if matcher:match("([%w_]+) ([%w_]+)$")
      or matcher:match("(char %*%*)([%w_]+)$")
      or matcher:match("([%w_]+ %*)([%w_]+)$")
      or matcher:match("(struct [%w_]+ %*)([%w_]+)$")
      or matcher:match("([%w_]+callback) ([%w_]+)$")
      or matcher:match("([%w_]+callback)$")
  then
    param_type = matcher[1]
    param_name = matcher[2]
    print(("%s|%s|%s"):format(name, param_type, param_name or ""))
  end
  assert(param_type)

--[[
  local param = params[3]:gsub("%s+$", ""):gsub("%s+", " ")
  local param_type
  if param:match("^char %*%*[%w_]+$") then
    param_type = "char **"
  elseif param:match("^char %*[%w_]+$") then
    param_type = "char *"
  elseif param:match("^long [%w_]+$") then
    param_type = "long"
  elseif param:match("^curl_off_t [%w_]+$") then
    param_type = "curl_off_t"
  elseif param:match("^void %*[%w_]+$") then
    param_type = "void *"
  elseif param:match("^struct curl_httppost %*[%w_]+$") then
    param_type = "struct curl_httppost *"
  elseif param:match("^struct curl_slist %*[%w_]+$") then
    param_type = "struct curl_slist *"
  elseif param:match("^CURL %*[%w_]+$") then
    param_type = "CURL *"
  elseif param:match("^CURLSH %*[%w_]+$") then
    param_type = "CURLSH *"
  elseif param:match("^FILE %*[%w_]+$") then
    param_type = "FILE *"
  else
    local callback_type = assert(param:match("^([%w_]+callback)"))
  end
]]
end

--[[
https://curl.haxx.se/libcurl/c/curl_easy_setopt.html
> Before version 7.17.0, strings were not copied. Instead the user was forced
> keep them available until libcurl no longer needed them.
]]
local version_min = curl_version_bits("7.17.0")

local symbols_file = source_dir .. "/docs/libcurl/symbols-in-versions"
local symbols_name = assert(symbols_file:match("(curl%-%d+%.%d+%.%d+/.*)"))

local out = assert(io.open("symbols.cpp", "w"))
local options = sequence()

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
    introduced = assert(curl_version_bits(introduced))
    deprecated = curl_version_bits(deprecated)
    removed = curl_version_bits(removed)
    if not ignore_symbols[name] and (deprecated == nil or deprecated > version_min) and (removed == nil or removed > version_min) then
      if name:match("^CURLOPT_") or name:match("^CURLMOPT_") then
        local alias_name = alias_symbols[name]
        if alias_name == nil then
          local option = check_option(name)
        end
      end

      local condition
      if introduced >= version_min then
        condition = ("0x%06x <= LIBCURL_VERSION_NUM"):format(introduced)
      end
      if removed ~= nil then
        if condition ~= nil then
          condition = condition .. " && "
        end
        condition = condition .. ("LIBCURL_VERSION_NUM < 0x%06x"):format(removed)
      end
      if condition ~= nil then
        out:write("#if ", condition, "\n")
      end
      out:write("    luaX_set_field<lua_Integer>(L, -1, \"", name, "\", ", name, ");\n")
      if condition ~= nil then
        out:write("#endif\n")
      end
    end
  end
end

out:write([[
  }
}
]])
out:close()

for option in options:each() do
  print(option.name)
end
