#include <kernel.h>
#include <boot/fdt.h>
#include <modules/gpio/bcm.h>

// static spinlock_t bootlock;
// static int bsp_cpu = -1;

static kparams_t params;
static kparams_ramdisk_t pramdisk;
// Look at the following link for the boot protocol
// https://www.kernel.org/doc/Documentation/arm64/booting.txt
void arm_cortex_a72_init(uint64_t dtbp, uint64_t x1, uint64_t x2, uint64_t x3, uint64_t x4){
  // (void)machine_id;
  
  // // cpu0_only:
  // SpinLock(&bootlock);
  // if (bsp_cpu != -1) goto all_cpus;
  // bsp_cpu = pcpu;
  
  // Setup early console
#ifdef PLAT_BCM2837B0
// HACK for pi3 on QEMU
// QEMU uses UART0 instead of the miniUART :D
  extern void bcm_pl011uart_init();
  bcm_pl011uart_init();
#else
  extern void bcm_miniuart_init();
  bcm_miniuart_init();
#endif
  fdt_header_t* fdth = (fdt_header_t*)dtbp;
  KInfo("DTB stored @ 0x%x [MAGIC: 0x%x, ver: 0x%x, size: %d]\r\n",
    (uint32_t)dtbp, ben2len_u32(fdth->magic), ben2len_u32(fdth->version), 
    ben2len_u32(fdth->totalsize));
  
  KInfo("KERN ENTRY @ 0x%x\r\n", (uint32_t)x4);
  SetGpioDirection(21, 1);
  SetGpio(21, 1);
  // Gather the boot information  
  // We are getting a device tree! should add support for that!
  params.cmdline = NULL;
  pramdisk.addr = 0x00200000;
  pramdisk.size = 8 * 1024;
  params.ramdisk = &pramdisk;
  params.platform_string = "ARM-CORTEX-A72";
  params.flags = KPARAMS_FL_RAMDISK;
  
  // Continue with the platform-independent startup!
// all_cpus:
//   SpinUnlock(&bootlock);
	qKernelStart(&params, 0);
}
