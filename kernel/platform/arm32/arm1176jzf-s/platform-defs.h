#ifndef _PLATFORM_DEFS_H
#define _PLATFORM_DEFS_H

#include <stdint.h>
typedef uint64_t timestamp_t;

static inline void MemFenceRd(){
  asm volatile("dmb ishld" ::: "memory");
}
static inline void MemFenceWr(){
  asm volatile("dmb ishst" ::: "memory");
}
static inline void MemFenceAll(){
  asm volatile("dmb ish" ::: "memory");
}

typedef struct{
} spinlock_t;

// Max Physical Memory Size in FRAMES
#define MAX_FRAMES    (0x100000)
// Default size of a physical memory frame
#define MEM_FRAME_SZ  (0x1000)
#define MEM_ADDR_TO_FRAME_NUMBER(addr)  ((addr)>>12)
#define MEM_FRAME_NUM_TO_ADDR(frm) ((frm)<<12)
// Default size of a virtual memory page
#define MEM_PAGE_SZ   (0x1000)

#endif