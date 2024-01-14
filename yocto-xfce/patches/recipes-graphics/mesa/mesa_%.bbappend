# PROVIDES:remove = "virtual/libgles1 virtual/libgles2 virtual/egl virtual/libwayland-egl"

# do_install:append () {
#     rm -f ${D}/${libdir}/libEGL*
#     rm -f ${D}/${libdir}/libGLESv1_CM.*
#     rm -f ${D}/${libdir}/libGLESv2.*
#     rm -f ${D}/${libdir}/libgbm*
#     rm -f ${D}/${libdir}/libwayland-egl*
# }

PACKAGECONFIG:append:rk3399 = " kmsro panfrost"
