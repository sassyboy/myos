#ifndef MEM_PHYSICAL_H
#define MEM_PHYSICAL_H

#include <kernel.h>

typedef size_t phys_addr_t;
#define INVALID_PHYS_ADDR   ((phys_addr_t)-1)

/**
 * @fn AllocMemFrame
 * @brief Starting from 0x0, finds a free physical memory frame, marks it as
 *        used, and returns its base address.
 *        The base address is be divisible by MEM_FRAME_SZ.
 * 
 * @return base address of the free physical frame or INVALID_PHYS_ADDR if
 *         no free physical memory was found.
 */
phys_addr_t AllocMemFrame();

/**
 * @fn AllocHighMemFrame
 * @brief Similar to AllocMemFrame, but start searching from the highest
 *        available physical memory address.
 * 
* @return base address of the free physical frame or INVALID_PHYS_ADDR if
 *         no free physical memory was found.
 */
phys_addr_t AllocHighMemFrame();

phys_addr_t AllocContinuousHighMemFrames(size_t count);

phys_addr_t AllocContinuousMemFrames(size_t count);

/**
 * @fn MarkMemFrame
 */
void MarkMemFrames(phys_addr_t addr, int count, bool free);
void FreeMemFrame(phys_addr_t addr);
void FreeMemFrames(phys_addr_t addr, size_t count);

size_t NumFreeFrames();
#endif