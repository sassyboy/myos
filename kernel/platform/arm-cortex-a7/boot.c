#include <kernel.h>
#include <platform/interface.h>
#include <util/stdio.h>
#include <util/string.h>
#include <modules/gpio/bcm.h>
#include <platform/arm-generic/atag-boot.h>

static kparams_t params;
static kparams_mmap_entry_t pmmap[32];
static kparams_memory_t pmem;
static kparams_ramdisk_t pramdisk;
static spinlock_t bootlock;
static int bsp_cpu = -1;
static inline void delay(int32_t count)
{
	asm volatile("__delay_%=: subs %[count], %[count], #1; bne __delay_%=\n"
		 : "=r"(count): [count]"0"(count) : "cc");
}

void arm_cortex_a7_init(uint32_t pcpu, uint32_t machine_id, uint32_t atags){
  (void)machine_id;
  extern void bcm_pl011uart_init();
  atag_t* tag;
  int mmap_count;
  
  // cpu0_only:
  SpinLock(&bootlock);
  // Setup early console
  bcm_pl011uart_init();

  if (bsp_cpu != -1)
    goto all_cpus;
  else
    bsp_cpu = pcpu;
  
  if (atags == 0){
    atags = 0x100; // The default address
    KWarn("[WARN] Assuming ATAG is located @ 0x100\r\n");
  }

  memset(&params, 0, sizeof(kparams_t));
  params.platform_string = "ARM-CORTEX-A7";

  tag = (atag_t*)atags;
  mmap_count = 0;
  while (tag->hdr.tag != ATAG_NONE){
    switch(tag->hdr.tag){
    case ATAG_CORE:
      break;
    case ATAG_MEM:
      pmmap[mmap_count].base_high = 0;
      pmmap[mmap_count].base_low = tag->u.mem.start;
      pmmap[mmap_count].size_high = 0;
      pmmap[mmap_count].size_low = tag->u.mem.size;
      pmmap[mmap_count].type = KPARAMS_MEM_TYPE_AVAILABLE;
      mmap_count++;
      break;
    case ATAG_VIDEOTEXT:
      break;
    case ATAG_RAMDISK:
      break;
    case ATAG_INITRD2:
      pramdisk.addr = tag->u.initrd2.start;
      pramdisk.size = tag->u.initrd2.size;
      params.flags |= KPARAMS_FL_RAMDISK;
      params.ramdisk = &pramdisk;
      break;
    case ATAG_CMDLINE:
      params.cmdline = tag->u.cmdline.cmdline;
      params.flags |= KPARAMS_FL_CMDLINE;
      break;
    }
    tag = atag_next(tag);
  }

  if (mmap_count){
    pmem.count = mmap_count;
    pmem.mmap = &pmmap[0];
    params.phys_mem = &pmem;
    params.flags |= KPARAMS_FL_MMAP;
  }

all_cpus:
  SpinUnlock(&bootlock);
	qKernelStart(&params, pcpu);
}

