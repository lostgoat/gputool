gputool
=======

This is a tool for inspection of GPU HW, mainly intended for driver developers.

Supported Hardware
------------------

The underlying amdregdb contains the register spec format for all HW that amdgpu supports.

gputool currently supports:
 * POLARIS 10 (simple device id detection required for other hardware)

Supported Operations
--------------------

gputool currently supports:
 * Reading and decoding registers

Build
-----

NOTE: gputool requires std::uncaught_exceptions(). If your default toolchain
does not support it, then you can choose an alternate ony by setting CXX=...
in your environment.

```
mkdir build/
cd build/
cmake ..
make -j$(nproc)
```

Install
-------

Installing is optional, you can run gputool from the build folder.

```
cd build/
make install
```

Usage
-----

NOTE: gputool requires root privileges

```
sudo build/src/gputool <command>
OR
sudo gputool <command> (only if installed)
```

Commands
--------

Following is a list of supported commands:


###Register Read

The read command reads a register

```
read <regname>
```

Example:

```
andres@andres-vr:~/git/linux$ sudo gputool read TCC_CTRL
    TCC_CTRL: 0xf31fff7f
      MDC_SECTOR_SIZE: 0x0
      LATENCY_FIFO_SIZE: 0xf
      WRITEBACK_MARGIN: 0x7
      SRC_FIFO_SIZE: 0xf
      MDC_SIDEBAND_FIFO_SIZE: 0xf
      RATE: 0x3
      CACHE_SIZE: 0x3
      WB_OR_INV_ALL_VMIDS: 0x1
      METADATA_LATENCY_FIFO_SIZE: 0xf
      MDC_SIZE: 0x3
```
### Register Write

The write command writes to a register or register field

```
write <regname>.<optional fieldname> <value>

```

Example:

```
andres@andres-vr:~/git/gputool/build$ sudo gputool write TCC_CTRL.METADATA_LATENCY_FIFO_SIZE 0
    TCC_CTRL: 0xf31ff07f
      MDC_SECTOR_SIZE: 0x0
      LATENCY_FIFO_SIZE: 0xf
      WRITEBACK_MARGIN: 0x7
      SRC_FIFO_SIZE: 0xf
      MDC_SIDEBAND_FIFO_SIZE: 0xf
      RATE: 0x3
      CACHE_SIZE: 0x3
      WB_OR_INV_ALL_VMIDS: 0x1
      METADATA_LATENCY_FIFO_SIZE: 0x0
      MDC_SIZE: 0x3
```

### GCA Info

The gca_info command dumps some of the basic HW information

```
gca_info

```

Example:

```
andres@andres-vr:~/git/gputool/build$ sudo gputool gca_info
    version: 0x2
    ax_shader_engines: 0x4
    max_tile_pipes: 0x8
    max_cu_per_sh: 0x9
    max_sh_per_se: 0x1
    max_backends_per_se: 0x2
    max_texture_channel_caches: 0x8
    max_gprs: 0x100
    max_gs_threads: 0x20
    max_hw_contexts: 0x8
    sc_prim_fifo_size_frontend: 0x20
    sc_prim_fifo_size_backend: 0x100
    sc_hiz_tile_fifo_size: 0x30
    sc_earlyz_tile_fifo_size: 0x130
    num_tile_pipes: 0x8
    backend_enable_mask: 0xff
    mem_max_burst_length_bytes: 0x100
    mem_row_size_in_kb: 0x4
    shader_engine_tile_size: 0x20
    num_gpus: 0x1
    multi_gpu_tile_size: 0x40
    mc_arb_ramcfg: 0x60a2
    gb_addr_config: 0x22011003
    num_rbs: 0x8
    rev_id: 0x1
    pg_flags: 0x0
    cg_flags: 0x6000
    family: 0x82
    external_rev_id: 0x51
```

### Wave Info

The wave_info command dumps wave/CU information

```
wave_info

```

Example:

