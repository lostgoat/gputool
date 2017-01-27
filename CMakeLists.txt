# gputool project cmake file
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

cmake_minimum_required(VERSION 3.0.0)

project(gputool)

# Version number
set (GPUTOOL_VERSION_MAJOR 0)
set (GPUTOOL_VERSION_MINOR 2)
set (GPUTOOL_VERSION_PATCH 0)

set (AUTOGEN_HEADERS_DIR "${PROJECT_BINARY_DIR}/autogen/include")

# Include <gputool/config.h>
configure_file (
  "${PROJECT_SOURCE_DIR}/config.h.in"
  "${AUTOGEN_HEADERS_DIR}/gputool/config.h"
  )

include_directories("${AUTOGEN_HEADERS_DIR}")
include_directories(
	"include/"
	"amdregdb/include"
	)

include(cmake/comp_base.cmake)

add_subdirectory(amdregdb)
add_subdirectory(src)

comp_target_features(amdregdb PUBLIC cpp17_lib/uncaught_exceptions)
comp_target_features(gputool PUBLIC cpp17_lib/uncaught_exceptions)

set (SRC_EXTENSIONS cpp h)
set (SRC_DIRS src/ include/)

# make cppcheck will run some basic static analysis
set (CPPCHECK_EXTENSIONS ${SRC_EXTENSIONS})
set (CPPCHECK_DIR ${SRC_DIRS})
include(cmake/cppcheck.cmake)

# make format fixes the formatting in-place
set (CLANG_FORMAT_EXTENSIONS ${SRC_EXTENSIONS})
set (CLANG_FORMAT_DIR ${SRC_DIRS})
include(cmake/clang-format.cmake)

# make genregs generates registers headers
include(cmake/genregs.cmake)
