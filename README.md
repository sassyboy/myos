# Just practicing the building of a portable light-weight OS 

## Install the toolchain:

- For 32-bit `ARM`-based platforms (Test on gcc version: 9.2.1):

```bash
sudo apt install gcc-arm-none-eabi
```
- For 64-bit `ARM`-based platforms (Tested on gcc version: 9.3.0):

```bash
sudo apt install gcc-aarch64-linux-gnu
```
- TODO: Add x86 and x86_64

- TODO: Freeze the toolchains into one single docker image

## Build the kernel and user programs:
Choose the desired configuration (target platform and etc.) in the `default-config.mk` and then run:

```bash
$ make
```
The resulting images are placed in the `build` directory.

- `kernel.elf`: The ELF image of the kernel that can be used with QEMU, GRUB or other loaders that support ELF.
- `kernel.img`: The binary image of the kernel.

## Make a bootable drive:
See example boot-drive/boot-partition content in the `disk-templates` directory.

## Emulate a device
The following devices has been tested through emulation. You can reproduce the emulation environment using the specified commands in each case. You may need to install different flavors of `QEMU` for each architecture.

- **Raspberry Pi 2:** `make emulate-pi2`
- **Raspberry Pi 3B+:** `make emulate-pi2`

<hr/>
Author: Sasan Golchin (golchin -AT- bu.edu)
