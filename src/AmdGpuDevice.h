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

#pragma once

#include <amdregdb/RegSpec.h>
#include <stdint.h>
#include <memory>
#include <string>
#include <vector>

#include "AmdDebugFs.h"

namespace gputool
{
// ---------------------------------------------------------------------------

class WaveInfo
{
  public:
    amddebugfs::wave_info mWaveInfo;
    int se;
    int sh;
    int cu;
    int wave;
    int simd;

    WaveInfo(int se, int sh, int cu, int wave, int simd)
        : se(se), sh(sh), cu(cu), wave(wave), simd(simd)
    {
    }

    ~WaveInfo(){};
};

class AmdGpuDevice
{
  public:
    typedef enum AsicType {
        CHIP_TAHITI = 0,
        CHIP_PITCAIRN,
        CHIP_VERDE,
        CHIP_OLAND,
        CHIP_HAINAN,
        CHIP_BONAIRE,
        CHIP_KAVERI,
        CHIP_KABINI,
        CHIP_HAWAII,
        CHIP_MULLINS,
        CHIP_TOPAZ,
        CHIP_TONGA,
        CHIP_FIJI,
        CHIP_CARRIZO,
        CHIP_STONEY,
        CHIP_POLARIS10,
        CHIP_POLARIS11,
        CHIP_UNSUPPORTED,
        CHIP_LAST,
    } AsicType;

    AmdGpuDevice();
    ~AmdGpuDevice();

    uint32_t read(const amdregdb::RegSpec &reg);
    void write(const amdregdb::RegSpec &reg, uint32_t val, const amdregdb::RegField *pField);
    std::vector<const amdregdb::RegSpec *> getRegSpecs(std::string pattern);
    const amdregdb::RegSpec *getRegSpec(std::string name);
    std::vector<std::unique_ptr<WaveInfo>> getWaveInfo();
    uint32_t getFieldAs(std::string regName, std::string fieldName, uint32_t val);

    amddebugfs::gca_info mGcaInfo;

  private:
    AsicType getAsicType();
    void populateSupportedBlocks();
    void populateGcaInfo();
    bool supportsBlock(const amdregdb::RegBlock *block);
    void fillWaveInfo(int fd, WaveInfo *wave);

    int mRegFd;
    std::vector<const amdregdb::RegBlock *> mRegBlocks;
    std::vector<std::string> mSupportedBlockNames;

    static const int sRegSizeByte = 4;
    static const uint32_t sRequiredGcaInfoVer = 2;
    static const uint32_t sRequiredWaveInfoVer = 0;

    static constexpr char const *sRegPath = "/sys/kernel/debug/dri/0/amdgpu_regs";
    static constexpr char const *sWaveInfoPath = "/sys/kernel/debug/dri/0/amdgpu_wave";
    static constexpr char const *sGcaInfoPath =
        "/sys/kernel/debug/dri/0/amdgpu_gca_config";
};
// ---------------------------------------------------------------------------
};  // namespace gputool
