#pragma once

#include "RegSpec.h"

namespace gputool {

const RegSpec gfx_v8_regs[] = {
    {"SMC_IND_INDEX_11", 0x1ac, {}},
    {"SMC_IND_DATA_11", 0x1ad, {}},
    {"CP_DEVICE_ID", 0x304b, {{"ID", 0xffffffff, 0x0}}},
    {"CB_HW_CONTROL", 0x2684, {{"CTRL", 0xffffffff, 0x0}}},
    {"CB_BLEND_RED", 0xa105, {{"BLEND_RED", 0xffffffff, 0x0}}},
    {"CB_COLOR_CONTROL", 0xa202, {{"OVERWRITE_COMBINER_DISABLE", 0x1, 0x0}}}};
};
