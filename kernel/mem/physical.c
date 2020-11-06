#include "physical.h"

#define DEBUG_MEM_PHYSICAL
static uint8_t bitmap[MAX_FRAMES/8]; // 1: Free, 0: In use
static phys_addr_t last_frame_plus1 = 0;
static size_t num_free_frames = 0;

static inline void update_frame(phys_addr_t frame, bool free){
  size_t indx = frame >> 3;
  uint8_t msk = (uint8_t)1 << (frame & 0x7);
  if (free){
    bitmap[indx] |= msk;
    num_free_frames++;
  } else {
    num_free_frames--;
    bitmap[indx] &= ~msk;
  }
}
static inline bool is_free(phys_addr_t frame){
  size_t indx = frame >> 3;
  uint8_t msk = (uint8_t)1 << (frame & 0x7);
  return (bitmap[indx] & msk);
}

void MarkMemFrames(phys_addr_t addr, int count, bool free){
  // Implementation could be optimized for multiples of 8
  phys_addr_t i, start;
  start = MEM_ADDR_TO_FRAME_NUMBER(addr);
  for (i = start ; i < start + count ; i++)
    update_frame(i, free);
}

#ifdef DEBUG_MEM_PHYSICAL
void qDumpPhysBitmap(){
  phys_addr_t i;
  for (i = 0 ; i < last_frame_plus1 ; i++){
    if (i % 64 == 0)
      KInfo("%x\t", i);
    if (is_free(i))
      KInfo("F");
    else
      KInfo("X");
    if ((i + 1) % 64 == 0)
      KInfo("\r\n");
  }
  KInfo("\r\n");
}
#endif

static phys_addr_t alloc_frame(size_t count, bool go_high){
  phys_addr_t i, j;
  phys_addr_t base = INVALID_PHYS_ADDR;
  size_t c = 0;
  if (go_high){
    for (i = 0 ; i < last_frame_plus1 ; i++){
      if (!bitmap[i>>3]){
        // No free frame in this item
        i = ((i>>3)+1)<<3; // skip to the next bitmap item
        c = 0;
        base = INVALID_PHYS_ADDR;
        continue;
      }
      if (is_free(i)){
        // This one's free!
        if (base == INVALID_PHYS_ADDR)
          base = i;
        c++;
      } else {
        c = 0;
        base = INVALID_PHYS_ADDR;
      }
      if (c == count){
        base = MEM_FRAME_NUM_TO_ADDR(base);
        MarkMemFrames(base, c, false);
        break;
      }
    }
  } else {
    for (i = last_frame_plus1 - 1 ; i >= 0 ; i--){
      if (!bitmap[i>>3]){
        // No free frame in this item
        i = ((i>>3)-1)<<3; // skip to the next bitmap item
        c = 0;
        base = INVALID_PHYS_ADDR;
        continue;
      }
      if (is_free(i)){
        // This one's free!
        c++;
      } else {
        c = 0;
        base = INVALID_PHYS_ADDR;
      }
      if (c == count){
        base = MEM_FRAME_NUM_TO_ADDR(i);
        MarkMemFrames(base, c, false);
        break;
      }
    }
  }
  return base;
}

phys_addr_t AllocMemFrame(){
  return alloc_frame(1, true);
}

phys_addr_t AllocContinuousMemFrames(size_t count){
  if (count < 1)
    return INVALID_PHYS_ADDR;
  return alloc_frame(count, true);
}

phys_addr_t AllocHighMemFrame(){
  return alloc_frame(1, false);
}

phys_addr_t AllocContinuousHighMemFrames(size_t count){
  if (count < 1)
    return INVALID_PHYS_ADDR;
  return alloc_frame(count, false);
}

void FreeMemFrame(phys_addr_t addr){
  MarkMemFrames(addr, 1, true);
}
void FreeMemFrames(phys_addr_t addr, size_t count){
  MarkMemFrames(addr, count, true);
}

size_t NumFreeFrames(){
  return num_free_frames;
}

void qInitPhysMemBitmap(kparams_memory_t* pmem){
  int i;
  // Mark the free memory
  for (i = 0 ; i < pmem->count ; i++){
    if (pmem->mmap[i].type != KPARAMS_MEM_TYPE_AVAILABLE)
      continue;
    MarkMemFrames(pmem->mmap[i].base_low,
                  pmem->mmap[i].size_low / MEM_FRAME_SZ, true);
    if (pmem->mmap[i].base_low + pmem->mmap[i].size_low > last_frame_plus1)
      last_frame_plus1 = MEM_ADDR_TO_FRAME_NUMBER(
        pmem->mmap[i].base_low + pmem->mmap[i].size_low);
  }
  KInfo("\r\nFree Frames: %d, Last Frame + 1 = %d\r\n", num_free_frames, last_frame_plus1);
}
