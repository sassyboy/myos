#include <platform/interface.h>

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