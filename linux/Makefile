export OUT=$(PWD)/out
export BOOT=$(OUT)/boot.img
export ARCH=arm64
export CROSS_COMPILE=/usr/local/arm-gnu-toolchain-13.3.rel1-x86_64-aarch64-none-linux-gnu/bin/aarch64-none-linux-gnu-
export BOOT_UUID=95E46EA5

clean:
	cd linux; \
	sudo make mrproper

kernel: buildkernel postbuildkernel

buildkernel:
	cp ./patches/defconfig ./linux/arch/arm64/configs
	cd linux; \
	sudo \rm -rf tmp; mkdir tmp; mkdir -p tmp/lib; \
	sudo make mrproper; \
	make defconfig; \
	make all -j8; \
	sudo make ARCH=arm64 INSTALL_MOD_PATH=./tmp modules_install; \
	sudo make ARCH=arm64 INSTALL_DTBS_PATH=./tmp/boot/dtb dtbs_install; \
	sudo make ARCH=arm64 INSTALL_PATH=./tmp/boot install

postbuildkernel:
	mkimage -A arm64 -T script -C none -n 'Boot Script' -d ./patches/ubuntu-jammy/boot.cmd ./patches/ubuntu-jammy/boot.scr
	./mkextlinux.sh
	cd linux; \
	\rm -f $(BOOT); \
	mkfs.vfat -n "boot" -i ${BOOT_UUID} -S 512 -C ${BOOT} 512000; \
	mmd -i ${BOOT} ::/extlinux; \
	mmd -i ${BOOT} ::/dtb; \
	mcopy -i ${BOOT} -s $(OUT)/extlinux.conf ::/extlinux; \
	mcopy -i ${BOOT} -s ./tmp/boot/* ::; \
