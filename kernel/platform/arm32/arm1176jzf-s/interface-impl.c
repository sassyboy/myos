#include <platform/interface.h>

/**
 * Memory Management Interface
 * 
 * The Translation Table Base Register (TTBR in CP15 register 2) holds the
 * physical address of the base of the first-level table.
 * 
 * VMSAv6 introduced an additional translation table base register and a
 * translation table base control register: TTBR0, TTBR1 and TTBCR.
 * 
 * - TTBR0 is expected to be used for process specific addresses.
 * - TTBR1 is expected to be used for operating system and I/O addresses, which
 *   do not change on a context switch.
 * - The size of the TTBR1 table is always 16KB, but the TTBR0 table ranges in
 *   size from 128 bytes to 16KB, depending on the value (N) in the TTBCR,
 *   where N = 0 to 7.
 */
// size_t GetMMUTranslationBaseAddress(){

//   // asm volatile(
//   //   "mrc p15,0,r2,c1,c0,0;"
//   //   "orr r2, #0x0001;"
//   //   "mcr p15,0,r2,c1,c0,0;"
//   // );
// }
// void SetMMUTranslationBaseAddress(size_t base_addr);

void DisableInterrupts(){
  asm volatile("cpsid if");
}

void EnableInterrupts(){
  asm volatile("cpsie if");
}

void InitIRQHandling(){
  extern void bcm_intctrl_init();
  bcm_intctrl_init();
}
void MaskIRQ(int irqn){
  extern int bcm_intctrl_irq_mask(int);
  bcm_intctrl_irq_mask(irqn);
}
void UnmaskIRQ(int irqn){
  extern int bcm_intctrl_irq_unmask(int);
  bcm_intctrl_irq_unmask(irqn);
}
void RegisterIRQHandler(int irqn, irq_handler_t func){
  extern void bcm_intctrl_reg_handler(int, irq_handler_t);
  bcm_intctrl_reg_handler(irqn, func);
}

uint32_t GetTimeStampFreq(){
  return 1000000UL;
}

timestamp_t ReadTimeStamp(){
  extern uint64_t systimer_bcm_read_tsc();
  return systimer_bcm_read_tsc();
}

void SetTimeStampEvent(timestamp_t tsc){
  extern void systimer_bcm_set_point(uint32_t t);
  systimer_bcm_set_point((uint32_t)tsc);
}

void DelayUntil(timestamp_t ts){
  timestamp_t now;
  do{
    now = ReadTimeStamp();
  }while(now < ts);
}
void DelayMicro(uint64_t us){
  timestamp_t end = ReadTimeStamp() + us;
  DelayUntil(end);
}
void DelayNano(uint64_t ns){
  if (ns < 1000)
    return; // This platform does not support sub-microsecond delays
  timestamp_t end = ReadTimeStamp() + ns/1000;
  DelayUntil(end);
}

// We don't need spinlocks in a single core system
void SpinLockInit(spinlock_t* lock){
  (void)lock;
}

int SpinLockAttempt(spinlock_t* lock){
  (void)lock;
	return 0;
}
void SpinLock(spinlock_t* lock){
	(void)lock;
}
void SpinUnlock(spinlock_t* lock){
  (void)lock;
}

void HaltCPU(){
  asm volatile("wfe");
}