DESCRIPTION = "Linux kernel for RockPi-4"

inherit kernel
inherit python3native
require recipes-kernel/linux/linux-yocto.inc

LIC_FILES_CHKSUM = "file://${COMMON_LICENSE_DIR}/GPL-2.0-only;md5=801f80980d171dd6425610833a22dbe6"

DEPENDS += "openssl-native"

SRC_URI = " \
	git://git.kernel.org/pub/scm/linux/kernel/git/torvalds/linux.git;protocol=https;branch=master; \
"

SRCREV = "${AUTOREV}"
LINUX_VERSION = "6.9-rc5"
KCONFIG_MODE = "alldefconfig"

# Override local version in order to use the one generated by linux build system
# And not "yocto-standard"
LINUX_VERSION_EXTENSION = ""
PR = "r1"
PV = "${LINUX_VERSION}"

# Include only supported boards for now
COMPATIBLE_MACHINE = "(rk3036|rk3066|rk3288|rk3328|rk3399|rk3308)"

do_compile:append() {
	oe_runmake dtbs
}

# Make sure we use /usr/bin/env ${PYTHON_PN} for scripts
do_patch:append() {
	for s in `grep -rIl python ${S}/scripts`; do
		sed -i -e '1s|^#!.*python[23]*|#!/usr/bin/env ${PYTHON_PN}|' $s
	done
}

# do_deploy:append() {
#	install -d ${DEPLOYDIR}/overlays
#	install -m 644 ${WORKDIR}/linux-rockpi_4*/arch/arm64/boot/dts/rockchip/overlays-rockpi4/* ${DEPLOYDIR}/overlays
#	install -m 644 ${S}/arch/arm64/boot/dts/rockchip/overlays-rockpi4/hw_intfc.conf ${DEPLOYDIR}/
# }
