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

namespace amddebugfs
{
// ---------------------------------------------------------------------------

typedef struct gca_info {
    uint32_t version;
    uint32_t max_shader_engines;
    uint32_t max_tile_pipes;
    uint32_t max_cu_per_sh;
    uint32_t max_sh_per_se;
    uint32_t max_backends_per_se;
    uint32_t max_texture_channel_caches;
    uint32_t max_gprs;
    uint32_t max_gs_threads;
    uint32_t max_hw_contexts;
    uint32_t sc_prim_fifo_size_frontend;
    uint32_t sc_prim_fifo_size_backend;
    uint32_t sc_hiz_tile_fifo_size;
    uint32_t sc_earlyz_tile_fifo_size;
    uint32_t num_tile_pipes;
    uint32_t backend_enable_mask;
    uint32_t mem_max_burst_length_bytes;
    uint32_t mem_row_size_in_kb;
    uint32_t shader_engine_tile_size;
    uint32_t num_gpus;
    uint32_t multi_gpu_tile_size;
    uint32_t mc_arb_ramcfg;
    uint32_t gb_addr_config;
    uint32_t num_rbs;

    uint32_t rev_id;
    uint32_t pg_flags;
    uint32_t cg_flags;

    uint32_t family;
    uint32_t external_rev_id;
} gca_info;

typedef struct wave_info {
    uint32_t version;
    uint32_t status;
    uint32_t pc_lo;
    uint32_t pc_hi;
    uint32_t exec_lo;
    uint32_t exec_hi;
    uint32_t hw_id;
    uint32_t inst_dw0;
    uint32_t inst_dw1;
    uint32_t gpr_alloc;
    uint32_t lds_alloc;
    uint32_t trapsts;
    uint32_t ib_sts;
    uint32_t tba_lo;
    uint32_t tba_hi;
    uint32_t tma_lo;
    uint32_t tma_hi;
    uint32_t ib_dbg0;
    uint32_t m0;
} wave_info;

// ---------------------------------------------------------------------------
};  // namespace gputool
