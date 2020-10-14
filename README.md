# Just practicing the building of a portable light-weight OS 

## Install the toolchain:

- For `ARM`-based platforms (Test on gcc version: 9.2.1):

```bash
sudo apt install gcc-arm-none-eabi
```

## To build:
Choose the desired configuration (target platform and etc.) in the `default-config.mk` and then run:

```bash
$ make
```
The resulting images are placed in the `build` directory.

- `kernel.elf`: The ELF image of the kernel that can be used with QEMU, GRUB or other loaders that support ELF.
- `kernel.img`: The binary image of the kernel.

<hr/>
Author: Sasan Golchin (golchin -AT- bu.edu)
