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
        UC_REG_SRBM_READ,
        UC_REG_SRBM_WRITE,
        UC_PRINT_GCA_INFO,
        UC_PRINT_HQD_INFO,
        UC_PRINT_WAVE_INFO,
        UC_PRINT_WAVE_PRIORITY_INFO,
        UC_BAD_INPUT,
        UC_EXIT,
    } UserCommand;

  public:
    UserInput(std::string command);
    ~UserInput(){};

    std::string mOriginalInput;
    UserCommand mCommand;
    std::string mRegName;
    std::string mFieldName;
    uint32_t mRegValue;

    /* SRBM info */
    uint32_t mMe;
    uint32_t mPipe;
    uint32_t mQueue;
    uint32_t mVmid;

  private:
    std::istringstream mParseStream;
    uint32_t getInt();
    std::string getString();
};

uint32_t UserInput::getInt()
{
    std::string str = getString();
    if (str.find('x') != std::string::npos)
        return std::strtoul(str.c_str(), NULL, 16);
    else
        return std::atoi(str.c_str());
}

std::string UserInput::getString()
{
    std::string token;
    std::getline(mParseStream, token, ' ');
    return token;
}

UserInput::UserInput(std::string command)
    : mOriginalInput(command), mCommand(UC_BAD_INPUT), mRegName(""), mRegValue(0), mParseStream(command + " ")
{
    std::string strInput;

    /* Barebones parser */
    strInput = getString();
    if (strInput == "read") {
        mCommand = UC_REG_READ;
        mRegName = getString();
    } else if (strInput == "write") {
        mCommand = UC_REG_WRITE;
        mRegName = getString();
        mRegValue = getInt();
    } else if (strInput == "srbm_read") {
        mCommand = UC_REG_SRBM_READ;
        mMe = getInt();
        mPipe = getInt();
        mQueue = getInt();
        mVmid = getInt();
        mRegName = getString();
    } else if (strInput == "srbm_write") {
        mCommand = UC_REG_SRBM_WRITE;
        mMe = getInt();
        mPipe = getInt();
        mQueue = getInt();
        mVmid = getInt();
        mRegName = getString();
        mRegValue = getInt();
    } else if (strInput == "gca_info") {
        mCommand = UC_PRINT_GCA_INFO;
    } else if (strInput == "wave_priority") {
        mCommand = UC_PRINT_WAVE_PRIORITY_INFO;
    } else if (strInput == "wave_info") {
        mCommand = UC_PRINT_WAVE_INFO;
    } else if (strInput == "hqd_info") {
        mCommand = UC_PRINT_HQD_INFO;
    } else if (strInput == "exit" || strInput == "quit") {
        mCommand = UC_EXIT;
    } else {
        mCommand = UC_BAD_INPUT;
    }

    size_t pos = mRegName.find('.');
    if (pos != std::string::npos) {
        std::string tmp = mRegName;
        mRegName = tmp.substr(0, pos);
        mFieldName = tmp.substr(pos + 1, tmp.length() - pos);
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

int GpuToolUi::runSingle(std::string command)
{
    std::unique_ptr<UserInput> input = util::make_unique<UserInput>(command);

    return dispatch(*input);
}

int GpuToolUi::dispatch(const UserInput &input)
{
    switch (input.mCommand) {
        case UserInput::UC_REG_SRBM_READ:
        case UserInput::UC_REG_SRBM_WRITE:
        case UserInput::UC_REG_READ:
        case UserInput::UC_REG_WRITE:
            doRegOp(input);
            break;
        case UserInput::UC_PRINT_GCA_INFO:
            doPrintGcaInfo(input);
            break;
        case UserInput::UC_PRINT_HQD_INFO:
            doPrintHqdInfo(input);
            break;
        case UserInput::UC_PRINT_WAVE_INFO:
            doPrintWaveInfo(input);
            break;
        case UserInput::UC_PRINT_WAVE_PRIORITY_INFO:
            doPrintWavePriorityInfo(input);
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
    std::vector<const amdregdb::RegSpec *> regSpec =
        mGpuDevice->getRegSpecs(input.mRegName);
    for (auto const &reg : regSpec) {
        switch (input.mCommand) {
            case UserInput::UC_REG_WRITE:
                if (input.mFieldName.empty()) {
                    mGpuDevice->write(*reg, input.mRegValue);
                } else {
                    mGpuDevice->writeField(*reg, input.mFieldName, input.mRegValue);
                }
                /* fallthrough */
            case UserInput::UC_REG_READ:
                printFormattedReg(reg, mGpuDevice->read(*reg));
                break;
            case UserInput::UC_REG_SRBM_WRITE:
                if (input.mFieldName.empty()) {
                    mGpuDevice->srbmWrite(reg->name, input.mMe, input.mPipe, input.mQueue, input.mVmid, input.mRegValue);
                } else {
                    mGpuDevice->srbmWriteField(reg->name, input.mFieldName, input.mMe, input.mPipe, input.mQueue, input.mVmid, input.mRegValue);
                }
                /* fallthrough */
            case UserInput::UC_REG_SRBM_READ:
                printFormattedReg(reg, mGpuDevice->srbmRead(reg->name, input.mMe, input.mPipe, input.mQueue, input.mVmid));
                break;
            default:
                return -1;
        }
    }

    return 0;
}

int GpuToolUi::doPrintGcaInfo(const UserInput &input)
{
#define DUMP_FIELD(name) printf("    %s: 0x%x\n", #name, mGpuDevice->mGcaInfo.name)
    DUMP_FIELD(version);
    DUMP_FIELD(max_shader_engines);
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

int GpuToolUi::doPrintHqdInfo(const UserInput &input)
{
    std::vector<std::string> hqdRegs = {
        "CP_HQD_ACTIVE",
        "CP_HQD_VMID",
        "CP_HQD_PIPE_PRIORITY",
        "CP_HQD_QUEUE_PRIORITY",
        "CP_HQD_QUANTUM",
        "CP_HQD_ERROR",
    };

    /* Maybe get the loop info from gca_info? */
    for (int iMe = 1; iMe < 2; ++iMe) {
        for (int iPipe = 0; iPipe < 4; ++iPipe) {
            for (int iQueue = 0; iQueue < 8; ++iQueue) {
                uint32_t hqdActive = mGpuDevice->srbmRead("CP_HQD_ACTIVE", iMe, iPipe, iQueue, 0);
                if (hqdActive) {
                    printf("HQD ME(%d) PIPE(%d) QUEUE(%d):\n", iMe, iPipe, iQueue);
                    for (auto const &regName : hqdRegs) {
                        uint32_t val = mGpuDevice->srbmRead(regName, iMe, iPipe, iQueue, 0);
                        printFormattedReg(mGpuDevice->getRegSpec(regName), val);
                    }
                }
            }
        }
    }

    return 0;
}

int GpuToolUi::doPrintWaveInfo(const UserInput &input)
{
    std::vector<std::unique_ptr<WaveInfo>> waves = mGpuDevice->getWaveInfo();

    for (auto const &waveInfo : waves) {
        amddebugfs::wave_info *data = &waveInfo->mWaveInfo;

        printf("Wave SE(%d) SH(%d) CU(%d):\n", waveInfo->se, waveInfo->sh, waveInfo->cu);

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
        printf("\n");
    }

    return 0;
}

int GpuToolUi::doPrintWavePriorityInfo(const UserInput &input)
{
    std::vector<std::unique_ptr<WaveInfo>> waves = mGpuDevice->getWaveInfo();

    int numCus = 0;
    int numCusValid = 0;

    for (auto const &waveInfo : waves) {
        amddebugfs::wave_info *data = &waveInfo->mWaveInfo;
        numCus++;

        if (!mGpuDevice->getFieldAs("SQ_WAVE_STATUS", "VALID", data->status))
            continue;

        numCusValid++;
        printf("%d:%d:%d: VMID:0x%x SPI_PRIO: 0x%x USER_PRIO: 0x%x\n", waveInfo->se,
               waveInfo->sh, waveInfo->cu,
               mGpuDevice->getFieldAs("SQ_WAVE_HW_ID", "VM_ID", data->hw_id),
               mGpuDevice->getFieldAs("SQ_WAVE_STATUS", "SPI_PRIO", data->status),
               mGpuDevice->getFieldAs("SQ_WAVE_STATUS", "USER_PRIO", data->status));
    }

    printf("%d/%d CUs in use %d%%\n", numCusValid, numCus, numCusValid * 100 / numCus);
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
