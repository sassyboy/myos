#include <kernel.h>
#include <modules/gpio/bcm.h>

void qKernelStart(kstartup_params_t* params){
  KInfo("In %s():\r\n",__FUNCTION__);
  KInfo("\tPlatform Name: %s, PCPU: %d\r\n",params->platform_string, params->cpu);  
  KInfo("\tKernel @ 0x%x (Size: %d KBs)\r\n",
    &_kernel_start, (uint32_t)(&_kernel_size)/1024);

  KInfo("\tRamdisk @ 0x%x (Size: %d KBs)\r\n",
    params->ramdisk_addr, params->ramdisk_size/1024);
  
  timestamp_t start, end;
  start = ReadTimeStamp();
  DelayMicro(25000);
  end = ReadTimeStamp();
  KInfo("\tCurrent TimeStamp: 0x%x\r\n", (uint32_t)start);
  KInfo("\tCurrent TimeStamp: 0x%x\r\n", (uint32_t)end);
  KInfo("\tCalling the user program...\r\n");
  typedef int (*user_main_t)();
  user_main_t f = (user_main_t)params->ramdisk_addr;
  int res = f();
  KInfo("User Program returned %d\r\n", res);
  KInfo("End of %s()\r\n", __FUNCTION__);
  while(1){}
}
