#!/bin/sh
BOOT=/dev/sda
parted -s ${BOOT} mklabel gpt
parted -s ${BOOT} unit s mkpart loader1 ext4 64 8063
parted -s ${BOOT} unit s mkpart loader2 ext4 16384 24575
parted -s ${BOOT} unit s mkpart trust ext4 24576 32767
parted -s ${BOOT} unit s mkpart boot fat32 32768 1261567
parted -s ${BOOT} set 4 boot on
parted -s ${BOOT} unit s mkpart boot2 ext4 1261568 3358719
parted -s ${BOOT} -- unit s mkpart rootfs ext4 3358720 -34s
ROOT_UUID="B921B045-1DF0-41C3-AF44-4C6F280D3FAE"
gdisk ${BOOT} <<EOF
x
c
6
${ROOT_UUID}
w
y
EOF