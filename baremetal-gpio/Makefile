CROSS_COMPILE=/usr/local/gcc-linaro-7.5.0-2019.12-x86_64_aarch64-linux-gnu/bin/aarch64-linux-gnu-
STARTUP_DEFS=-D__STARTUP_CLEAR_BSS -D__START=main
INCLUDE = -I.
CFLAGS  = -g -march=armv8-a -O1 -Wl,--build-id=none -nostdlib -fno-builtin $(INCLUDE)
LDSCRIPTS=-L. -T u-boot-spl.lds -lgcc
LFLAGS= $(LDSCRIPTS)

test: test.img

test.img: test.bin
	./rkbin/tools/loaderimage --pack --uboot $< $@ 0x200000 --size 1024 1
	mkimage -n rk3399 -T rksd -d ./rkbin/bin/rk33/rk3399_ddr_800MHz_v1.30.bin idbloader.img
	cat ./rkbin/bin/rk33/rk3399_miniloader_v1.30.bin >> idbloader.img
	./rkbin/tools/trust_merger trust.ini

test.bin: test.elf
	$(CROSS_COMPILE)objcopy -O binary $< $@
	$(CROSS_COMPILE)objdump -S $< > test.list

test.elf: start.S exceptions.S crt0_64.S test.c uart.c printf.c timer.c clock.c gpio.c pwm.c i2c.c spi.c mmc.c
	$(CROSS_COMPILE)gcc $(CFLAGS) $^ $(LFLAGS) -o $@

.PHONY: clean test

clean:
	rm -f *.img test.bin *.elf test.list
