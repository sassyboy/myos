# Select the target platform by assigning TARGET_PLAT one of the following
# values:
# X86_GENERIC, X86_64_GENERIC
# RASPBERRYPI_ZERO, RASPBERRYPI_A, RASPBERRYPI_B, RASPBERRYPI_TWO
TARGET_PLAT=RASPBERRYPI_ZERO

OPT=-O2
# Compiler flags for .c files:
CFLAGS="$OPT -fpic -ffreestanding -nostdlib -Wall -Wextra -c"

# Linker flags
LFLAGS="$OPT -ffreestanding -nostartfiles -nostdlib"
