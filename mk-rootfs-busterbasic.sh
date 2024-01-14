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

if [ ! -e live-image-$ARCH.tar.tar.gz ]; then
	echo "\033[36m Run sudo lb build first \033[0m"
fi

finish() {
	sudo umount $TARGET_ROOTFS_DIR/dev
	exit -1
}
trap finish ERR

echo -e "\033[36m Extract image \033[0m"
sudo tar -xpf live-image-$ARCH.tar.tar.gz

sudo cp -rf ../linux/linux/tmp/lib/modules $TARGET_ROOTFS_DIR/lib

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
sudo mount -o bind /dev $TARGET_ROOTFS_DIR/dev

cat << EOF | sudo chroot $TARGET_ROOTFS_DIR

ln -sf /etc/resolvconf/run/resolv.conf /etc/resolv.conf
resolvconf -u
apt-get update
apt-get upgrade -y
apt-get install -y build-essential git wget

chmod o+x /usr/lib/dbus-1.0/dbus-daemon-launch-helper
chmod +x /etc/rc.local

cp /packages/rkwifibt/brcmfmac43456-sdio.bin /lib/firmware/brcm/brcmfmac43456-sdio.radxa,rockpi4b.bin
cp /packages/rkwifibt/brcmfmac43456-sdio.radxa,rockpi4b.txt /lib/firmware/brcm/
cp /packages/rkwifibt/BCM4345C5.hcd /lib/firmware/brcm/
apt-get install -f -y

systemctl enable resize-helper

#---------------Clean--------------
rm -rf /var/lib/apt/lists/*

EOF

sudo umount $TARGET_ROOTFS_DIR/dev
