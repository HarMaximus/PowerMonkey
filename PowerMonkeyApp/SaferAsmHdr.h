/*******************************************************************************
*  ______                            ______                 _
* (_____ \                          |  ___ \               | |
*  _____) )___   _ _ _   ____   ___ | | _ | |  ___   ____  | |  _  ____  _   _
* |  ____// _ \ | | | | / _  ) / __)| || || | / _ \ |  _ \ | | / )/ _  )| | | |
* | |    | |_| || | | |( (/ / | |   | || || || |_| || | | || |< (( (/ / | |_| |
* |_|     \___/  \____| \____)|_|   |_||_||_| \___/ |_| |_||_| \_)\____) \__  |
*                                                                       (____/
* Copyright (C) 2021 Ivan Dimkovic. All rights reserved.
*
* All trademarks, logos and brand names are the property of their respective
* owners. All company, product and service names used are for identification
* purposes only. Use of these names, trademarks and brands does not imply
* endorsement.
*
* SPDX-License-Identifier: Apache-2.0
* Full text of the license is available in project root directory (LICENSE)
*
* WARNING: This code is a proof of concept for educative purposes. It can
* modify internal computer configuration parameters and cause malfunctions or
* even permanent damage. It has been tested on a limited range of target CPUs
* and has minimal built-in failsafe mechanisms, thus making it unsuitable for
* recommended use by users not skilled in the art. Use it at your own risk.
*
*******************************************************************************/

#pragma once

#ifdef __cplusplus
extern "C" {
#endif

/*******************************************************************************
 * Below calls are implemented in SaferAsm.asm
 ******************************************************************************/

VOID stop_interrupts_on_this_cpu(VOID);
VOID resume_interrupts_on_this_cpu(VOID);

UINT64 safer_rdmsr64(
    const UINT32 msr_idx,
    UINT32* is_err);

UINT32 safer_wrmsr64(
    const UINT32 msr_idx,
    const UINT64 value);

UINT32 safer_mmio_read32(
  const UINT32 addr, 
  UINT32* is_err);

UINT32 safer_mmio_or32(
  const UINT32 addr, 
  const UINT32 value);

UINT32 safer_mmio_write32(
  const UINT32 addr,
  const UINT32 value);

VOID get_current_idtr(VOID* pidtr);

UINT32 get_pciex_base_addr(VOID);

UINT64 hlp_atomic_increment_u64(UINT64 *val);
UINT64 hlp_atomic_decrement_u64(UINT64* val);

UINT32 hlp_atomic_increment_u32(UINT32* val);
UINT32 hlp_atomic_decrement_u32(UINT32* val);

/*******************************************************************************
 * ISR entry points in SaferAsm.asm
 ******************************************************************************/

extern VOID* monkey_isr_0;
extern VOID* monkey_isr_1;
extern VOID* monkey_isr_2;
extern VOID* monkey_isr_3;
extern VOID* monkey_isr_4;
extern VOID* monkey_isr_5;
extern VOID* monkey_isr_6;
extern VOID* monkey_isr_7;
extern VOID* monkey_isr_8;
extern VOID* monkey_isr_9;
extern VOID* monkey_isr_10;
extern VOID* monkey_isr_11;
extern VOID* monkey_isr_12;
extern VOID* monkey_isr_13;
extern VOID* monkey_isr_14;
extern VOID* monkey_isr_15;
extern VOID* monkey_isr_16;
extern VOID* monkey_isr_17;
extern VOID* monkey_isr_18;
extern VOID* monkey_isr_19;
extern VOID* monkey_isr_20;
extern VOID* monkey_isr_21;
extern VOID* monkey_isr_22;
extern VOID* monkey_isr_23;
extern VOID* monkey_isr_24;
extern VOID* monkey_isr_25;
extern VOID* monkey_isr_26;
extern VOID* monkey_isr_27;
extern VOID* monkey_isr_28;
extern VOID* monkey_isr_29;
extern VOID* monkey_isr_30;
extern VOID* monkey_isr_31;

#ifdef __cplusplus
}
#endif