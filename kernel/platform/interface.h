#ifndef PLAT_INTERFACE_H
#define PLAT_INTERFACE_H
#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

/**
 * Platform-specific Definitions:
 * typedef timestamp_t
 */
#include <platform-defs.h>

/**
 * Memory Management Interface
 */
size_t GetMMUTranslationBaseAddress();
void SetMMUTranslationBaseAddress(size_t base_addr);

/**
 * Interrupts and System Call interface
 */
typedef void (*irq_handler_t)(int irqn);
typedef void (*syscall_handler_t)(int syscall_no, void* params);
void DisableInterrupts();
void EnableInterrupts();
bool InterruptsEnabled();

/**
 * System IRQ Controller interface
 */
void InitIRQHandling();
void MaskIRQ(int irqn);
void UnmaskIRQ(int irqn);
void RegisterIRQHandler(int irqn, irq_handler_t func);

/**
 * System Timer Interface
 */
timestamp_t ReadTimeStamp();
uint32_t GetTimeStampFreq();
void SetTimeStampEvent(timestamp_t tsc);
timestamp_t GetTimeStampEvent();

/**
 * Busy-Waiting Delay Primitives
 */
void DelayUntil(timestamp_t ts);
void DelayMicro(uint64_t us);
void DelayNano(uint64_t ns);

/**
 * Memory Barrier/Fence Instructions
 * 
 * @fn MemFence: A full system memory barrier. It ensures that all explicit
 *               memory accesses that appear in program order before
 *               MemFence() are observed before any explicit memory
 *               accesses that appear in program order after MemFence().
 * @fn MemFenceRd: Similar to MemFence(), but waits only for memory read
 *                 operations to complete.
 * @fn MemFenceWr: Similar to MemFence(), but waits only for memory write
 *                 operations to complete.
 * 
 * Note that MemFence*() functions may not affect the ordering of any other
 * instructions executing on the processor.
 * 
 * NOTE: These functions must be defined as static inline functions inside
 * the corresponding platform-defs.h
 */


/**
 * Synchronization
 */
void SpinLockInit(spinlock_t* lock);
int SpinLockAttempt(spinlock_t* lock);
void SpinLock(spinlock_t* lock);
void SpinUnlock(spinlock_t* lock);

/**
 * Processor Calls
 */
void HaltCPU();
#endif
