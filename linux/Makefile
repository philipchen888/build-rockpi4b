export OUT=$(PWD)/out
export BOOT=$(OUT)/boot.img
export ARCH=arm64
export CROSS_COMPILE=/usr/local/gcc-linaro-7.5.0-2019.12-x86_64_aarch64-linux-gnu/bin/aarch64-linux-gnu-

clean:
	cd linux; \
	sudo make mrproper

kernel: buildkernel postbuildkernel

buildkernel:
	cd linux; \
	sudo make mrproper; \
	make defconfig; \
	make all -j4; \
	sudo make ARCH=arm64 INSTALL_MOD_PATH=./tmp modules_install

postbuildkernel:
	cd linux; \
	\rm -f $(BOOT); \
	mkfs.vfat -n "boot" -S 512 -C ${BOOT} 512000; \
	mmd -i ${BOOT} ::/extlinux; \
	mcopy -i ${BOOT} -s ../rk3399.conf ::/extlinux/extlinux.conf; \
	mcopy -i ${BOOT} -s ./arch/arm64/boot/Image ::; \
	mcopy -i ${BOOT} -s ./arch/arm64/boot/dts/rockchip/rk3399-rock-pi-4b.dtb ::
