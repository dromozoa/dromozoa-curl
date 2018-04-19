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

assert(curl.global_init())

local easy = assert(curl.easy())
assert(easy:reset())

local result, message, code = easy:setopt(-1, 1)
if verbose then
  io.stderr:write(message, "\n")
end
assert(not result)
assert(code == curl.CURLE_UNKNOWN_OPTION or code == curl.CURLE_BAD_FUNCTION_ARGUMENT)

local url = "https://dromozoa.s3.amazonaws.com/pub/dromozoa-curl/test.txt"

assert(easy:setopt(curl.CURLOPT_URL, url))
assert(easy:setopt(curl.CURLOPT_FOLLOWLOCATION, 1))

local header = {}
assert(easy:setopt(curl.CURLOPT_HEADERFUNCTION, function (data)
  header[#header + 1] = data
end))

local body = {}
assert(easy:setopt(curl.CURLOPT_WRITEFUNCTION, function (data)
  body[#body + 1] = data
end))

assert(easy:perform())
assert(easy:getinfo(curl.CURLINFO_EFFECTIVE_URL) == url)
assert(easy:getinfo(curl.CURLINFO_RESPONSE_CODE) == 200)
assert(easy:getinfo(curl.CURLINFO_CONTENT_LENGTH_DOWNLOAD) == 56)
assert(easy:getinfo(curl.CURLINFO_CONTENT_TYPE) == "text/plain")

local header = table.concat(header):gsub("\r\n\r\n$", "\r\n"):gsub("\r\n%s+", " ")
local header = assert(header:match "^HTTP/1%.1 200 OK\r\n(.*)")
local headers = {}
for item in header:gmatch "(.-)\r\n" do
  if verbose then
    io.stderr:write(item, "\n")
  end
  local name, value = assert(item:match "^([^:]+):%s*(.*)")
  headers[name] = value
end

assert(headers["ETag"] == [["edea258640a0460e6156cfb98d6cdf3b"]])
assert(headers["Accept-Ranges"] == "bytes")
assert(headers["Content-Type"] == "text/plain")
assert(headers["Content-Length"] == "56")
assert(headers["Server"] == "AmazonS3")

assert(table.concat(body) == [[
1. foo
2. bar
3. baz
4. qux
5. foo
6. bar
7. baz
8. qux
]])
