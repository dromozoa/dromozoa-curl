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

local unix = require "dromozoa.unix"
local curl = require "dromozoa.curl"

local selector = unix.selector()

local multi = assert(curl.multi())

local easy = assert(curl.easy())
assert(easy:setopt(curl.CURLOPT_URL, "https://dromozoa.s3.amazonaws.com/pub/index.html"))
assert(multi:add_handle(easy))

multi:setopt_socket_function(function (easy, s, what)
  print(easy, s, what)
end)

multi:setopt_timer_function(function (multi, timeout_ms)
  print(multi, timeout_ms)
end)

-- selector:select(1)

assert(multi:remove_handle(easy))
assert(easy:cleanup())
assert(multi:cleanup())
