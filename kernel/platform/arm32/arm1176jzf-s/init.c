#include <kernel.h>
#include "../inlineasm.h"
/**
 * @ref ARM Architecture Reference Manual - ARM DDI 0100I
 */

// Each entry of the 1st-level table describes 1MB. We want to support 4GBs,
// Therefore we need 2^12 or 0x1000 items in it:
// It must be located in the BSS section at a 16KB boundary
uint32_t global_pg_dir[0x1000]  __attribute__ ((aligned(16*1024)));

static void init_mmu(){
  uint32_t ctrl; //CP15 Register 1 (The Control Register)
  ctrl = read_c1_ctrl_reg();

  /** Select PageTable format to ARMv6
   * We don't want to be backwards-compatible with ARMv4/v5.
   * The page table format is selected using the XP bit in CP15 register 1. When
   * subpage AP bits are enabled (CP15 register 1 XP = 0), the page table
   * formats are backwards compatible with ARMv4/v5
   */
  ctrl |= C1_CTRL_XP;

  /** Setup Translation Table Base */
  write_c2_ttbcr(0); // TTB is 16KB aligned and we only use TTBR0
  write_c2_ttbr0((uint32_t)&global_pg_dir); // TTBs are not cacheable


  /** Setup Memory Domains
   * The ARM architecture supports 16 domains. Each page table entry and TLB
   * entry contains a field that specifies which domain the entry is in. Access
   * to each domain is controlled by a two-bit field in the
   * Domain Access Control Register.
   * 
   * We use domain 0 for the entire memory. Make sure access are all checked
   * against the TLB
   */
  write_c3_dacr(DOMAC_CLIENT);

  /** Setup global pages (Identity-map each MB for now)
   * TEX, C, B = 000 0 0 i.e. Strongly Ordered for everywhere but the first 4MB i.e. kernel
   * XN (Execute Never) = 0
   * Domain = 0
   * APX AP = 0 01
   * S (Shared) = 0
   * nG (Not Global) = 0
   */
  uint32_t virt_addr_mb;
  uint32_t cacheopts;
  for (virt_addr_mb = 0 ; virt_addr_mb < 0x1000 ; virt_addr_mb++){
    if (virt_addr_mb < 5)
      cacheopts = 0xC; // Cacheable and Bufferable
    else
      cacheopts = 0; // Strongly ordered
    global_pg_dir[virt_addr_mb] = (0x2) // This is a Section Descriptor
                                | cacheopts
                                | (1ul << 10) // AP = 01 Privileged Access Only
                                | (virt_addr_mb << 20);
  }

  /** ENABLE MMU
   * According to Section B4.2.3 - Enabling and disabling the MMU :
   * Before the MMU is enabled all relevant CP15 registers must be programmed.
   * See Section B4.9 CP15 registers
   * This includes setting up suitable translation tables in memory.
   * Prior to enabling the MMU, the instruction cache should be disabled and
   * invalidated. The instruction cache can then be re-enabled at the same
   * time as the MMU is enabled.
   */
  
  ctrl |= C1_CTRL_M;
  write_c1_ctrl_reg(ctrl);
}

static void cpuinit(){
  init_mmu();
  // Switch to Supervisor mode
  // mrs r0, cpsr
  // bic r0, r0, #0x1F     // clear mode bits
  // orr r0, r0, #0x13     // set Supervisor mode
  // msr spsr_cxsf, r0
  // add r0, pc, #4        // hold (in ELR_hyp) the address to return to  (to make 'eret' working right)
  // msr ELR_hyp, r0       // save the address in ELR_hyp
  // eret                  // apply the mode change (Exception return)

  // Invalidate all the caches
  // See https://developer.arm.com/documentation/ddi0360/e/control-coprocessor-cp15/register-descriptions/c7--cache-operations-register?lang=en
  asm volatile("mcr p15, 0, r2, c7, c7, 0;");
  // Set secure state
  // mrc p15, 0, r1, c1, c1, 0   // Get SCR  (Secure Configuration Register)
  // bic r1, r1, #1              // Reset 'Non-secure' bit (set secure state)
  // mcr p15, 0, r1, c1, c1, 0   // Write to SCR

  // Turn on caches and branch prediction!
  // See https://developer.arm.com/documentation/ddi0360/e/control-coprocessor-cp15/register-descriptions/c1--control-register?lang=en
  uint32_t ctrl = read_c1_ctrl_reg();
  ctrl |= C1_CTRL_I   // Enable L1 Instruction cache
       |  C1_CTRL_Z   // Enable Branch prediction
       |  C1_CTRL_C;  // Enable L1 Data cache
  write_c1_ctrl_reg(ctrl);
}

void arm1176jzfs_init(uint32_t r0, uint32_t machine_id, uint32_t atags){
  (void)machine_id;
  cpuinit();
  // Setup early console
  extern void bcm_miniuart_init(); bcm_miniuart_init();
  if (r0){
    KPanic("[PANIC] Corrupted Boot Info\r\n");
  }
  if (atags == 0){
    atags = 0x100; // The default address
  }

  KInfo("CPU State:\r\n");
  KInfo("\tC0, Main ID: 0x%x, Cache Type: 0x%x, TLB Type: 0x%x, CPU ID: 0x%x\r\n",
    read_c0_main_id_reg(), read_c0_cache_type_reg(), read_c0_tlb_type_reg(),
    read_c0_cpu_id_reg());
  KInfo("\tC1, Control Register: 0x%x\r\n", read_c1_ctrl_reg());
  KInfo("\tC2, TTBCR: 0x%x, TTBR0: 0x%x, TTBR1: 0x%x\r\n", read_c2_ttbcr(),
    read_c2_ttbr0(), read_c2_ttbr1());
  KInfo("\tC3, DACR: 0x%x\r\n", read_c3_dacr());
  
  extern kparams_t* atag_to_kparams(uint32_t);
  kparams_t* params = atag_to_kparams(atags);
  params->platform_string = "ARM1176JZF-S";

  // Copy the exception vector table to 0x0
  extern uint32_t _start;
  int i;
  uint32_t* src = (uint32_t*)&_start;
  uint32_t* dst = (uint32_t*)0;
  for (i = 0; i < 16; i++)
    dst[i] = src[i];
  
	qKernelStart(params, 0);
}

