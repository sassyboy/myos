#include <kernel.h>
#include <mem/physical.h>

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
      KInfo("\tBase: 0x%x, Size %d KB, Type: 0x%x\r\n",
        params->phys_mem->mmap[i].base_low,
        params->phys_mem->mmap[i].size_low / 1024,
        params->phys_mem->mmap[i].type);
  }
  extern void qInitPhysMemBitmap(kparams_memory_t*);
  qInitPhysMemBitmap(params->phys_mem);
  // Mark Kernel Pages as in use
  MarkMemFrames((phys_addr_t)&_kernel_start, 
                (int)(&_kernel_size)/MEM_FRAME_SZ, false);
  MarkMemFrames(0, 2, false);// reset vector and stuff

  // Parse the Command Line
  if (params->flags & KPARAMS_FL_CMDLINE){
    KInfo("Command Line: %s\r\n", params->cmdline);
  }

  // KInfo("Enabling IRQ Handling...\r\n");
  // InitIRQHandling();
  // EnableInterrupts();
  // SetTimeStampEvent(ReadTimeStamp() + 25000);
  // UnmaskIRQ(1);

  MarkMemFrames((phys_addr_t)params->ramdisk->addr,
    params->ramdisk->size/MEM_FRAME_SZ, false); // ramdisk.img
  KInfo("Now there are %d free frames\r\n", NumFreeFrames());
  extern void qDumpPhysBitmap();
  qDumpPhysBitmap();


  // Call into the user program
  typedef int (*user_main_t)();
  user_main_t f = (user_main_t)(params->ramdisk->addr);
  KInfo("Calling the user program @0x%x (size: %d KB)...\r\n\r\n", 
    (uint32_t)f, params->ramdisk->size/1024);
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