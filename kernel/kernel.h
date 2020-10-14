#ifndef KERNEL_H
#define KERNEL_H

#include <config.h>
#include <stddef.h>
#include <stdint.h>

typedef struct{
  int cpu;
  const char* platform_string;
} kstartup_params_t;

/**
 * @fn qKernelStart: Peform platform-independent initialization of the kernel
 * 
 * @pre Interrupts disabled
 */
void qKernelStart(kstartup_params_t* params);

#endif
