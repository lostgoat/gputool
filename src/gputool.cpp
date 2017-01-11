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
#include <util/util.h>
#include "GpuToolUi.h"

int main()
{
    int r = 0;

    printf("Starting gputool v%d.%d.%d\n", GPUTOOL_VERSION_MAJOR, GPUTOOL_VERSION_MINOR,
           GPUTOOL_VERSION_PATCH);

    try {
        auto ui = util::make_unique<gputool::GpuToolUi>();
        r = ui->run();
    } catch (...) {
        std::terminate();
    }

    return r;
}
