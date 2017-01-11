# gputool amdregdb/ cmake file
#
# Copyright (C) 2017 Andres Rodriguez
#
# This file is part of gputool.
#
# gputool is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 2 of the License, or
# (at your option) any later version.
#
# gputool is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with gputool.  If not, see <http://www.gnu.org/licenses/>.

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -O0")

set(SOURCES
	AmdRegDb.cpp
    bif/bif_3_0.cpp
    bif/bif_4_1.cpp
    bif/bif_5_0.cpp
    bif/bif_5_1.cpp
    dce/dce_10_0.cpp
    dce/dce_11_0.cpp
    dce/dce_11_2.cpp
    dce/dce_6_0.cpp
    dce/dce_8_0.cpp
    gca/gfx_6_0.cpp
    gca/gfx_7_2.cpp
    gca/gfx_8_0.cpp
    gca/gfx_8_1.cpp
    gmc/gmc_6_0.cpp
    gmc/gmc_7_0.cpp
    gmc/gmc_7_1.cpp
    gmc/gmc_8_1.cpp
    gmc/gmc_8_2.cpp
    oss/oss_1_0.cpp
    oss/oss_2_0.cpp
    oss/oss_2_4.cpp
    oss/oss_3_0_1.cpp
    oss/oss_3_0.cpp
    smu/smu_6_0.cpp
    smu/smu_7_0_0.cpp
    smu/smu_7_0_1.cpp
    smu/smu_7_1_0.cpp
    smu/smu_7_1_1.cpp
    smu/smu_7_1_2.cpp
    smu/smu_7_1_3.cpp
    smu/smu_8_0.cpp
    uvd/uvd_4_0.cpp
    uvd/uvd_4_2.cpp
    uvd/uvd_5_0.cpp
    uvd/uvd_6_0.cpp
    vce/vce_1_0.cpp
    vce/vce_2_0.cpp
    vce/vce_3_0.cpp

)

add_library(amdregdb ${SOURCES})

set_property(TARGET amdregdb PROPERTY CXX_STANDARD 11)
set_property(TARGET amdregdb PROPERTY CXX_STANDARD_REQUIRED ON)

install (TARGETS amdregdb DESTINATION lib)
