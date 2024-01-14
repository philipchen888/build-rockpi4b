SUMMARY = "LXDE session manager"
HOMEPAGE = "http://lxde.sf.net/"
SECTION = "x11"

LICENSE = "GPLv2"
LIC_FILES_CHKSUM = "file://COPYING;md5=a76102f7f48780284bee49e6edaeb5a9"

DEPENDS = "dbus dbus-glib glib-2.0 intltool-native libgee virtual/libx11 lxde-common polkit vala-native xorgproto"
RDEPENDS_${PN} = "upower dbus-x11"

SRC_URI  = " \
    ${SOURCEFORGE_MIRROR}/lxde/lxsession-${PV}.tar.xz \
    file://0001-replace-use-of-UniqueApp-with-GtkApplication.patch \
    file://0002-GTK-3-GtkObject-no-longer-exists.patch \
"
SRC_URI[md5sum] = "e255aaf3c2a8e49812e3477fb3f099b0"
SRC_URI[sha256sum] = "252f84c80fd3713a506cc8d8ac9773175712d47b031cacc08c8027622ce7ec5d"

PACKAGECONFIG ?= "gtk3"
PACKAGECONFIG[gtk3] = "--enable-gtk3,,gtk+3"
python __anonymous () {
    depends = d.getVar("DEPENDS", d, 1)
    if 'gtk3' not in d.getVar('PACKAGECONFIG', True):
        d.setVar("DEPENDS", "%s gtk+" % depends)
}

inherit autotools pkgconfig gettext

FILES_${PN} += "${datadir}/lxsession"
