#include <platform/interface.h>
#include "../sync.h"

void DisableInterrupts(){
  // asm volatile("cpsid if");
}

void EnableInterrupts(){
  // asm volatile("cpsie if");
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
  lock->owner = 0;
  lock->next = 0;
}

int SpinLockAttempt(spinlock_t* lock){
	return spinlock_trylock(lock);
}
void SpinLock(spinlock_t* lock){
	spinlock_lock(lock);
}
void SpinUnlock(spinlock_t* lock){
  spinlock_unlock(lock);
}

void HaltCPU(){
  asm volatile("wfe");
}