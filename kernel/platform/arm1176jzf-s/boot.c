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

void arm1176jzfs_init(uint32_t r0, uint32_t machine_id, uint32_t atags){
  (void)machine_id;
  // Setup early console
  extern void uart_bcm_init(); uart_bcm_init();
  if (r0){
    KPanic("[PANIC] Corrupted Boot Info\r\n");
  }
  if (atags == 0){
    atags = 0x100; // The default address
    KWarn("[WARN] Assuming ATAG is located @ 0x100\r\n");
  }
	
  memset(&params, 0, sizeof(kparams_t));
  params.platform_string = "ARM1176JZF-S";

  atag_t* tag = (atag_t*)atags;
  int mmap_count = 0;
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

	qKernelStart(&params, 0);
}

