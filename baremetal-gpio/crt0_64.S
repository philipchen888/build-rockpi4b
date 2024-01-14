/*
 * crt0 - C-runtime startup Code for AArch64 U-Boot
 *
 * (C) Copyright 2013
 * David Feng <fenghua@phytium.com.cn>
 *
 * (C) Copyright 2012
 * Albert ARIBAUD <albert.u.boot@aribaud.net>
 *
 * SPDX-License-Identifier:	GPL-2.0+
 */

/*
 * This file handles the target-independent stages of the U-Boot
 * start-up where a C runtime environment is needed. Its entry point
 * is _main and is branched into from the target's start.S file.
 *
 * _main execution sequence is:
 *
 * 1. Set up initial environment for calling board_init_f().
 *    This environment only provides a stack and a place to store
 *    the GD ('global data') structure, both located in some readily
 *    available RAM (SRAM, locked cache...). In this context, VARIABLE
 *    global data, initialized or not (BSS), are UNAVAILABLE; only
 *    CONSTANT initialized data are available. GD should be zeroed
 *    before board_init_f() is called.
 *
 * 2. Call board_init_f(). This function prepares the hardware for
 *    execution from system RAM (DRAM, DDR...) As system RAM may not
 *    be available yet, , board_init_f() must use the current GD to
 *    store any data which must be passed on to later stages. These
 *    data include the relocation destination, the future stack, and
 *    the future GD location.
 *
 * 3. Set up intermediate environment where the stack and GD are the
 *    ones allocated by board_init_f() in system RAM, but BSS and
 *    initialized non-const data are still not available.
 *
 * 4a.For U-Boot proper (not SPL), call relocate_code(). This function
 *    relocates U-Boot from its current location into the relocation
 *    destination computed by board_init_f().
 *
 * 4b.For SPL, board_init_f() just returns (to crt0). There is no
 *    code relocation in SPL.
 *
 * 5. Set up final environment for calling board_init_r(). This
 *    environment has BSS (initialized to 0), initialized non-const
 *    data (initialized to their intended value), and stack in system
 *    RAM (for SPL moving the stack and GD into RAM is optional - see
 *    CONFIG_SPL_STACK_R). GD has retained values set by board_init_f().
 *
 * TODO: For SPL, implement stack relocation on AArch64.
 *
 * 6. For U-Boot proper (not SPL), some CPUs have some work left to do
 *    at this point regarding memory, so call c_runtime_cpu_setup.
 *
 * 7. Branch to board_init_r().
 *
 * For more information see 'Board Initialisation Flow in README.
 */
#define GD_START_ADDR_SP 120
#define GENERATED_GBL_DATA_SIZE 352
#define GENERATED_BD_INFO_SIZE 160
#define GD_SIZE 352
#define GD_BD 0
#define GD_MALLOC_BASE 248
#define GD_RELOCADDR 88
#define GD_RELOC_OFF 128
#define PM_CTX_SIZE 136
#define PM_CTX_PHYS 344
#define CONFIG_SYS_INIT_SP_ADDR  0x00b00000

	.globl _main
_main:
/*
 * Set up initial C runtime environment and call board_init_f(0).
 */
	ldr	x0, =0x00b00000  // __StackTop
	bic	sp, x0, #0xf	/* 16-byte alignment for ABI compliance */
	mov	x0, sp
	mov	sp, x0
	/* set up gd here, outside any C code */
	mov	x18, x0

	mov	x0, #0
        bl      c_runtime_cpu_setup

	/* set up gd here, outside any C code */
	mov	x18, x0
	/*
	 * Perform 'sp = (x0 != NULL) ? x0 : sp' while working
	 * around the constraint that conditional moves can not
	 * have 'sp' as an operand
	 */
	mov	x1, sp
	cmp	x0, #0
	csel	x0, x0, x1, ne
	mov	sp, x0

/*
 * Clear BSS section
 */
	ldr	x0, =__bss_start		/* this is auto-relocated! */
	ldr	x1, =__bss_end			/* this is auto-relocated! */
clear_loop:
	str	xzr, [x0], #8
	cmp	x0, x1
	b.lo	clear_loop

	/* call board_init_r(gd_t *id, ulong dest_addr) */
	mov	x0, x18				/* gd_t */
	ldr	x1, [x18, #GD_RELOCADDR]	/* dest_addr */
	b	main			/* PC relative jump */
