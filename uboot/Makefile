export OUT=$(PWD)/out
export ARCH=arm64
export CROSS_COMPILE=/usr/local/arm-gnu-toolchain-13.3.rel1-x86_64-aarch64-none-linux-gnu/bin/aarch64-none-linux-gnu-
export BL31=$(PWD)/rkbin/bin/rk33/rk3399_bl31_v1.35.elf

u-boot: cleanuboot builduboot postbuilduboot

clean:
	cd u-boot; \
	make clean

cleanuboot:
	\rm -f $(OUT)/idbloader.img
	\rm -f $(OUT)/trust.img
	\rm -f $(OUT)/uboot.img

builduboot:
	rm -rf out; mkdir out;
	cp ./patches/rock-pi-4-rk3399_defconfig ./u-boot/configs
	cd u-boot; \
	make clean; \
	make ARCH=arm64 rock-pi-4-rk3399_defconfig; \
	make all

postbuilduboot:
	cd u-boot; \
	../rkbin/tools/loaderimage --pack --uboot ./u-boot-dtb.bin uboot.img 0x200000 --size 1024 1; \
	./tools/mkimage -n rk3399 -T rksd -d ../rkbin/bin/rk33/rk3399_ddr_800MHz_v1.25.bin idbloader.img; \
	cat ../rkbin/bin/rk33/rk3399_miniloader_v1.19.bin >> idbloader.img; \
	cp idbloader.img ${OUT}/; \
	../rkbin/tools/trust_merger --size 1024 1 ../trust.ini; \
	cp uboot.img ${OUT}/; \
	cp trust.img ${OUT}/; \
	../rkbin/tools/firmwareMerger -P ../spi.ini ${OUT}; \
	mv ${OUT}/Firmware.img ${OUT}/uboot-trust-spi.img

