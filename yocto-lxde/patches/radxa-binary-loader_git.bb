DESCRIPTION = "Rockchip-Radxa binary loader"

LICENSE = "BINARY"
LIC_FILES_CHKSUM = "file://LICENSE.TXT;md5=564e729dd65db6f65f911ce0cd340cf9"
NO_GENERIC_LICENSE[BINARY] = "LICENSE.TXT"

DEPENDS = "radxa-binary-native"

SRC_URI = "git://github.com/radxa/rkbin.git;protocol=https;branch=master;"
SRCREV = "530b61d0e59289d7a24c404faa7617fc87e14495"

S = "${WORKDIR}/git"

LOADER_px30 ?= "bin/rk33/px30_loader_v1.14.120.bin"
LOADER_rk3308 ?= "bin/rk33/rk3308_loader_uart0_m0_emmc_port_support_sd_20190717.bin"
LOADER_rk3328 ?= "bin/rk33/rk3328_loader_ddr333_v1.16.250.bin"
LOADER_rk3399 ?= "bin/rk33/rk3399_loader_v1.12.112.bin"
LOADER_rk3399pro ?= "bin/rk33/rk3399pro_loader_v1.20.115.bin"
LOADER_rk3566 ?= "bin/rk35/rk356x_spl_loader_ddr1056_v1.10.111.bin"
LOADER_rk3568 ?= "bin/rk35/rk356x_spl_loader_ddr1056_v1.10.111.bin"

MINILOADER_px30 ?= "bin/rk33/px30_miniloader_v1.20.bin"
MINILOADER_rk3308 ?= "bin/rk33/rk3308_miniloader_emmc_port_support_sd_20190717.bin"
MINILOADER_rk3328 ?= "bin/rk33/rk322xh_miniloader_v2.50.bin"
MINILOADER_rk3399 ?= "bin/rk33/rk3399_miniloader_v1.19.bin"
MINILOADER_rk3399pro ?= "bin/rk33/rk3399pro_miniloader_v1.15.bin"

DDR_px30 ?= "bin/rk33/px30_ddr_333MHz_v1.14.bin"
DDR_rk3308 ?= "bin/rk33/rk3308_ddr_589MHz_uart0_m0_v1.26.bin"
DDR_rk3328 ?= "bin/rk33/rk3328_ddr_333MHz_v1.16.bin"
DDR_rk3399 ?= "bin/rk33/rk3399_ddr_800MHz_v1.20.bin"
DDR_rk3399pro ?= "bin/rk33/rk3399_ddr_800MHz_v1.20.bin"
DDR_rk3566 ?= "bin/rk35/rk3568_ddr_1056MHz_v1.10.bin"
DDR_rk3568 ?= "bin/rk35/rk3568_ddr_1056MHz_v1.10.bin"

BL31_px30 ?= "bin/rk33/px30_bl31_v1.18.elf"
BL31_rk3308 ?= "bin/rk33/rk3308_bl31_v2.10.elf"
BL31_rk3328 ?= "bin/rk33/rk322xh_bl31_v1.42.elf"
BL31_rk3399 ?= "bin/rk33/rk3399_bl31_v1.26.elf"
BL31_rk3399pro ?= "bin/rk33/rk3399pro_bl31_v1.22.elf"
BL31_rk3566 ?= "bin/rk35/rk3568_bl31_v1.32.elf"
BL31_rk3568 ?= "bin/rk35/rk3568_bl31_v1.32.elf"

inherit deploy

DDR_BIN = "ddr.bin"
LOADER_BIN = "loader.bin"
MINILOADER_BIN = "miniloader.bin"
ATF_BIN = "atf.bin"
UBOOT_IMG = "uboot.img"
UBOOT_ITB = "u-boot.itb"
BL31_ELF = "bl31.elf"

RKBINARY_DEPLOY_DIR = "${DEPLOYDIR}/radxa-binary"

do_deploy () {
	install -d ${RKBINARY_DEPLOY_DIR}
	[ ${DDR} ] && cp ${S}/${DDR} ${RKBINARY_DEPLOY_DIR}/${DDR_BIN}
	[ ${MINILOADER} ] && cp ${S}/${MINILOADER} ${RKBINARY_DEPLOY_DIR}/${MINILOADER_BIN}	
	[ ${LOADER} ] && cp ${S}/${LOADER} ${RKBINARY_DEPLOY_DIR}/${LOADER_BIN}
	[ ${ATF} ] && cp ${S}/${ATF} ${RKBINARY_DEPLOY_DIR}/${ATF_BIN}
	[ ${BL31} ] && cp ${S}/${BL31} ${RKBINARY_DEPLOY_DIR}/${BL31_ELF}
}

addtask deploy before do_build after do_compile

do_package[noexec] = "1"
do_packagedata[noexec] = "1"
do_package_write[noexec] = "1"
do_package_write_ipk[noexec] = "1"
do_package_write_rpm[noexec] = "1"
do_package_write_deb[noexec] = "1"
do_package_write_tar[noexec] = "1"
