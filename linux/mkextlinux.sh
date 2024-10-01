#!/bin/sh
cat >./out/extlinux.conf <<EOF
label kernel-mainline
    kernel /`cd ./linux/tmp/boot; ls vmlinuz*`
    fdt /dtb/rockchip/rk3399-rock-pi-4b.dtb
    append root=PARTUUID=b921b045-1d rw rootfstype=ext4 init=/sbin/init rootwait
EOF
