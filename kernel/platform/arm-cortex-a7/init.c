#include <kernel.h>

static spinlock_t bootlock;
static int bsp_cpu = -1;

void arm_cortex_a7_init(uint32_t pcpu, uint32_t machine_id, uint32_t atags){
  (void)machine_id;
  extern void bcm_pl011uart_init();
  kparams_t* params = NULL;

  // cpu0_only:
  SpinLock(&bootlock);
  if (bsp_cpu != -1) goto all_cpus;
  bsp_cpu = pcpu;
  
  // Setup early console
  bcm_pl011uart_init();

  // Gather the boot information
  if (atags == 0){
    atags = 0x100; // The default address
    KWarn("[WARN] Assuming ATAG is located @ 0x100\r\n");
  }
  
  extern kparams_t* atag_to_kparams(uint32_t);
  params = atag_to_kparams(atags);
  params->platform_string = "ARM-CORTEX-A7";
  
  // Continue with the platform-independent startup!
all_cpus:
  SpinUnlock(&bootlock);
	qKernelStart(params, pcpu);
}

