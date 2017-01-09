/*
 * Copyright (C) 2017  Andres Rodriguez
 *                     Valve Corporation
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

#include <gputool/config.h>
#include <stdio.h>
#include <map>
#include <vector>
#include "util.h"

namespace gputool
{
typedef int RegisterId;

typedef struct RegisterField {
    const char *name;
    const uint32_t mask;
    const uint32_t shift;
} RegisterField;

typedef struct Register {
    const char *name;
    const uint64_t offset;
    std::vector<RegisterField> fields;
} Register;

static const Register gfx_v8_regs[] = {
    {"SMC_IND_INDEX_11", 0x1ac, {}},
    {"SMC_IND_DATA_11", 0x1ad, {}},
    {"CB_BLEND_RED", 0xa105, {{"BLEND_RED", 0xffffffff, 0x0}}},
    {"CB_COLOR_CONTROL", 0xa202, {{"OVERWRITE_COMBINER_DISABLE", 0x1, 0x0}}}};

std::map<std::string, RegisterId> searchDb;

}  // namespace gputool

int doStuff()
{
    SCOPE_EXIT { printf("SCOPE_EXIT\n"); };
    SCOPE_SUCCESS { printf("SCOPE_SUCCESS\n"); };
    SCOPE_FAIL { printf("SCOPE_FAIL\n"); };
	util::die("bad stuff\n");
    return 0;
}

int main()
{
    printf("Hello world %d.%d\n", GPUTOOL_VERSION_MAJOR, GPUTOOL_VERSION_MINOR);
    try {
        doStuff();
    } catch (...) {
		std::terminate();
    }
    return 0;
}
