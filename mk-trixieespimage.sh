#!/bin/bash -e

BOOTIMAGE="linaro-esp.img"
BOOT_UUID=95E46EA5

echo Making esp!

if [ -e ${BOOTIMAGE} ]; then
	rm ${BOOTIMAGE}
fi

echo Format boot to vfat
mkfs.vfat -n "boot" -i ${BOOT_UUID} -S 512 -C ${BOOTIMAGE} 512000

mmd -i ${BOOTIMAGE} ::/EFI
mmd -i ${BOOTIMAGE} ::/EFI/boot
mmd -i ${BOOTIMAGE} ::/EFI/debian
mcopy -i ${BOOTIMAGE} -s ../linux/patches/trixie/grubaa64.efi ::/EFI/boot/bootaa64.efi
mcopy -i ${BOOTIMAGE} -s ../linux/patches/trixie/grubaa64.efi ::/EFI/debian
mcopy -i ${BOOTIMAGE} -s ../linux/patches/trixie/grub.cfg ::/EFI/debian

echo Boot Image: ${BOOTIMAGE}
