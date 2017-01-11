# gputool src/ cmake file
#
# Copyright (C) 2017 Andres Rodriguez
#
# This file is part of gputool.
#
# Foobar is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 2 of the License, or
# (at your option) any later version.
#
# Foobar is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with Foobar.  If not, see <http://www.gnu.org/licenses/>.

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -O0")

set(SOURCES
        GpuToolUi.cpp
        AmdGpuDevice.cpp
        gputool.cpp)

add_executable(gputool ${SOURCES})
target_link_libraries (gputool amdregdb)

set_property(TARGET gputool PROPERTY CXX_STANDARD 11)
set_property(TARGET gputool PROPERTY CXX_STANDARD_REQUIRED ON)

install (TARGETS gputool DESTINATION bin)
