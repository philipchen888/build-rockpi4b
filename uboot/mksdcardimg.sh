#!/bin/sh
BOOT=./sdcard.img
\rm ${BOOT}
dd if=/dev/zero of=${BOOT} bs=1M count=0 seek=1024
parted -s ${BOOT} mklabel gpt
parted -s ${BOOT} unit s mkpart loader1 64 8063
parted -s ${BOOT} unit s mkpart loader2 16384 24575
parted -s ${BOOT} unit s mkpart trust 24576 32767
dd if=../uboot/out/idbloader.img of=${BOOT} seek=64 conv=notrunc,fsync
dd if=../uboot/out/uboot.img of=${BOOT} seek=16384 conv=notrunc,fsync
dd if=../uboot/out/trust.img of=${BOOT} seek=24576 conv=notrunc,fsync
