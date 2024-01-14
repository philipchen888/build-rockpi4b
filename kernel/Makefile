export OUT=$(PWD)/out
export BOOT=$(OUT)/boot.img
export ARCH=arm64
export CROSS_COMPILE=/usr/local/gcc-linaro-7.5.0-2019.12-x86_64_aarch64-linux-gnu/bin/aarch64-linux-gnu-

kernel: buildkernel postbuildkernel

clean:
	cd kernel; \
	sudo make ARCH=arm64 clean

buildkernel:
	cd kernel; \
	cp ../patches/rockpi-4b-linux.dts arch/arm64/boot/dts/rockchip; \
	make ARCH=arm64 clean; \
	make ARCH=arm64 rockchip_linux_defconfig; \
	make ARCH=arm64 -j4; \
	sudo make ARCH=arm64 INSTALL_MOD_PATH=./tmp modules_install

postbuildkernel:
	cd kernel; \
	\rm -f $(BOOT); \
	mkfs.vfat -n "boot" -S 512 -C ${BOOT} 512000; \
	mmd -i ${BOOT} ::/extlinux; \
	mcopy -i ${BOOT} -s ../rk3399.conf ::/extlinux/extlinux.conf; \
	mcopy -i ${BOOT} -s ./arch/arm64/boot/Image ::; \
	mcopy -i ${BOOT} -s ./arch/arm64/boot/dts/rockchip/rockpi-4b-linux.dtb ::
