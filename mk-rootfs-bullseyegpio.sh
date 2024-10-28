#!/bin/bash -e

# Directory contains the target rootfs
TARGET_ROOTFS_DIR="binary"

if [ -e $TARGET_ROOTFS_DIR ]; then
	sudo rm -rf $TARGET_ROOTFS_DIR
fi

if [ "$ARCH" == "armhf" ]; then
	ARCH='armhf'
elif [ "$ARCH" == "arm64" ]; then
	ARCH='arm64'
else
    echo -e "\033[36m please input is: armhf or arm64...... \033[0m"
fi

if [ ! $VERSION ]; then
	VERSION="release"
fi

if [ ! -e live-image-arm64.tar.tar.gz ]; then
	echo "\033[36m Run sudo lb build first \033[0m"
fi

finish() {
	sudo umount $TARGET_ROOTFS_DIR/dev
	exit -1
}
trap finish ERR

echo -e "\033[36m Extract image \033[0m"
sudo tar -xpf live-image-arm64.tar.tar.gz

sudo cp -rf ../kernel/kernel/tmp/lib/modules $TARGET_ROOTFS_DIR/lib
sudo cp -rf ../kernel/kernel/tmp/lib/firmware $TARGET_ROOTFS_DIR/lib

# packages folder
sudo mkdir -p $TARGET_ROOTFS_DIR/packages
sudo cp -rf ../packages/$ARCH/* $TARGET_ROOTFS_DIR/packages

# overlay folder
sudo cp -rf ../overlay/* $TARGET_ROOTFS_DIR/

echo -e "\033[36m Change root.....................\033[0m"
if [ "$ARCH" == "armhf" ]; then
	sudo cp /usr/bin/qemu-arm-static $TARGET_ROOTFS_DIR/usr/bin/
elif [ "$ARCH" == "arm64"  ]; then
	sudo cp /usr/bin/qemu-aarch64-static $TARGET_ROOTFS_DIR/usr/bin/
fi
sudo mount -o bind /proc $TARGET_ROOTFS_DIR/proc
sudo mount -o bind /sys $TARGET_ROOTFS_DIR/sys
sudo mount -o bind /dev $TARGET_ROOTFS_DIR/dev
sudo mount -o bind /dev/pts $TARGET_ROOTFS_DIR/dev/pts

cat << EOF | sudo chroot $TARGET_ROOTFS_DIR

rm -rf /etc/resolv.conf
echo -e "nameserver 8.8.8.8\nnameserver 8.8.4.4" > /etc/resolv.conf
resolvconf -u
apt-get update
apt-get upgrade -y
apt-get install -y build-essential git wget dkms

chmod o+x /usr/lib/dbus-1.0/dbus-daemon-launch-helper
chmod +x /etc/rc.local

dpkg -i /packages/xserver/*.deb
apt-get install -f -y

echo -e "\033[36m Install rkwifibt.................... \033[0m"
dpkg -i  /packages/rkwifibt/*.deb
apt-get install -f -y
mkdir /vendor
mkdir /vendor/etc
ln -sf /system/etc/firmware /vendor/etc/
cp /packages/rkwifibt/BCM4345C5.hcd /lib/firmware/brcm/
sed 's/^exit 0/rfkill unblock bluetooth\nbrcm_patchram_plus1 -d --enable_hci --no2bytes --use_baudrate_for_downloade --tosleep  200000 --baudrate 1500000 --patchram \/lib\/firmware\/brcm\/BCM4345C5.hcd \/dev\/ttyS0 > \/dev\/null 2>\&1  \&\nexit 0/' /etc/rc.local > rclocal
mv rclocal /etc/rc.local
chmod +x /etc/rc.local
echo 'source /etc/rc.local' >> /etc/init.d/rcS

systemctl enable rockchip.service
systemctl enable resize-helper

#---------------Clean--------------
rm -rf /var/lib/apt/lists/*
sync
EOF

sudo umount $TARGET_ROOTFS_DIR/proc
sudo umount $TARGET_ROOTFS_DIR/sys
sudo umount $TARGET_ROOTFS_DIR/dev/pts
sudo umount $TARGET_ROOTFS_DIR/dev
