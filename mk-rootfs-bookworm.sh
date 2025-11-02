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
	sudo umount -lf $TARGET_ROOTFS_DIR/proc || true
	sudo umount -lf $TARGET_ROOTFS_DIR/sys || true
	sudo umount -lf $TARGET_ROOTFS_DIR/dev/pts || true
	sudo umount -lf $TARGET_ROOTFS_DIR/dev || true
	exit -1
}
trap finish ERR

echo -e "\033[36m Extract image \033[0m"
sudo tar -xpf live-image-arm64.tar.tar.gz

sudo cp -rf ../linux/linux/tmp/lib/modules $TARGET_ROOTFS_DIR/lib

# packages folder
sudo mkdir -p $TARGET_ROOTFS_DIR/packages
sudo cp -rf ../packages/$ARCH/* $TARGET_ROOTFS_DIR/packages
sudo cp -rf ../linux/linux/tmp/boot/* $TARGET_ROOTFS_DIR/boot
export KERNEL_VERSION=$(ls $TARGET_ROOTFS_DIR/boot/vmlinuz-* 2>/dev/null | sed 's|.*/vmlinuz-||' | sort -V | tail -n 1)
echo $KERNEL_VERSION
sudo sed -e "s/6.14.0-rc3/$KERNEL_VERSION/g" < ../linux/patches/40_custom_uuid | sudo tee $TARGET_ROOTFS_DIR/boot/40_custom_uuid > /dev/null
cat $TARGET_ROOTFS_DIR/boot/40_custom_uuid

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
\rm -rf /etc/initramfs/post-update.d/z50-raspi-firmware
apt-get upgrade -y
apt-get install -y build-essential git wget firmware-linux grub-efi-arm64 e2fsprogs zstd initramfs-tools gdm3

mkdir -p /boot/efi
grub-install --target=arm64-efi --efi-directory=/boot/efi --bootloader-id=GRUB
update-grub

cp /boot/40_custom_uuid /etc/grub.d/
chmod +x /etc/grub.d/40_custom_uuid
rm -rf /boot/40_custom_uuid

# Migrate extlinux.conf to GRUB
rm -rf /boot/extlinux
cat << GRUB_EOF > /etc/default/grub
GRUB_DEFAULT="Boot from UUID"
GRUB_TIMEOUT=5
GRUB_CMDLINE_LINUX_DEFAULT="quiet"
GRUB_CMDLINE_LINUX=""
GRUB_EOF

cat << FSTAB_EOF > /etc/fstab
UUID=B921B045-1DF0-41C3-AF44-4C6F280D3FAE /  ext4    errors=remount-ro   0   1
UUID=95E4-6EA5  /boot/efi  vfat    umask=0077      0       1
FSTAB_EOF

update-grub

chmod o+x /usr/lib/dbus-1.0/dbus-daemon-launch-helper
chmod +x /etc/rc.local

cp /packages/rkwifibt/brcmfmac43456-sdio.bin /lib/firmware/brcm/brcmfmac43456-sdio.radxa,rockpi4b.bin
cp /packages/rkwifibt/brcmfmac43456-sdio.radxa,rockpi4b.txt /lib/firmware/brcm/
cp /packages/rkwifibt/BCM4345C5* /lib/firmware/brcm/
apt-get install -f -y

systemctl enable rc-local
systemctl enable resize-helper
chsh -s /bin/bash linaro
update-initramfs -c -k $KERNEL_VERSION

#---------------Clean--------------
rm -rf /var/lib/apt/lists/*
sync
EOF

sudo umount -lf $TARGET_ROOTFS_DIR/proc || true
sudo umount -lf $TARGET_ROOTFS_DIR/sys || true
sudo umount -lf $TARGET_ROOTFS_DIR/dev/pts || true
sudo umount -lf $TARGET_ROOTFS_DIR/dev || true
sync
