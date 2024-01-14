DEFAULT_PREFERENCE = "1"

require recipes-bsp/u-boot/u-boot.inc
require recipes-bsp/u-boot/u-boot-common.inc

DESCRIPTION = "RockPi U-Boot"
LICENSE = "GPL-2.0-or-later"
LIC_FILES_CHKSUM = "file://Licenses/README;md5=2ca5f2c35c8cc335f0a19756634782f1"
COMPATIBLE_MACHINE = "(rk3036|rk3066|rk3188|rk3288|rk3328|rk3399|rk3308)"
SECTION = "bootloaders"
DEPENDS = "dtc-native bc-native swig-native bison-native coreutils-native ${PYTHON_PN}-pyelftools-native ${PYTHON_PN}-native linux-rockpi-4"

SRCREV_FORMAT = "default_rkbin"

PROVIDES = "virtual/bootloader"

PV = "2017.09+git${SRCPV}"
S = "${WORKDIR}/git"
B = "${WORKDIR}/build"

# u-boot will build native python module
inherit python3native python3-dir

do_deploy[depends] += " linux-rockpi-4:do_deploy"

do_configure () {
    if [ -z "${UBOOT_CONFIG}" ]; then
        if [ -n "${UBOOT_MACHINE}" ]; then
            oe_runmake -C ${S} O=${B} ${UBOOT_MACHINE}
        else
            oe_runmake -C ${S} O=${B} oldconfig
        fi

        ${S}/scripts/kconfig/merge_config.sh -m .config ${@" ".join(find_cfgs(d))}
        cml1_do_configure
    fi
}

do_compile:prepend () {
	export STAGING_INCDIR=${STAGING_INCDIR_NATIVE};
	export STAGING_LIBDIR=${STAGING_LIBDIR_NATIVE};
}

do_compile:append () {
	# copy to default search path
	if [ ${SPL_BINARY} ]; then
		cp ${B}/spl/${SPL_BINARY} ${B}/
	fi
}

SRC_URI = " \
	git://github.com/u-boot/u-boot.git;protocol=https;branch=u-boot-2023.07.y; \
	file://modify.patch; \
"
SRCREV = "${AUTOREV}"
