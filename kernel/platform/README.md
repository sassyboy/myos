# How to add support for a new platform?

1. Create a directory with the name of the platform/architecture under `kernel/platform/`
1. Implement the following platform-dependent source code and build script files:
    1. **Kernel entry:** Must be defined in a section named `.text.boot`. The entry code must clear the memory region [`_kernel_bss_start` to `_kernel_bss_end`], initialize the platform and call the kernel's platform-agnostic start function (`qKernelStart()`) with the interrupts disabled, once for each CPU. See `arm1176jzf-s/entry.S` as an example.
    1. **Platform-specific C definitions:** Under the platform directory, there must be a header file named `platform-defs.h` that provides definitions for platform-specific types and constants. See `arm1176jzf-s/platform-defs.h` as an example.
    1. **Platform-specific implementation:** Under the platform directory, there must one or more source files implementing the functions declared in `kernel/platform/interface.h`.
    1. **Platform-specific linker script:** Under the platform directory, the must be a linker script named `include.ld` that defines platform-specific variables (such as `LNK_KERNEL_ORG`) we use in kernel's linker script (`kernel/linker.ld`).
    1. **Platform-specific Makefile script:** Under the platform directory, there must be a Makefile script named `include.mk` that defines the variable `OBJFILES` i.e., the list of object files pertaining the source files in the platform directory. See `arm1176jzf-s/include.mk` as an example.
1. Add the proper bash script to the `configure` file to generate appropriate versions `kernel/config.h` and `config.mk` should the platform is selected in the `default-config.mk` script (using `TARGET_PLAT`).
