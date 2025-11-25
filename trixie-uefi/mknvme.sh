#!/bin/sh
BOOT=/dev/sda
dd if=/dev/zero of=${BOOT} bs=1M count=1024 conv=notrunc,fsync
parted -s ${BOOT} mklabel gpt
parted -s ${BOOT} -- unit s mkpart rootfs ext4 32768 -34s
