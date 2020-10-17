# Select the target platform by assigning TARGET_PLAT one of the following
# values:
# X86_GENERIC, X86_64_GENERIC
# RPI_0W, RPI_2, RPI_3BP_64, RPI_4B_64
TARGET_PLAT=RPI_0W

OPT=-O2
# Compiler flags for .c files:
CFLAGS="$OPT -fpic -ffreestanding -nostdlib -Wall -Wextra -c"

# Linker flags
LFLAGS="$OPT -ffreestanding -nostartfiles -nostdlib"
