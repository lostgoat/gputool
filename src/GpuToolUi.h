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
#include "AmdDebugFs.h"

namespace gputool
{
// ---------------------------------------------------------------------------

class UserInput;
class AmdGpuDevice;

class GpuToolUi
{
  public:
    GpuToolUi();
    ~GpuToolUi();
    int run();
    int runSingle(std::string command);

  private:
    std::unique_ptr<UserInput> getNextInput();
    int dispatch(const UserInput &input);
    int doRegOp(const UserInput &input);
    int doPrintGcaInfo(const UserInput &input);
    int doPrintWaveInfo(const UserInput &input);
    int doPrintWavePriorityInfo(const UserInput &input);
    void printFormattedReg(const amdregdb::RegSpec *spec, uint32_t val);

    std::unique_ptr<AmdGpuDevice> mGpuDevice;
};
// ---------------------------------------------------------------------------
};  // namespace gputool
