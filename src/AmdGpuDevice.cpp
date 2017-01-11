/*
 * Copyright (C) 2017  Andres Rodriguez
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to
 * deal in the Software without restriction, including without limitation the
 * rights to use, copy, modify, merge, publish, distribute, sublicense, and/or
 * sell copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL THE
 * COPYRIGHT HOLDER(S) BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
 * WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR
 * IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#include "AmdGpuDevice.h"

#include <fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <vector>

#include <cstddef>
#include "util.h"
#include "RegSpec.h"
#include "AmdRegDb.h"

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

std::vector<const amdregdb::RegSpec*> AmdGpuDevice::getRegSpec(std::string name)
{
    std::vector<const amdregdb::RegSpec*> regs;

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
