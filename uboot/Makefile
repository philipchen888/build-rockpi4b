export OUT=$(PWD)/out
export ARCH=arm64
export CROSS_COMPILE=/usr/local/gcc-linaro-7.5.0-2019.12-x86_64_aarch64-linux-gnu/bin/aarch64-linux-gnu-
export BL31=$(PWD)/rkbin/bin/rk33/rk3399_bl31_v1.26.elf

u-boot: cleanuboot builduboot postbuilduboot

clean:
	cd u-boot; \
	make clean

cleanuboot:
	\rm -f $(OUT)/idbloader.img
	\rm -f $(OUT)/rk3399_loader_v1.20.119.bin
	\rm -f $(OUT)/trust.img
	\rm -f $(OUT)/uboot.img

builduboot:
	cd u-boot; \
	make clean; \
	make ARCH=arm64 rock-pi-4-rk3399_defconfig; \
	make all

postbuilduboot:
	cd u-boot; \
	../rkbin/tools/loaderimage --pack --uboot ./u-boot-dtb.bin uboot.img 0x200000 --size 1024 1; \
	./tools/mkimage -n rk3399 -T rksd -d ../rkbin/bin/rk33/rk3399_ddr_800MHz_v1.20.bin idbloader.img; \
	cat ../rkbin/bin/rk33/rk3399_miniloader_v1.19.bin >> idbloader.img; \
	cp idbloader.img ${OUT}/; \
	cp ../rkbin/bin/rk33/rk3399_loader_v1.20.119.bin ${OUT}/; \
	../rkbin/tools/trust_merger --size 1024 1 ../trust.ini; \
	cp uboot.img ${OUT}/; \
	cp trust.img ${OUT}/

