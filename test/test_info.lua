-- Copyright (C) 2018 Tomoyuki Fujimori <moyu@dromozoa.com>
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

local dyld = require "dromozoa.dyld"
local curl = require "dromozoa.curl"

local verbose = os.getenv "VERBOSE" == "1"

assert(dyld.dlopen_pthread())

local easy = assert(curl.easy())
if verbose then
  assert(easy:setopt(curl.CURLOPT_VERBOSE, 1))
end
assert(easy:setopt(curl.CURLOPT_IPRESOLVE, curl.CURL_IPRESOLVE_V4))
assert(easy:setopt(curl.CURLOPT_URL, "https://kotori.dromozoa.com/cgi-bin/dromozoa-curl.cgi"))
assert(easy:setopt(curl.CURLOPT_WRITEFUNCTION, function (data)
  if verbose then
    io.stderr:write(data)
  end
end))
assert(easy:setopt(curl.CURLOPT_COOKIEFILE, ""))

assert(easy:perform())

local data = assert(easy:getinfo(curl.CURLINFO_COOKIELIST))
if verbose then
  for i = 1, #data do
    io.stderr:write(data[i], "\n")
  end
end
assert(#data == 2)
assert(data[1]:find "\tbar\tbaz$")
assert(data[2]:find "\tbaz\tqux$")

if curl.CURLINFO_TLS_SESSION then
  local info = assert(easy:getinfo(curl.CURLINFO_TLS_SESSION))
  if verbose then
    io.stderr:write(info.backend, "\n")
    io.stderr:write(tostring(info.internals), "\n")
  end
  assert(info.backend > 0)
  assert(type(info.internals) == "userdata")
end
