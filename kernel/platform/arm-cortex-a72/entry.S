.section ".text.boot"
 
// Entry point for the kernel. Registers:
// x0 -> 32 bit pointer to DTB in memory (primary core only) / 0 (secondary cores)
// x1 -> 0
// x2 -> 0
// x3 -> 0
// x4 -> 32 bit kernel entry point, _start location
.globl _start
_start:
	mrs	x5, mpidr_el1		
	and	x5, x5,#0xFF		// Check processor id
	cbz	x5, core0			
	b	halt	// Hang for all non-primary CPU
    
core0:
	// set stack before our code
  ldr     x5, =_start
  mov     sp, x5
 
  // clear bss
  ldr     x5, =_kernel_bss_start
  ldr     w6, =_kernel_bss_size
1:cbz     w6, 2f
  str     xzr, [x5], #8
  sub     w6, w6, #1
  cbnz    w6, 1b
 
  // jump to C code, should not return
2: 
	bl      arm_cortex_a72_init
  
halt:
	wfe
  b halt