```
andres@andres-vr:~/git/gputool/build$ sudo gputool wave_info

Wave SE(0) SH(0) CU(0):
    SQ_WAVE_STATUS: 0x8010500
      VALID: 0x1
      SCC: 0x0
      PERF_EN: 0x0
      IN_TG: 0x0
      PRIV: 0x0
      MUST_EXPORT: 0x1
      TTRACE_EN: 0x0
      VCCZ: 0x1
      COND_DBG_USER: 0x0
      ECC_ERR: 0x0
      TTRACE_CU_EN: 0x0
      HALT: 0x0
      IN_BARRIER: 0x0
      USER_PRIO: 0x0
      EXECZ: 0x0
      INST_ATC: 0x0
      SKIP_EXPORT: 0x0
      COND_DBG_SYS: 0x0
      ALLOW_REPLAY: 0x0
      SPI_PRIO: 0x0
      TRAP_EN: 0x0
      EXPORT_RDY: 0x1
      TRAP: 0x0
    SQ_WAVE_PC_LO: PC_LO 0x1ef136c
    SQ_WAVE_PC_HI: PC_HI 0x1
    SQ_WAVE_EXEC_LO: EXEC_LO 0xffffffff
    SQ_WAVE_EXEC_HI: EXEC_HI 0xffffffff
    SQ_WAVE_HW_ID: 0x8106800
      PIPE_ID: 0x0
      SH_ID: 0x0
      ME_ID: 0x0
      TG_ID: 0x0
      QUEUE_ID: 0x0
      STATE_ID: 0x1
      SIMD_ID: 0x0
      SE_ID: 0x3
      CU_ID: 0x8
      WAVE_ID: 0x0
      VM_ID: 0x1
    SQ_WAVE_INST_DW0: INST_DW0 0xbf8c0f70
    SQ_WAVE_INST_DW1: INST_DW1 0x2240511
    SQ_WAVE_GPR_ALLOC: 0x2000500
      VGPR_BASE: 0x0
      SGPR_SIZE: 0x2
      VGPR_SIZE: 0x5
      SGPR_BASE: 0x0
    SQ_WAVE_LDS_ALLOC: 0x2000
      LDS_SIZE: 0x2
      LDS_BASE: 0x0
    SQ_WAVE_TRAPSTS: 0x20000000
      DP_RATE: 0x1
      EXCP_CYCLE: 0x0
      SAVECTX: 0x0
      EXCP: 0x0
    SQ_WAVE_IB_STS: 0x1
      VM_CNT: 0x1
      RCNT: 0x0
      EXP_CNT: 0x0
      VALU_CNT: 0x0
      FIRST_REPLAY: 0x0
      LGKM_CNT: 0x0
    SQ_WAVE_TBA_LO: ADDR_LO 0x0
    SQ_WAVE_TBA_HI: ADDR_HI 0x0
    SQ_WAVE_TMA_LO: ADDR_LO 0x0
    SQ_WAVE_TMA_HI: ADDR_HI 0x0
    SQ_WAVE_IB_DBG0: 0x803
      IBUF_ST: 0x3
      PC_INVALID: 0x0
      IS_HYB: 0x0
      INST_STR_ST: 0x0
      NEED_KILL_IFETCH: 0x0
      HYB_CNT: 0x0
      KILL: 0x0
      ECC_ST: 0x0
      IBUF_RPTR: 0x0
      MISC_CNT: 0x0
      NEED_NEXT_DW: 0x0
      IBUF_WPTR: 0x2
      NO_PREFETCH_CNT: 0x0
    SQ_WAVE_M0: M0 0x80000000

[snipped] /* repeats for all other CUs */

```

### Wave Priority

The wave_priority command prints a summary of currently executing wave priorities

```
wave_priority
```

