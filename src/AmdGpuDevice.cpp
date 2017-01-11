/*
 * Copyright (C) 2017  Andres Rodriguez
 *
 * This file is part of gputool.
 *
 * gputool is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
 * (at your option) any later version.
 *
 * gputool is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with gputool.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "AmdGpuDevice.h"

#include <fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <vector>

#include <amdregdb/AmdRegDb.h>
#include <amdregdb/RegSpec.h>
#include <util/util.h>
#include <cstddef>

namespace gputool
{
// ---------------------------------------------------------------------------

const char *AmdGpuDevice::sRegPath = "/sys/kernel/debug/dri/0/amdgpu_regs";

AmdGpuDevice::AmdGpuDevice() : mRegFd(-1)
{
    mRegFd = open(sRegPath, O_RDWR);
    failOn(mRegFd < 0, "Error opening %s for AmdGpuDevice, are your root?\n", sRegPath);
    SCOPE_FAIL { close(mRegFd); };

    printf("Initialized AmdGpuDevice\n");
}

AmdGpuDevice::~AmdGpuDevice()
{
    close(mRegFd);
}

uint32_t AmdGpuDevice::read(const amdregdb::RegSpec &reg)
{
    int r;
    uint32_t val;

    /* Careful: Register offset is in sequence number, not bytes*/
    r = ::lseek(mRegFd, reg.offset * sRegSizeByte, SEEK_SET);
    failOn(r == -1, "Failed to seek register %s\n", reg.name);

    r = ::read(mRegFd, (void *)&val, sizeof(val));
    failOn(r != sizeof(val), "Failed to read register %s\n", reg.name);

    return val;
}

std::vector<const amdregdb::RegSpec *> AmdGpuDevice::getRegSpec(std::string name)
{
    std::vector<const amdregdb::RegSpec *> regs;

    for (size_t i = 0; i < ARRAY_SIZE(amdregdb::gca_gfx_8_0_regs); ++i) {
        if (name == amdregdb::gca_gfx_8_0_regs[i].name)
            regs.push_back(&amdregdb::gca_gfx_8_0_regs[i]);
    }

    return regs;
}

void AmdGpuDevice::write(const amdregdb::RegSpec &reg, uint32_t val)
{
    printf("Write stub: %s->0x%x\n", reg.name, val);
}

// ---------------------------------------------------------------------------
};
