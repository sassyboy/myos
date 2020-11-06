
 
/* Entry point for the kernel. Registers:
 * x0 -> 32 bit pointer to DTB in memory (primary core only) / 0 (secondary cores)
 * x1 -> Reserved but we use it for the ID of the calling CPU
 * x2 -> Reserved but we use it for the stack bottom of the calling CPU
 * x3 -> Reserved
 * x4 -> 32 bit kernel entry point, _start location
 */

.section ".text.boot"
.globl _start
_start:
  /* Load the PCPU ID into x1*/
	mrs	    x1, mpidr_el1		
	and	    x1, x1, #0xFF

  /* Setup the initial stack for each CPU (8KB each). x2: start of stack */
  ldr     x2, =_start
  mov     x5, #8192
  mul     x5, x5, x1
  sub     x2, x2, x5
  mov     sp, x2
	cbnz	  x1, 2f			
    
  /* BSP only */
  /* clear bss */
  ldr     x5, =_kernel_bss_start
  ldr     w6, =_kernel_bss_size
1:cbz     w6, 2f
  str     xzr, [x5], #8
  sub     w6, w6, #1
  cbnz    w6, 1b
 
  /* All CPUS: jump to C code, should not return */
2: 
	bl      arm_cortex_a72_init
  
halt:
	wfe
  b halt