Example:
```
andres@andres-vr:~/git/gputool/build$ sudo gputool wave_priority
0:0:0: VMID:0x5 SPI_PRIO: 0x2 USER_PRIO: 0x0
0:0:1: VMID:0x5 SPI_PRIO: 0x2 USER_PRIO: 0x0
0:0:2: VMID:0x5 SPI_PRIO: 0x2 USER_PRIO: 0x0
0:0:3: VMID:0x1 SPI_PRIO: 0x0 USER_PRIO: 0x0
0:0:4: VMID:0x1 SPI_PRIO: 0x0 USER_PRIO: 0x0
0:0:5: VMID:0x1 SPI_PRIO: 0x0 USER_PRIO: 0x0
0:0:6: VMID:0x1 SPI_PRIO: 0x0 USER_PRIO: 0x0
0:0:7: VMID:0x1 SPI_PRIO: 0x0 USER_PRIO: 0x0
0:0:8: VMID:0x1 SPI_PRIO: 0x0 USER_PRIO: 0x0
1:0:0: VMID:0x1 SPI_PRIO: 0x0 USER_PRIO: 0x0
1:0:1: VMID:0x1 SPI_PRIO: 0x0 USER_PRIO: 0x0
1:0:2: VMID:0x1 SPI_PRIO: 0x0 USER_PRIO: 0x0
1:0:3: VMID:0x1 SPI_PRIO: 0x0 USER_PRIO: 0x0
1:0:4: VMID:0x1 SPI_PRIO: 0x0 USER_PRIO: 0x0
1:0:5: VMID:0x1 SPI_PRIO: 0x2 USER_PRIO: 0x0
1:0:6: VMID:0x1 SPI_PRIO: 0x0 USER_PRIO: 0x0
1:0:7: VMID:0x5 SPI_PRIO: 0x2 USER_PRIO: 0x0
1:0:8: VMID:0x1 SPI_PRIO: 0x0 USER_PRIO: 0x0
2:0:0: VMID:0x1 SPI_PRIO: 0x0 USER_PRIO: 0x0
2:0:1: VMID:0x1 SPI_PRIO: 0x0 USER_PRIO: 0x0
2:0:2: VMID:0x1 SPI_PRIO: 0x0 USER_PRIO: 0x0
2:0:3: VMID:0x1 SPI_PRIO: 0x0 USER_PRIO: 0x0
2:0:4: VMID:0x1 SPI_PRIO: 0x0 USER_PRIO: 0x0
2:0:5: VMID:0x1 SPI_PRIO: 0x0 USER_PRIO: 0x0
2:0:6: VMID:0x5 SPI_PRIO: 0x2 USER_PRIO: 0x0
2:0:7: VMID:0x1 SPI_PRIO: 0x0 USER_PRIO: 0x0
3:0:0: VMID:0x5 SPI_PRIO: 0x2 USER_PRIO: 0x0
3:0:1: VMID:0x5 SPI_PRIO: 0x2 USER_PRIO: 0x0
3:0:2: VMID:0x5 SPI_PRIO: 0x2 USER_PRIO: 0x0
3:0:3: VMID:0x5 SPI_PRIO: 0x2 USER_PRIO: 0x0
3:0:4: VMID:0x5 SPI_PRIO: 0x2 USER_PRIO: 0x0
3:0:5: VMID:0x5 SPI_PRIO: 0x2 USER_PRIO: 0x0
3:0:6: VMID:0x5 SPI_PRIO: 0x2 USER_PRIO: 0x0
3:0:7: VMID:0x5 SPI_PRIO: 0x2 USER_PRIO: 0x0
3:0:8: VMID:0x1 SPI_PRIO: 0x0 USER_PRIO: 0x0
35/36 CUs in use 97%
```

### SRBM Register Read

The srbm_read command reads an SRBM register. The me/pipe/queue/vmid
information can be retreived from the wave_info command.

```
srbm_read <me> <pipe> <queue> <vmid> <regname>
```

Example:
```
andres@andres-vr:~/git/gputool/build$ sudo gputool srbm_read 1 0 0 0 COMPUTE_PGM_RSRC1
    SRBM_GFX_CNTL: 0x4
      QUEUEID: 0x0
      VMID: 0x0
      PIPEID: 0x0
      MEID: 0x1
    SRBM_GFX_CNTL: 0x0
      QUEUEID: 0x0
      VMID: 0x0
      PIPEID: 0x0
      MEID: 0x0
    COMPUTE_PGM_RSRC1: 0x2c0c82
      BULKY: 0x0
      FLOAT_MODE: 0xc0
      CDBG_USER: 0x0
      IEEE_MODE: 0x0
      PRIORITY: 0x3
      SGPRS: 0x2
      DEBUG_MODE: 0x0
      VGPRS: 0x2
      DX10_CLAMP: 0x1
      PRIV: 0x0
```

#### Tip for retreiving SRBM info

```
sudo gputool wave_info | egrep 'Wave|VALID|ME|PIPE|VM_ID|QUEUE'
```

### SRBM Register Write

The srbm_write command reads an SRBM register.

```
srbm_write <me> <pipe> <queue> <vmid> <regname>
```

Extra Build Targets
-------------------

The following extra build targets are supported:
 * genregs: Updated amdregdb from an amdgpu linux tree (needs improvement)
 * format: formats the code in place
 * cppcheck: run some basic static analysis on the code
