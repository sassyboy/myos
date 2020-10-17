#include <kernel.h>

void qKernelStart(kparams_t* params, int pcpu){
  KernelLock();
  KInfo("Loading %s ...\r\n", KERNEL_STRING);
  if (pcpu!=0){
    KInfo("No SMP support for now. Halting PCPU %d !\r\n", pcpu);
    KernelUnlock();
    while(1){
      HaltCPU();
    }
  }
  KInfo("Kernel @ 0x%x (Size: %d KBs)\r\n",
    &_kernel_start, (uint32_t)(&_kernel_size)/1024);

  if (params->flags & KPARAMS_FL_RAMDISK)
    KInfo("Ramdisk @ 0x%x (Size: %d KBs)\r\n",
      params->ramdisk->addr, params->ramdisk->size/1024);
  
  // Traverse the Memory Map
  int i;
  if (params->flags & KPARAMS_FL_MMAP){
    KInfo("Memory Map:\r\n");
    for (i = 0; i < params->phys_mem->count; i++)
      KInfo("\tBase: 0x%x, Size 0x%d KB, Type: 0x%x\r\n",
        params->phys_mem->mmap[i].base_low,
        params->phys_mem->mmap[i].size_low / 1024,
        params->phys_mem->mmap[i].type);
  }

  // Parse the Command Line
  if (params->flags & KPARAMS_FL_CMDLINE){
    KInfo("Command Line: %s\r\n", params->cmdline);
  }

  // KInfo("Enabling IRQ Handling...\r\n");
  // InitIRQHandling();
  // EnableInterrupts();
  // SetTimeStampEvent(ReadTimeStamp() + 25000);
  // UnmaskIRQ(1);

  // Call into the user program
  typedef int (*user_main_t)();
  user_main_t f = (user_main_t)(params->ramdisk->addr);
  KInfo("Calling the user program @0x%x...\r\n\r\n", (uint32_t)f);
  KernelUnlock();
  int res = f();
  KInfo("\r\nUser Program returned %d\r\n", res);
  KPanic("[PANIC] Nothing more to do!");
}

static spinlock_t klock;
void KernelLock(){
  SpinLock(&klock);
}
void KernelUnlock(){
  SpinUnlock(&klock);
}

void KPanic(const char* err){
  KError(err);
  while (1){}
}