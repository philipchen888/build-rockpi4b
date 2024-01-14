/*
 * (C) Copyright 2013
 * David Feng <fenghua@phytium.com.cn>
 *
 * (C) Copyright 2002
 * Gary Jennejohn, DENX Software Engineering, <garyj@denx.de>
 *
 * (C) Copyright 2010
 * Texas Instruments, <www.ti.com>
 *	Aneesh V <aneesh@ti.com>
 *
 * SPDX-License-Identifier:	GPL-2.0+
 */

MEMORY { .sram : ORIGIN = 0x0, LENGTH = 0x100000 }
MEMORY { .sdram : ORIGIN = 0x00400000, LENGTH = 0x2000 }

OUTPUT_FORMAT("elf64-littleaarch64", "elf64-littleaarch64", "elf64-littleaarch64")
OUTPUT_ARCH(aarch64)
ENTRY(_start)
SECTIONS
{
	.text : {
		. = ALIGN(8);
		*(.__image_copy_start)
		*(.text*)
	} >.sram

	.rodata : {
		. = ALIGN(8);
		*(SORT_BY_ALIGNMENT(SORT_BY_NAME(.rodata*)))
	} >.sram

	.data : {
		. = ALIGN(8);
		*(.data*)
	} >.sram

	.u_boot_list : {
		. = ALIGN(8);
		KEEP(*(SORT(.u_boot_list*)));
	} >.sram

	.image_copy_end : {
		. = ALIGN(8);
		*(.__image_copy_end)
	} >.sram

	.end : {
		. = ALIGN(8);
		*(.__end)
	} >.sram

	_image_binary_end = .;

	.bss_start (NOLOAD) : {
		. = ALIGN(8);
		__bss_start = .;
		KEEP(*(.__bss_start));
	} >.sdram

	.bss (NOLOAD) : {
		*(.bss*)
		 . = ALIGN(8);
	} >.sdram

	.bss_end (NOLOAD) : {
		__bss_end = .;
		KEEP(*(.__bss_end));
	} >.sdram

	/DISCARD/ : { *(.dynsym) }
	/DISCARD/ : { *(.dynstr*) }
	/DISCARD/ : { *(.dynamic*) }
	/DISCARD/ : { *(.plt*) }
	/DISCARD/ : { *(.interp*) }
	/DISCARD/ : { *(.gnu*) }
}
