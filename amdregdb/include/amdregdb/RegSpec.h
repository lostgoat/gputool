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

#include <stdint.h>
#include <vector>
#include <string>

namespace amdregdb
{
// ---------------------------------------------------------------------------

typedef int RegId;

typedef struct RegField {
    const char *name;
    const uint32_t mask;
    const uint32_t shift;
} RegField;

typedef struct RegSpec {
    const char *name;
    const uint32_t offset;
    std::vector<RegField> fields;
} RegSpec;

typedef struct RegBlock {
    const char *name;
	const RegSpec *regs;
	const int size;
} RegBlock;

// ---------------------------------------------------------------------------
};  // namespace amdregdb
