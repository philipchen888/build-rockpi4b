# Load GRUB EFI binary from the USB SATA SSD (/dev/sda1)
fatload usb 0:1 ${kernel_addr_r} efi/boot/bootaa64.efi

# Boot the GRUB EFI binary using U-Boot's EFI loader
bootefi ${kernel_addr_r}
