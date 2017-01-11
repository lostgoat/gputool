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

    amdregdb::load_all_blocks();

    populateSupportedBlocks();

    for (auto const &block : amdregdb::gRegDb) {
        if (supportsBlock(block)) {
            printf("Loading %s\n", block->name);
            mRegBlocks.push_back(block);
        }
    }

    printf("Initialized AmdGpuDevice\n");
}

AmdGpuDevice::~AmdGpuDevice()
{
    close(mRegFd);
}

AmdGpuDevice::AsicType AmdGpuDevice::getAsicType()
{
    return CHIP_POLARIS10;
}

void AmdGpuDevice::populateSupportedBlocks()
{
    switch (getAsicType()) {
    case CHIP_TOPAZ:
        mSupportedBlockNames.push_back(std::string("gmc_gmc_7_0"));
        mSupportedBlockNames.push_back(std::string("gca_gfx_8_0"));
        break;
    case CHIP_FIJI:
        mSupportedBlockNames.push_back(std::string("gmc_gmc_8_5"));
        mSupportedBlockNames.push_back(std::string("dce_dce_10_1"));
        mSupportedBlockNames.push_back(std::string("gca_gfx_8_0"));
        mSupportedBlockNames.push_back(std::string("uvd_uvd_6_0"));
        mSupportedBlockNames.push_back(std::string("vce_vce_3_0"));
        break;
    case CHIP_TONGA:
        mSupportedBlockNames.push_back(std::string("gmc_gmc_8_0"));
        mSupportedBlockNames.push_back(std::string("dce_dce_10_0"));
        mSupportedBlockNames.push_back(std::string("gca_gfx_8_0"));
        mSupportedBlockNames.push_back(std::string("uvd_uvd_5_0"));
        mSupportedBlockNames.push_back(std::string("vce_vce_3_0"));
        break;
    case CHIP_POLARIS11:
    case CHIP_POLARIS10:
        mSupportedBlockNames.push_back(std::string("gmc_gmc_8_1"));
        mSupportedBlockNames.push_back(std::string("dce_dce_11_2"));
        mSupportedBlockNames.push_back(std::string("gca_gfx_8_0"));
        mSupportedBlockNames.push_back(std::string("uvd_uvd_6_3"));
        mSupportedBlockNames.push_back(std::string("vce_vce_3_4"));
        break;
    case CHIP_CARRIZO:
        mSupportedBlockNames.push_back(std::string("gmc_gmc_8_0"));
        mSupportedBlockNames.push_back(std::string("dce_dce_11_0"));
        mSupportedBlockNames.push_back(std::string("gca_gfx_8_0"));
        mSupportedBlockNames.push_back(std::string("uvd_uvd_6_0"));
        mSupportedBlockNames.push_back(std::string("vce_vce_3_1"));
        break;
    case CHIP_STONEY:
        mSupportedBlockNames.push_back(std::string("gmc_gmc_8_0"));
        mSupportedBlockNames.push_back(std::string("dce_dce_11_0"));
        mSupportedBlockNames.push_back(std::string("gca_gfx_8_1"));
        mSupportedBlockNames.push_back(std::string("uvd_uvd_6_2"));
        mSupportedBlockNames.push_back(std::string("vce_vce_3_4"));
        break;
    default:
        util::die("Unrecognized device.\n"
                  "    You can help us register your device ID by editing %s\n"
                  "    Pull requests are welcome", __FILE__);
    }
}

bool AmdGpuDevice::supportsBlock(const amdregdb::RegBlock *block)
{
    for (auto const &iName : mSupportedBlockNames) {
        if (block->name == iName) {
            return true;
        }
    }

    return false;
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

    /* This might need faster searching in the future, maybe a PATRICIA trie */
    /* Also support for fuzzy searching */
    /* So far it seems okay on my system though */
    for (auto const &block : mRegBlocks) {
        for (int i = 0; i < block->size; ++i) {
            if (name == block->regs[i].name)
                regs.push_back(&block->regs[i]);
        }
    }

    return regs;
}

void AmdGpuDevice::write(const amdregdb::RegSpec &reg, uint32_t val)
{
    printf("Write stub: %s->0x%x\n", reg.name, val);
}

// ---------------------------------------------------------------------------
};
