#!/bin/sh
BOOT=/dev/sda
dd if=/dev/zero of=${BOOT} bs=1M count=1024 conv=notrunc,fsync
parted -s ${BOOT} mklabel gpt
parted -s ${BOOT} unit s mkpart loader1 ext4 64 8063
parted -s ${BOOT} unit s mkpart loader2 ext4 16384 24575
parted -s ${BOOT} unit s mkpart trust ext4 24576 32767
parted -s ${BOOT} unit s mkpart boot fat32 32768 9830399
parted -s ${BOOT} set 4 boot on
