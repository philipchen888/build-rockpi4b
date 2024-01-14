#!/bin/sh

BOARD_DIR="$(dirname $0)"

install -m 0644 -D $BOARD_DIR/../../../../../linux/linux/arch/arm64/boot/Image $BINARIES_DIR/Image
install -m 0644 -D $BOARD_DIR/../../../../../linux/linux/arch/arm64/boot/dts/rockchip/rk3399-rock-pi-4b.dtb $BINARIES_DIR/rk3399-rock-pi-4b.dtb
install -m 0644 -D $BOARD_DIR/../../../../../linux/rk3399.conf $BINARIES_DIR/extlinux/extlinux.conf

install -m 0644 -D $BOARD_DIR/../../../../../uboot/out/trust.img $BINARIES_DIR/trust.img
install -m 0644 -D $BOARD_DIR/../../../../../uboot/out/uboot.img $BINARIES_DIR/uboot.img
install -m 0644 -D $BOARD_DIR/../../../../../uboot/out/idbloader.img $BINARIES_DIR/idbloader.img
