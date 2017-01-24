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
        UC_PRINT_GCA_INFO,
        UC_PRINT_WAVE_INFO,
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
    } else if (token == "gca_info") {
        mCommand = UC_PRINT_GCA_INFO;
    } else if (token == "wave_info") {
        mCommand = UC_PRINT_WAVE_INFO;
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
        case UserInput::UC_PRINT_GCA_INFO:
            doPrintGcaInfo(input);
            break;
        case UserInput::UC_PRINT_WAVE_INFO:
            doPrintWaveInfo(input);
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

void GpuToolUi::printFormattedReg(const amdregdb::RegSpec *spec, uint32_t val)
{
    failOn(!spec, "Missing a register definition\n");

    switch (spec->fields.size()) {
        case 0:
            printf("    %s: 0x%x\n", spec->name, val);
            break;
        case 1:
            printf("    %s: %s 0x%x\n", spec->name, spec->fields.front().name, val);
            break;
        default:
            printf("    %s: 0x%x\n", spec->name, val);
            for (auto const &field : spec->fields) {
                uint32_t fieldVal = (val & field.mask) >> field.shift;
                printf("      %s: 0x%x\n", field.name, fieldVal);
            }
            break;
    }
}

int GpuToolUi::doRegOp(const UserInput &input)
{
    uint32_t regVal;

    std::vector<const amdregdb::RegSpec *> regSpec =
        mGpuDevice->getRegSpecs(input.mRegName);
    for (auto const &reg : regSpec) {
        if (input.mCommand == UserInput::UC_REG_WRITE)
            mGpuDevice->write(*reg, input.mRegValue);

        regVal = mGpuDevice->read(*reg);
        printFormattedReg(reg, regVal);
    }

    return 0;
}

int GpuToolUi::doPrintGcaInfo(const UserInput &input)
{
#define DUMP_FIELD(name) printf("%s: 0x%x\n", #name, mGpuDevice->mGcaInfo.name)
    DUMP_FIELD(version);
    DUMP_FIELD(ax_shader_engines);
    DUMP_FIELD(max_tile_pipes);
    DUMP_FIELD(max_cu_per_sh);
    DUMP_FIELD(max_sh_per_se);
    DUMP_FIELD(max_backends_per_se);
    DUMP_FIELD(max_texture_channel_caches);
    DUMP_FIELD(max_gprs);
    DUMP_FIELD(max_gs_threads);
    DUMP_FIELD(max_hw_contexts);
    DUMP_FIELD(sc_prim_fifo_size_frontend);
    DUMP_FIELD(sc_prim_fifo_size_backend);
    DUMP_FIELD(sc_hiz_tile_fifo_size);
    DUMP_FIELD(sc_earlyz_tile_fifo_size);
    DUMP_FIELD(num_tile_pipes);
    DUMP_FIELD(backend_enable_mask);
    DUMP_FIELD(mem_max_burst_length_bytes);
    DUMP_FIELD(mem_row_size_in_kb);
    DUMP_FIELD(shader_engine_tile_size);
    DUMP_FIELD(num_gpus);
    DUMP_FIELD(multi_gpu_tile_size);
    DUMP_FIELD(mc_arb_ramcfg);
    DUMP_FIELD(gb_addr_config);
    DUMP_FIELD(num_rbs);
    DUMP_FIELD(rev_id);
    DUMP_FIELD(pg_flags);
    DUMP_FIELD(cg_flags);
    DUMP_FIELD(family);
    DUMP_FIELD(external_rev_id);
#undef DUMP_FIELD

    return 0;
}

int GpuToolUi::doPrintWaveInfo(const UserInput &input)
{
    std::vector<std::unique_ptr<WaveInfo>> waves = mGpuDevice->getWaveInfo();

    int i = 0;
    for (auto const &waveInfo : waves) {
        amddebugfs::wave_info *data = &waveInfo->mWaveInfo;

        printf("Wave: %d\n", i++);

        printFormattedReg(mGpuDevice->getRegSpec("SQ_WAVE_STATUS"), data->status);
        printFormattedReg(mGpuDevice->getRegSpec("SQ_WAVE_PC_LO"), data->pc_lo);
        printFormattedReg(mGpuDevice->getRegSpec("SQ_WAVE_PC_HI"), data->pc_hi);
        printFormattedReg(mGpuDevice->getRegSpec("SQ_WAVE_EXEC_LO"), data->exec_lo);
        printFormattedReg(mGpuDevice->getRegSpec("SQ_WAVE_EXEC_HI"), data->exec_hi);
        printFormattedReg(mGpuDevice->getRegSpec("SQ_WAVE_HW_ID"), data->hw_id);
        printFormattedReg(mGpuDevice->getRegSpec("SQ_WAVE_INST_DW0"), data->inst_dw0);
        printFormattedReg(mGpuDevice->getRegSpec("SQ_WAVE_INST_DW1"), data->inst_dw1);
        printFormattedReg(mGpuDevice->getRegSpec("SQ_WAVE_GPR_ALLOC"), data->gpr_alloc);
        printFormattedReg(mGpuDevice->getRegSpec("SQ_WAVE_LDS_ALLOC"), data->lds_alloc);
        printFormattedReg(mGpuDevice->getRegSpec("SQ_WAVE_TRAPSTS"), data->trapsts);
        printFormattedReg(mGpuDevice->getRegSpec("SQ_WAVE_IB_STS"), data->ib_sts);
        printFormattedReg(mGpuDevice->getRegSpec("SQ_WAVE_TBA_LO"), data->tba_lo);
        printFormattedReg(mGpuDevice->getRegSpec("SQ_WAVE_TBA_HI"), data->tba_hi);
        printFormattedReg(mGpuDevice->getRegSpec("SQ_WAVE_TMA_LO"), data->tma_lo);
        printFormattedReg(mGpuDevice->getRegSpec("SQ_WAVE_TMA_HI"), data->tma_hi);
        printFormattedReg(mGpuDevice->getRegSpec("SQ_WAVE_IB_DBG0"), data->ib_dbg0);
        printFormattedReg(mGpuDevice->getRegSpec("SQ_WAVE_M0"), data->m0);
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
