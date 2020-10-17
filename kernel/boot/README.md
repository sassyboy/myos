# Boot helper structures/functions

This directory contains data structures and helper functions to decode
the following boot information

- ATAG: Used in 32-bit ARM platforms.

- FDT (Flattened Device Trees): Used in both 32-bit and 64-bit ARM platforms. It's mandatory for the 64-bit ones.

- Multiboot 2: Mostly used in x86 and x86_64 PC platforms. GRUB uses the Multiboot standard to inform the kernel about the system.
