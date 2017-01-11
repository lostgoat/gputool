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
    AmdGpuDevice();
    ~AmdGpuDevice();

    uint32_t read(const amdregdb::RegSpec &reg);
    void write(const amdregdb::RegSpec &reg, uint32_t val);
    std::vector<const amdregdb::RegSpec *> getRegSpec(std::string);

  private:
    int mRegFd;

    static const char *sRegPath;
    static const int sRegSizeByte = 4;
};
// ---------------------------------------------------------------------------
};  // namespace gputool
