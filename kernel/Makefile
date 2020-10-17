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
modules/intctrl/bcm_intctrl.o \
modules/timers/systimer_bcm.o \
modules/serial/bcm_miniuart.o modules/serial/bcm_pl011uart.o \
modules/gpio/bcm.o \
util/stdio.o

BUILDDIR=../build/
KERN_ELF=$(BUILDDIR)kernel.elf
KERN_IMG=$(BUILDDIR)$(KERN_IMG_FNAME)

all: $(KERN_IMG)
	
$(KERN_IMG): $(KERN_ELF)
	$(OBJCPY) $< -O binary $@

$(KERN_ELF): $(OBJFILES)
	$(CC) $(LFLAGS) -T linker.ld -L $(PLATDIR) -o $@ $^

%.o: %.c
	$(CC) $(CFLAGS) $< -o $@

%.o: %.S
	$(CC) $(CFLAGS) $< -o $@

clean:
	rm -rf $(OBJFILES) $(KERN_ELF) $(KERN_IMG)
