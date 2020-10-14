#include <kernel.h>
#include <platform/interface.h>
#include <modules/gpio/bcm.h>
#include <util/stdio.h>

void qKernelStart(kstartup_params_t* params){
  KInfo("In %s() %d 0x%x\r\n",__FUNCTION__, -12, 255);
  KInfo("\tPlatform Name: %s, PCPU: %d\r\n",params->platform_string, params->cpu);  
  timestamp_t start, end;
  start = ReadTimeStamp();
  DelayMicro(25000);
  end = ReadTimeStamp();
  KInfo("\tCurrent TimeStamp: 0x%x\r\n", (uint32_t)start);
  KInfo("\tCurrent TimeStamp: 0x%x\r\n", (uint32_t)end);
  while(1){}
}
