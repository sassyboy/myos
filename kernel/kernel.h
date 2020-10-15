#ifndef KERNEL_H
#define KERNEL_H

#include <config.h>
#include <stddef.h>
#include <stdint.h>
#include <platform/interface.h>
#include <util/stdio.h>

/******************************************************************************
 * Useful symbols from the linker script                                      *
 ******************************************************************************/
extern uint32_t _kernel_start, _kernel_end, _kernel_size;
extern uint32_t _kernel_text_start, _kernel_text_end, _kernel_text_size;


/******************************************************************************
 * Kernel start-up data structures and routines                               *
 ******************************************************************************/

#define KPARAMS_MEM_TYPE_AVAILABLE              1
#define KPARAMS_MEM_TYPE_RESERVED               2
#define KPARAMS_MEM_TYPE_ACPI_RECLAIMABLE       3
#define KPARAMS_MEM_TYPE_NVS                    4
#define KPARAMS_MEM_TYPE_BADRAM                 5

typedef struct{
  uint32_t base_high;
  uint32_t base_low;
  uint32_t size_high;
  uint32_t size_low;
  uint32_t type;
} kparams_mmap_entry_t;

typedef struct{
  int count;
  kparams_mmap_entry_t* mmap;
} kparams_memory_t;

typedef struct{
  uint32_t addr;
  size_t size; // In bytes
} kparams_ramdisk_t;

typedef struct{
  int pcpu_count;
} kparams_smp_t;

#define KPARAMS_FL_MMAP       0x1
#define KPARAMS_FL_RAMDISK    0x2
#define KPARAMS_FL_CMDLINE    0x4
#define KPARAMS_FL_VIDEO_FB   0x8
#define KPARAMS_FL_ROOTDEV    0x10
#define KPARAMS_FL_SMP        0x20
#define KPARAMS_FL_AMP        0x40

typedef struct{
  uint32_t              flags;
  const char*           platform_string;
  kparams_memory_t*     phys_mem;
  kparams_ramdisk_t*    ramdisk;
  char*                 cmdline;
  kparams_smp_t*        smp;
} kparams_t;

/**
 * @fn qKernelStart: Peform platform-independent initialization of the kernel
 * 
 * @pre Interrupts disabled
 */
void qKernelStart(kparams_t* params, int pcpu);

void KernelLock();
void KernelUnlock();

/******************************************************************************
 * Error Handling                                                             *
 ******************************************************************************/
void KPanic(const char* err);
#endif
