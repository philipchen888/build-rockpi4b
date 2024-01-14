/*
 * (C) Copyright 2013
 * David Feng <fenghua@phytium.com.cn>
 *
 * SPDX-License-Identifier:	GPL-2.0+
 */

#define GICD_CTLRy            0x0000
#define GICD_TYPERy           0x0004
#define GICD_IGROUPRny        0x0080
#define GICC_BASEy            0xFF812000
#define GICD_BASEy            0xFF811000
#define GICC_CTLRy            0x0000
#define GICD_ISENABLERny      0x0100
#define GICC_PMRy             0x0004
#define CONFIG_SYS_TEXT_BASE  0x200000

.macro  switch_el, xreg, el3_label, el2_label, el1_label
        mrs     \xreg, CurrentEL
        cmp     \xreg, 0xc
        b.eq    \el3_label
        cmp     \xreg, 0x8
        b.eq    \el2_label
        cmp     \xreg, 0x4
        b.eq    \el1_label
.endm

/*************************************************************************
 *
 * Startup Code (reset vector)
 *
 *************************************************************************/

.globl	_start
_start:
	b       1f
1:
	b	reset

	.align 3

.globl	_TEXT_BASE
_TEXT_BASE:
	.quad	CONFIG_SYS_TEXT_BASE

/*
 * These are defined in the linker script.
 */
.globl	_bss_start_ofs
_bss_start_ofs:
	.quad	__bss_start - _start

.globl	_bss_end_ofs
_bss_end_ofs:
	.quad	__bss_end - _start

reset:

	bl reset_sctrl

        adr     x0, vectors
        switch_el x1, 3f, 2f, 1f
3:      msr     vbar_el3, x0
        mrs     x0, scr_el3
        orr     x0, x0, #0xf                    /* SCR_EL3.NS|IRQ|FIQ|EA */
        msr     scr_el3, x0
        msr     cptr_el3, xzr                   /* Enable FP/SIMD */
        b       0f
2:      msr     vbar_el2, x0
        mov     x0, #0x33ff
        msr     cptr_el2, x0                    /* Enable FP/SIMD */
        b       0f
1:      msr     vbar_el1, x0
        mov     x0, #3 << 20
        msr     cpacr_el1, x0                   /* Enable FP/SIMD */
0:
        bl      gic_init_secure
        bl      gic_init_secure_percpu
        // Setting HCR_EL2.TGE AMO IMO FMO for exception rounting to EL2
        mrs     x0, CurrentEL
        cmp     x0, 0x8
        b.ne    end1

        mrs     x9, hcr_el2
        orr     x9, x9, #(7 << 3)
        orr     x9, x9, #(1 << 27)
        msr     hcr_el2, x9
end1:
	bl	_main

reset_sctrl:
	switch_el x1, 3f, 2f, 1f
3:
	mrs	x0, sctlr_el3
	b	0f
2:
	mrs	x0, sctlr_el2
	b	0f
1:
	mrs	x0, sctlr_el1

0:
	ldr	x1, =0xfdfffffa
	and	x0, x0, x1

	switch_el x1, 6f, 5f, 4f
6:
	msr	sctlr_el3, x0
	b	7f
5:
	msr	sctlr_el2, x0
	b	7f
4:
	msr	sctlr_el1, x0

7:
	dsb	sy
	isb
	b	__asm_invalidate_tlb_all
	ret

        .globl __asm_invalidate_tlb_all
__asm_invalidate_tlb_all:
        switch_el x9, 3f, 2f, 1f
3:      tlbi    alle3
        dsb     sy
        isb
        b       0f
2:      tlbi    alle2
        dsb     sy
        isb
        b       0f
1:      tlbi    vmalle1
        dsb     sy
        isb
0:
        ret

	.globl  c_runtime_cpu_setup
c_runtime_cpu_setup:
        /* Relocate vBAR */
        adr     x0, vectors
        switch_el x1, 3f, 2f, 1f
3:      msr     vbar_el3, x0
        b       0f
2:      msr     vbar_el2, x0
        b       0f
1:      msr     vbar_el1, x0
0:

        ret

	.globl  gic_init_secure
gic_init_secure:
	ldr     x0, =GICD_BASEy
        mov     w9, #0x3
        str     w9, [x0, GICD_CTLRy]
        ldr     w9, [x0, GICD_TYPERy]
        and     w10, w9, #0x1f
        cbz     w10, 1f
        add     x11, x0, GICD_IGROUPRny
        mov     w9, #~0
        str     w9, [x11], #0x4
0:      str     w9, [x11], #0x4
        sub     w10, w10, #0x1
        cbnz    w10, 0b

        ldr     x1, =GICC_BASEy
        mov     w0, #3
        str     w0, [x1]

        mov     w0, #1 << 7
        str     w0, [x1, #4]
1:
        ret

	.globl  gic_init_secure_percpu
gic_init_secure_percpu:
	ldr     x0, =GICD_BASEy 
        ldr     x1, =GICC_BASEy
        mov     w9, #~0                 /* Config SGIs and PPIs as Grp1 */
        str     w9, [x0, GICD_IGROUPRny] /* GICD_IGROUPR0 */
        mov     w9, #0x1                /* Enable SGI 0 */
        str     w9, [x0, GICD_ISENABLERny]

        /* Initialize Cpu Interface */
        mov     w9, #0x1e7              /* Disable IRQ/FIQ Bypass & */
                                        /* Enable Ack Group1 Interrupt & */
                                        /* EnableGrp0 & EnableGrp1 */
        str     w9, [x1, GICC_CTLRy]     /* Secure GICC_CTLR */

        mov     w9, #0x1 << 7           /* Non-Secure access to GICC_PMR */
        str     w9, [x1, GICC_PMRy]

        ret

	.balign  0x400
	.globl  irqflagrx
irqflagrx: 
	.word   0
	.globl  irqdatarx
irqdatarx:
	.word   0
