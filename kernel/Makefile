export OUT=$(PWD)/out
export BOOT=$(OUT)/boot.img
export ARCH=arm64
export CROSS_COMPILE=/usr/bin/aarch64-linux-gnu-
export BOOT_UUID=95E46EA5

kernel: buildkernel postbuildkernel

clean:
	cd kernel; \
	sudo make ARCH=arm64 clean

buildkernel:
	cd kernel; \
	sudo \rm -rf tmp; mkdir tmp; mkdir -p tmp/lib; \
	make ARCH=arm64 clean; \
	make ARCH=arm64 rockchip_linux_defconfig; \
	make ARCH=arm64 -j8; \
	sudo make ARCH=arm64 INSTALL_MOD_PATH=./tmp modules_install; \
	sudo make ARCH=arm64 INSTALL_DTBS_PATH=./tmp/boot/dtb dtbs_install; \
	sudo make ARCH=arm64 INSTALL_PATH=./tmp/boot install

postbuildkernel:
	cd kernel; \
	\rm -f $(BOOT); \
	mkfs.vfat -n "boot" -i ${BOOT_UUID} -S 512 -C ${BOOT} 512000; \
	mmd -i ${BOOT} ::/extlinux; \
	mcopy -i ${BOOT} -s ../rk3399.conf ::/extlinux/extlinux.conf; \
	mcopy -i ${BOOT} -s ./tmp/boot/config-5.10.131+ ::; \
	mcopy -i ${BOOT} -s ./tmp/boot/System.map-5.10.131+ ::; \
	mcopy -i ${BOOT} -s ./tmp/boot/vmlinuz-5.10.131+ ::; \
	mcopy -i ${BOOT} -s ./tmp/boot/dtb ::; 
