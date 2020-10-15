#include <kernel.h>

void arm1176jzfs_init(uint32_t r0, uint32_t machine_id, uint32_t atags){
  (void)machine_id;
  // Setup early console
  extern void bcm_miniuart_init(); bcm_miniuart_init();
  if (r0){
    KPanic("[PANIC] Corrupted Boot Info\r\n");
  }
  if (atags == 0){
    atags = 0x100; // The default address
    KWarn("[WARN] Assuming ATAG is located @ 0x100\r\n");
  }
	
  extern kparams_t* atag_to_kparams(uint32_t);
  kparams_t* params = atag_to_kparams(atags);
  params->platform_string = "ARM1176JZF-S";

	qKernelStart(params, 0);
}

