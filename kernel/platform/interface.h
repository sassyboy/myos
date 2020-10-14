#ifndef PLAT_INTERFACE_H
#define PLAT_INTERFACE_H
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/**
 * Platform-specific Definitions:
 * typedef timestamp_t
 */
#include <platform-defs.h>

/**
 * Interrupts and System Call interface
 */
typedef void (*interrupt_handler_t)(int vector);
typedef void (*syscall_handler_t)(int syscall_no, void* params);
void DisableInterrupts();
void EnableInterrupts();
bool InterruptsEnabled();

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

#endif
