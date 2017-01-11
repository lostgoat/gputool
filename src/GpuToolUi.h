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

#pragma once

#include <stdint.h>
#include <memory>

namespace gputool
{
// ---------------------------------------------------------------------------

struct RegSpec;
class UserInput;
class AmdGpuDevice;

class GpuToolUi
{
  public:
    GpuToolUi();
    ~GpuToolUi();
    int run();

  private:
    std::unique_ptr<UserInput> getNextInput();
    int dispatch(const UserInput &input);
    int doRegOp(const UserInput &input);

    std::unique_ptr<AmdGpuDevice> mGpuDevice;
};
// ---------------------------------------------------------------------------
};  // namespace gputool
