all: kernel build/ramdisk.img

kernel: force config.mk
	mkdir -p build
	make -C kernel
	
build/ramdisk.img: force config.mk
	mkdir -p build
	make -C programs

config.mk: default-config.mk
	./configure

emulate-pi2: build/kernel.elf
	qemu-system-arm -m 256 -M raspi2 -serial stdio -kernel $<

emulate-pi3: build/kernel8.img
	qemu-system-aarch64 -M raspi3 -serial stdio -kernel $<

clean:
	make -C kernel clean
	make -C programs clean

force:
