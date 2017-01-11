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
#include <string>
#include <vector>

namespace gputool
{
// ---------------------------------------------------------------------------

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
    void write(const amdregdb::RegSpec &reg, uint32_t val);
    std::vector<const amdregdb::RegSpec *> getRegSpec(std::string);

  private:
    AsicType getAsicType();
    void populateSupportedBlocks();
    bool supportsBlock(const amdregdb::RegBlock *block);

    int mRegFd;
    std::vector<const amdregdb::RegBlock *> mRegBlocks;
    std::vector<std::string> mSupportedBlockNames;

    static const char *sRegPath;
    static const int sRegSizeByte = 4;
};
// ---------------------------------------------------------------------------
};  // namespace gputool
