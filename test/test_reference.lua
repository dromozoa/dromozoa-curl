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

local curl = require "dromozoa.curl"

local verbose = os.getenv "VERBOSE" == "1"

local easy = assert(curl.easy())

collectgarbage()
collectgarbage()
local c1 = collectgarbage "count"

assert(easy:setopt(curl.CURLOPT_POSTFIELDS, ("X"):rep(1024)))

collectgarbage()
collectgarbage()
local c2 = collectgarbage "count"

assert(easy:setopt(curl.CURLOPT_POSTFIELDS))

collectgarbage()
collectgarbage()
local c3 = collectgarbage "count"

assert(easy:setopt(curl.CURLOPT_POSTFIELDS, ("X"):rep(16)))

collectgarbage()
collectgarbage()
local c4 = collectgarbage "count"

if verbose then
  io.stderr:write(c1, "\n")
  io.stderr:write(c2, "\n")
  io.stderr:write(c3, "\n")
  io.stderr:write(c4, "\n")
end

assert(c1 < c4)
assert(c3 < c4)
assert(c4 < c2)
