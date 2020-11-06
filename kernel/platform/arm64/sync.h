/**
 * Synchronization Primitives for the ARM64 architecture
 * 
 * Copied from the Jailhouse Project
 */ 

#ifndef PLAT_ARM64_SYNC_H
#define PLAT_ARM64_SYNC_H

#include <platform/interface.h>

#define TICKET_SHIFT	16

static inline void spinlock_lock(spinlock_t *lock){
  // unsigned int tmp;
	// spinlock_t lockval, newval;
	// asm volatile(
	//   /* Atomically increment the next ticket. */
  //   "	prfm	pstl1strm, %3\n"
  //   "1:	ldaxr	%w0, %3\n"
  //   "	add	%w1, %w0, %w5\n"
  //   "	stxr	%w2, %w1, %3\n"
  //   "	cbnz	%w2, 1b\n"
	//   /* Did we get the lock? */
  //   "	eor	%w1, %w0, %w0, ror #16\n"
  //   "	cbz	%w1, 3f\n"
	//   /*
	//   * No: spin on the owner. Send a local event to avoid missing an
	//   * unlock before the exclusive load.
	//   */
  //   "	sevl\n"
  //   "2:	wfe\n"
  //   "	ldaxrh	%w2, %4\n"
  //   "	eor	%w1, %w2, %w0, lsr #16\n"
  //   "	cbnz	%w1, 2b\n"
	//   /* We got the lock. Critical section starts here. */
  //   "3:"
	//   : "=&r" (lockval), "=&r" (newval), "=&r" (tmp), "+Q" (*lock)
	//   : "Q" (lock->owner), "I" (1 << TICKET_SHIFT)
	//   : "memory"
  // );
}

static inline int spinlock_trylock(spinlock_t *lock){
	// unsigned int tmp;
	// spinlock_t lockval;

	// asm volatile(
	//   /* Using LL/SC */
	//   "	prfm	pstl1strm, %2\n"
	//   "1:	ldaxr	%w0, %2\n"
	//   "	eor	%w1, %w0, %w0, ror #16\n"
	//   "	cbnz	%w1, 2f\n"
	//   "	add	%w0, %w0, %3\n"
	//   "	stxr	%w1, %w0, %2\n"
	//   "	cbnz	%w1, 1b\n"
	//   "2:"
	//   : "=&r" (lockval), "=&r" (tmp), "+Q" (*lock)
	//   : "I" (1 << TICKET_SHIFT)
	//   : "memory"
  // );
	// return !tmp;
  return 0;
}

static inline void spinlock_unlock(spinlock_t *lock) {
  // asm volatile(
  //   "	stlrh	%w1, %0\n"
	// : "=Q" (lock->owner)
	// : "r" (lock->owner + 1)
	// : "memory");
}

#endif
