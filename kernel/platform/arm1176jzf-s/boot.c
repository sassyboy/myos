#include <kernel.h>
#include <platform/interface.h>
#include <util/stdio.h>
#include <modules/gpio/bcm.h>

void arm1176jzfs_init(uint32_t r0, uint32_t r1, uint32_t atags){
  (void)r0;
  (void)r1;
  (void)atags;
	extern void uart_bcm_init(); uart_bcm_init();

  SetGpioDirection(16, GPIO_OUT);
  SetGpioDirection(20, GPIO_OUT);
  SetGpioDirection(21, GPIO_OUT);
  SetGpio(16, 1);
  SetGpio(20, 1);
  SetGpio(21, 1);
  kstartup_params_t params;
  params.cpu = 0;
  params.platform_string = "ARM1176JZF-S";
  params.ramdisk_addr = 0x200000;
  params.ramdisk_size = 16 * 1024;
	qKernelStart(&params);
}

