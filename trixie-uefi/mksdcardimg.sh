#!/bin/sh
BOOT=/dev/sda
parted -s ${BOOT} mklabel gpt
parted -s ${BOOT} unit s mkpart boot fat32 32768 1261567
parted -s ${BOOT} set 1 boot on
parted -s ${BOOT} unit s mkpart boot2 ext4 1261568 16793599
