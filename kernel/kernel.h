#ifndef KERNEL_H
#define KERNEL_H

#include <config.h>
#include <stddef.h>
#include <stdint.h>
#include <platform/interface.h>
#include <util/stdio.h>

/**
 * Useful symbols from the linker script
 */
extern uint32_t _kernel_start, _kernel_end, _kernel_size;
extern uint32_t _kernel_text_start, _kernel_text_end, _kernel_text_size;

typedef struct{
  int cpu;
  const char* platform_string;
  uint32_t ramdisk_addr;
  uint32_t ramdisk_size;
} kstartup_params_t;

/**
 * @fn qKernelStart: Peform platform-independent initialization of the kernel
 * 
 * @pre Interrupts disabled
 */
void qKernelStart(kstartup_params_t* params);

#endif
