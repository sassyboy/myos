include ../config.mk

CC=$(TOOLCHAIN_PREFIX)gcc
OBJCPY=$(TOOLCHAIN_PREFIX)objcopy

all: ../build/ramdisk.img

../build/ramdisk.img: testsuite.elf
	$(OBJCPY) $< -O binary $@

testsuite.elf: testsuite/main.c
	$(CC) -I../kernel -I../kernel/$(PLATDIR) -O2 -fpic -ffreestanding -nostdlib -Wall -Wextra -c $^ -o testsuite/main.o
	$(CC) $(LFLAGS) -T linker.ld -o $@ testsuite/main.o -Wl,--just-symbols=../build/kernel.elf

clean:
	rm -rf ../build/ramdisk.img testsuite.elf testsuite/main.o

force:
