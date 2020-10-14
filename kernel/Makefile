include ../config.mk

CC=$(TOOLCHAIN_PREFIX)gcc
OBJCPY=$(TOOLCHAIN_PREFIX)objcopy

# Prepare the list of the object files to compile into the kernel:
# 1) Add the platform-dependent object files are already added by the
# corresponding include.mk
include $(PLATDIR)include.mk
# 2) Add the platform-agnostic parts of the kernel
OBJFILES+= \
kernel.o \
modules/timers/systimer_bcm.o \
modules/serial/uart_bcm.o \
modules/gpio/bcm.o \
util/stdio.o

BUILDDIR=../build/
KERN_ELF=$(BUILDDIR)kernel.elf
KERN_IMG=$(BUILDDIR)kernel.img

all: $(KERN_IMG)
	
$(KERN_IMG): $(KERN_ELF)
	$(OBJCPY) $< -O binary $@

$(KERN_ELF): $(OBJFILES)
	$(CC) $(LFLAGS) -T $(PLATDIR)linker.ld -o $@ $^

%.o: %.c
	$(CC) $(CFLAGS) $< -o $@

%.o: %.S
	$(CC) $(CFLAGS) $< -o $@

clean:
	rm -rf $(OBJFILES) $(KERN_ELF) $(KERN_IMG)
