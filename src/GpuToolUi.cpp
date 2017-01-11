/*
 * Copyright (C) 2017  Andres Rodriguez & Valve Coporation
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

#include "GpuToolUi.h"

#include <stdio.h>
#include <iostream>
#include <sstream>
#include <string>

#include <amdregdb/RegSpec.h>
#include <util/util.h>
#include <cstddef>
#include "AmdGpuDevice.h"

namespace gputool
{
// ---------------------------------------------------------------------------

class UserInput
{
  public:
    typedef enum UserCommand {
        UC_REG_READ = 0,
        UC_REG_WRITE,
        UC_BAD_INPUT,
        UC_EXIT,
    } UserCommand;

  public:
    UserInput(std::string command);
    ~UserInput(){};

    std::string mOriginalInput;
    UserCommand mCommand;
    std::string mRegName;
    uint32_t mRegValue;
};

UserInput::UserInput(std::string command)
    : mOriginalInput(command), mCommand(UC_BAD_INPUT), mRegName(""), mRegValue(0)
{
    std::istringstream ss(command + " ");
    std::string token;

    /* Barebones parser */
    std::getline(ss, token, ' ');
    if (token == "read") {
        mCommand = UC_REG_READ;
        std::getline(ss, mRegName, ' ');
    } else if (token == "write") {
        mCommand = UC_REG_WRITE;
        std::getline(ss, mRegName, ' ');

        std::getline(ss, token, ' ');
        if (token.empty())
            mCommand = UC_BAD_INPUT;

        mRegValue = std::atoi(token.c_str());
    } else if (token == "exit" || token == "quit") {
        mCommand = UC_EXIT;
    } else {
        mCommand = UC_BAD_INPUT;
    }
}

GpuToolUi::GpuToolUi() : mGpuDevice(util::make_unique<AmdGpuDevice>())
{
}

GpuToolUi::~GpuToolUi()
{
}

int GpuToolUi::run()
{
    std::unique_ptr<UserInput> input;

    do {
        input = getNextInput();
        dispatch(*input);
    } while (input->mCommand != UserInput::UC_EXIT);

    return 0;
}

int GpuToolUi::dispatch(const UserInput &input)
{
    switch (input.mCommand) {
        case UserInput::UC_REG_READ:
        case UserInput::UC_REG_WRITE:
            doRegOp(input);
            break;
        case UserInput::UC_EXIT:
            // nothing to do here
            break;
        case UserInput::UC_BAD_INPUT:
            std::cout << "Bad input: " << input.mOriginalInput << "\n";
            break;
        default:
            std::cout << "Unrecognized input: " << input.mOriginalInput << "\n";
            break;
    }

    return 0;
}

int GpuToolUi::doRegOp(const UserInput &input)
{
    uint32_t regVal;

    std::vector<const amdregdb::RegSpec *> regSpec =
        mGpuDevice->getRegSpec(input.mRegName);
    for (auto const &reg : regSpec) {
        if (input.mCommand == UserInput::UC_REG_WRITE)
            mGpuDevice->write(*reg, input.mRegValue);

        regVal = mGpuDevice->read(*reg);

        /* This could probably use a real formatting attempt */
        printf("%s: 0x%x\n", reg->name, regVal);
        if (!reg->fields.empty()) {
            for (auto const &field : reg->fields) {
                uint32_t fieldVal = (regVal & field.mask) >> field.shift;
                printf("    %s: 0x%x\n", field.name, fieldVal);
            }
        }
    }

    return 0;
}

std::unique_ptr<UserInput> GpuToolUi::getNextInput()
{
    std::string command;

    std::cout << "gputool> ";
    std::getline(std::cin, command);

    return util::make_unique<UserInput>(command);
}

// ---------------------------------------------------------------------------
};
