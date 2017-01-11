# gputool project cmake file
#
# Copyright (C) 2017 Andres Rodriguez
#
# Permission is hereby granted, free of charge, to any person obtaining a copy
# of this software and associated documentation files (the "Software"), to deal
# in the Software without restriction, including without limitation the rights
# to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
# copies of the Software, and to permit persons to whom the Software is
# furnished to do so, subject to the following conditions:
#
# The above copyright notice and this permission notice shall be included in
# all copies or substantial portions of the Software.
#
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
# FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL THE
# COPYRIGHT HOLDER(S) BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
# WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR
# IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
#

cmake_minimum_required(VERSION 2.8.11)

project(gputool)

# Version number
set (GPUTOOL_VERSION_MAJOR 0)
set (GPUTOOL_VERSION_MINOR 1)
set (GPUTOOL_VERSION_PATCH 0)

set (AUTOGEN_HEADERS_DIR "${PROJECT_BINARY_DIR}/autogen/include")

# Include <gputool/config.h>
configure_file (
  "${PROJECT_SOURCE_DIR}/config.h.in"
  "${AUTOGEN_HEADERS_DIR}/gputool/config.h"
  )

include_directories("${AUTOGEN_HEADERS_DIR}")
include_directories(include)

add_subdirectory(src)

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
