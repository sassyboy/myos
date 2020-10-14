kernel: force config.mk
	mkdir -p build
	make -C kernel
	
config.mk: default-config.mk
	./configure

clean:
	make -C kernel clean

force:
