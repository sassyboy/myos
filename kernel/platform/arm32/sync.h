/**
 * Synchronization Primitives for the ARM32 architecture
 */

#ifndef PLAT_ARM32_SYNC_H
#define PLAT_ARM32_SYNC_H

#include <platform/interface.h>

static inline void spinlock_lock(uint32_t* var){
	unsigned long tmp;
  asm volatile(
    "1: ldrex   %0, [%1]\n"
    "	  teq     %0, #0\n"
    "   wfene\n"
    "	  strexeq %0, %2, [%1]\n"
    "   teqeq	  %0, #0\n"
    "	  bne	1b"
 	  : "=&r" (tmp) : "r" (var), "r" (1) : "cc"
  );
  MemFence();
}

static inline void spinlock_unlock(uint32_t* var){
  MemFence();
	asm volatile(
    "str	%1, [%0]\n"
    "dsb\n"
    "sev\n"
	: : "r" (var), "r" (0) : "cc"
  );
}

static inline int spinlock_trylock(uint32_t* var){
  unsigned long tmp;
  asm volatile(
    "1: ldrex   %0, [%1]\n"
    "	  teq     %0, #0\n"
    "   bne 2f\n"
    "	  strexeq %0, %2, [%1]\n"
    "2:"
 	  : "=&r" (tmp) : "r" (var), "r" (1) : "cc"
  );
  return tmp;
}


#endif
