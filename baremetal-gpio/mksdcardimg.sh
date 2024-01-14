#!/bin/sh
BOOT=./sdcard.img
\rm ${BOOT}
dd if=/dev/zero of=${BOOT} bs=1M count=0 seek=32
parted -s ${BOOT} mklabel gpt
parted -s ${BOOT} unit s mkpart loader1 64 8063
parted -s ${BOOT} unit s mkpart loader2 16384 24575
parted -s ${BOOT} unit s mkpart trust 24576 32767
ROOT_UUID="B921B045-1DF0-41C3-AF44-4C6F280D3FAE"
gdisk ${BOOT} <<EOF
x
c
3
${ROOT_UUID}
w
y
EOF
dd if=./idbloader.img of=${BOOT} seek=64 conv=notrunc,fsync
dd if=./test.img of=${BOOT} seek=16384 conv=notrunc,fsync
dd if=./trust.img of=${BOOT} seek=24576 conv=notrunc,fsync
