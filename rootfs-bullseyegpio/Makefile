export BOARD=radxa
export LB_BOOTSTRAP_INCLUDE="apt-transport-https gnupg"
export ARCH=arm64
export CROSS_COMPILE=/usr/local/gcc-linaro-7.5.0-2019.12-x86_64_aarch64-linux-gnu/bin/aarch64-linux-gnu-

rootfs: clean prebuild buildrootfs postbuild mkimage

clean:
	sudo lb clean --purge
	\rm -rf config .build rootfs

cleanall:
	sudo lb clean --purge
	\rm -rf config .build rootfs
	\rm -rf linaro-rootfs.img

prebuild:
	lb config \
	--apt-indices false \
	--apt-recommends false \
	--apt-secure false \
	--architectures arm64 \
	--archive-areas 'main contrib non-free' \
	--backports false \
	--binary-filesystem ext4 \
	--binary-images tar \
	--bootappend-live "hostname=linaro-alip username=linaro" \
	--bootstrap-qemu-arch arm64 \
	--bootstrap-qemu-static /usr/bin/qemu-aarch64-static \
	--cache false \
	--chroot-filesystem none \
	--compression gzip \
	--debootstrap-options "--variant=minbase --include=apt-transport-https,gnupg" \
	--distribution bullseye \
	--gzip-options '-9 --rsyncable' \
	--iso-publisher 'Linaro; http://www.linaro.org/; linaro-dev@lists.linaro.org' \
	--iso-volume 'Linaro Bullseye $(date +%Y%m%d-%H:%M)' \
	--linux-flavours none \
	--linux-packages none \
	--mode debian \
	--security true \
	--system normal \
	--updates true

buildrootfs:
	\cp -rf $(PWD)/../customization/* config/
	cp linaro.list.chroot config/package-lists/linaro.list.chroot
	sudo lb build

postbuild:
	ARCH=arm64 $(PWD)/../mk-rootfs-bullseyegpio.sh

mkimage:
	$(PWD)/../mk-image.sh
