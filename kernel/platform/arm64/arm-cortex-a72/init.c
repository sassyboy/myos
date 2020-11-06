#include <kernel.h>
#include <boot/fdt.h>
#include <modules/gpio/bcm.h>

static spinlock_t bootlock;
// static int bsp_cpu = -1;

static kparams_t params;
static kparams_ramdisk_t pramdisk;

void initcpu(){
  // SWITCH to EL1 if currently in EL2(i.e., hypervisor more)
  // See https://community.arm.com/developer/tools-software/tools/b/tools-software-ides-blog/posts/changing-exception-level-and-security-state-with-an-armv8a-fixed-virtual-platform

  // Invalidate icache and tlbs
  asm volatile(
    "ic	iallu;"
    "tlbi	alle2;"
  );

  /**
   * Enable Data/Instruction caches
   * No need to enable branch prediction according to:
   * https://developer.arm.com/documentation/100095/0001/level-1-memory-system/program-flow-prediction/enabling-program-flow-prediction
   */
  asm volatile(
    "mrs x0, SCTLR_EL2;"
    "orr x0, x0, #0x0004;"
    "orr x0, x0, #0x1000;"
    "msr SCTLR_EL2, x0;"
    "isb;"
  );
}
// Look at the following link for the boot protocol
// https://www.kernel.org/doc/Documentation/arm64/booting.txt
void arm_cortex_a72_init(uint64_t dtbp, uint64_t cpuid, uint64_t stack_base,
  uint64_t x3, uint64_t kern_entry){
  (void)x3;
  initcpu();
  // BSP-only initialization
  if (cpuid != 0)
    goto all_cpus;
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
  
  uint64_t currentEL;
  asm volatile("mrs %0, CurrentEL": "=r"(currentEL));
  KInfo("KERN ENTRY @ 0x%x CurrentEL:%d\r\n", (uint32_t)kern_entry, currentEL);
  // Gather the boot information  
  // We are getting a device tree! should add support for that!
  params.cmdline = NULL;
  pramdisk.addr = 0x00200000;
  pramdisk.size = 8 * 1024;
  params.ramdisk = &pramdisk;
  params.platform_string = "ARM-CORTEX-A72";
  params.flags = KPARAMS_FL_RAMDISK;
  
  /**
   * Raspberry-Pi firmware for ARMv8 does not start the APs until we write a
   * starting address for AP-code into the following vectors:
   * spin_cpu0 @0xd8 : .quad 0
   * spin_cpu1 @0xe0 : .quad 0
   * spin_cpu2 @0xe8 : .quad 0
   * spin_cpu3 @0xf0 : .quad 0
   * 
   * see: https://github.com/raspberrypi/tools/blob/master/armstubs/armstub8.S
   */
  SpinLockInit(&bootlock);
#if defined(PLAT_BCM2837B0) || defined(PLAT_BCM2711)
  *(volatile uint64_t*)0xe0 = kern_entry;
  *(volatile uint64_t*)0xe8 = kern_entry;
  *(volatile uint64_t*)0xf0 = kern_entry;
#endif

all_cpus:
  // SpinLock(&bootlock);
  KInfo("CPU %d going into qKernelStart, stack base: 0x%x\r\n",
    cpuid, stack_base);
  // SpinUnlock(&bootlock);
  // All CPUS:
  // Continue with the platform-independent startup!
	qKernelStart(&params, cpuid);
}

