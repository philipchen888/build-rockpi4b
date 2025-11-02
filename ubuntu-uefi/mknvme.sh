#!/bin/sh
BOOT=/dev/sda
dd if=/dev/zero of=${BOOT} bs=1M count=0 seek=8192
parted -s ${BOOT} mklabel gpt
parted -s ${BOOT} -- unit s mkpart boot2 ext4 32768 -34s
