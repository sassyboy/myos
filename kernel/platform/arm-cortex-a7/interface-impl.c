#include <platform/interface.h>

void DisableInterrupts(){
  asm volatile("cpsid if");
}

void EnableInterrupts(){
  asm volatile("cpsie if");
}

uint32_t GetTimeStampFreq(){
  return 1000000UL;
}

timestamp_t ReadTimeStamp(){
  extern uint64_t systimer_bcm_read_tsc();
  return systimer_bcm_read_tsc();
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

void SpinLockInit(spinlock_t* lock){
  lock->m = 0;
}

int SpinLockAttempt(spinlock_t* lock){
	unsigned long tmp;
  asm volatile(
    "1: ldrex   %0, [%1]\n"
    "	  teq     %0, #0\n"
    "   bne 2f\n"
    "	  strexeq %0, %2, [%1]\n"
    "2:"
 	  : "=&r" (tmp) : "r" (&lock->m), "r" (1) : "cc"
  );
  return tmp;
}
void SpinLock(spinlock_t* lock){
	unsigned long tmp;
  asm volatile(
    "1: ldrex   %0, [%1]\n"
    "	  teq     %0, #0\n"
    "   wfene\n"
    "	  strexeq %0, %2, [%1]\n"
    "   teqeq	  %0, #0\n"
    "	  bne	1b"
 	  : "=&r" (tmp) : "r" (&lock->m), "r" (1) : "cc"
  );
  MemFence();
}
void SpinUnlock(spinlock_t* lock){
  MemFence();
	asm volatile(
    "str	%1, [%0]\n"
    "dsb\n"
    "sev\n"
	: : "r" (&lock->m), "r" (0) : "cc"
  );
}

void HaltCPU(){
  asm volatile("wfe");
}