#include "atag.h"
#include <kernel.h>
#include <util/string.h>

static kparams_t params;
static kparams_mmap_entry_t pmmap[32];
static kparams_memory_t pmem;
static kparams_ramdisk_t pramdisk;

kparams_t* atag_to_kparams(size_t atags){
  atag_t* tag;
  int mmap_count;
  memset(&params, 0, sizeof(kparams_t));

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

  return &params;
}