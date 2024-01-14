apply_patches:
	cd poky/build; \
	cp ../../patches/rkwifibt-firmware.bb ../meta-rockchip/recipes-kernel/rkwifibt-firmware/; \
	\rm -rf ../meta-rockchip/recipes-multimedia/rockchip-rkaiq; \
	cp -rf ../../patches/resize-helper ../meta-rockchip/recipes-bsp/; \
	\rm -f ../meta-rockchip/recipes-kernel/linux/*.bb; \
	cp ../../patches/linux-rockpi-4_4.4.bb ../meta-rockchip/recipes-kernel/linux/; \
	cp ../../patches/rockchip-radxa-gpt-img.bbclass ../meta-rockchip/classes/; \
	\rm -rf ../meta-rockchip/recipes-graphics/; \
	cp -r ../../patches/recipes-graphics ../meta-rockchip/; \
	rm -f ../meta-rockchip/recipes-kernel/cypress-backports/brcmfmac.bb; \
	\rm -f ../meta-rockchip/recipes-bsp/rk-binary/*; \
	cp ../../patches/radxa-binary* ../meta-rockchip/recipes-bsp/rk-binary/; \
	\rm -rf ../meta-rockchip/recipes-bsp/u-boot/files/; \
	mkdir ../meta-rockchip/recipes-bsp/u-boot/files/; \
	cp ../../patches/modify.patch ../meta-rockchip/recipes-bsp/u-boot/files/; \
	cp ../../patches/u-boot-rockchip.bb ../meta-rockchip/recipes-bsp/u-boot/; \
	cp ../../patches/rockpi-4b-rk3399.conf ../meta-rockchip/conf/machine/; \
	cp ../../patches/xfce-desktop-image.bb ../meta/recipes-graphics/images/; \
	cp ../../patches/bblayers.conf ./conf/; \
	cp ../../patches/local.conf ./conf/
