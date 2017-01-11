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

#include "GpuToolUi.h"

#include <stdio.h>
#include <iostream>
#include <sstream>
#include <string>

#include <cstddef>
#include "AmdGpuDevice.h"
#include "RegSpec.h"
#include "util.h"

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

    std::vector<const amdregdb::RegSpec*> regSpec = mGpuDevice->getRegSpec(input.mRegName);
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
