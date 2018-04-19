-- Copyright (C) 2017,2018 Tomoyuki Fujimori <moyu@dromozoa.com>
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

local curl = require "dromozoa.curl"

local verbose = os.getenv "VERBOSE" == "1"

local url = "http://minalinsky.dromozoa.com/cgi-bin/dromozoa-curl-echo.cgi"
local body_data = {}

assert(curl.global_init())

local easy = assert(curl.easy())
if verbose then
  assert(easy:setopt(curl.CURLOPT_VERBOSE, 1))
end
assert(easy:setopt(curl.CURLOPT_RESOLVE, { "minalinsky.dromozoa.com:80:49.212.22.139" }))
assert(easy:setopt(curl.CURLOPT_URL, url))
assert(easy:setopt(curl.CURLOPT_WRITEFUNCTION, function (data)
  body_data[#body_data + 1] = data
end))

assert(easy:perform())

local body = table.concat(body_data)
if verbose then
  io.stderr:write(body)
end
local result = assert(assert((loadstring or load)(body))())
assert(result.REQUEST_METHOD == "GET")
assert(result.REQUEST_SCHEME == "http")
assert(result.REQUEST_URI == "/cgi-bin/dromozoa-curl-echo.cgi")
assert(result.QUERY_STRING == "")
assert(result.HTTP_HOST == "minalinsky.dromozoa.com")
assert(result.HTTP_USER_AGENT == "")
assert(result[1] == "")
