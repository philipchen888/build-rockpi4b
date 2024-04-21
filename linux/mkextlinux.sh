#!/bin/sh
cat >./out/extlinux.conf <<EOF
label kernel-mainline
    kernel /`cd ./linux/tmp/boot; ls vmlinuz*`
    fdt /dtbs/rockchip/rk3399-rock-pi-4b.dtb
    append earlyprintk console=ttyFIQ0,1500000n8 console=ttyS2,1500000n8 rw root=PARTUUID=b921b045-1d rootfstype=ext4 init=/sbin/init rootwait
EOF
