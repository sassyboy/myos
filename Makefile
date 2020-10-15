all: kernel build/ramdisk.img

kernel: force config.mk
	mkdir -p build
	make -C kernel
	
build/ramdisk.img: force config.mk
	mkdir -p build
	make -C programs

config.mk: default-config.mk
	./configure

clean:
	make -C kernel clean
	make -C programs clean

force:
