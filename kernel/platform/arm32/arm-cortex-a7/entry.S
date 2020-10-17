// r15 -> should begin execution at 0x8000.
// r0 -> 0x00000000 - We use it for PCPU ID
// r1 -> 0x00000C42 - machine id
// r2 -> 0x00000100 - start of ATAGS
// preserve these registers as argument for kernel_main

.section ".text.boot"
.globl _start
_start:
	// Read the CPU ID into r0
	mrc p15, 0, r0, c0, c0, 5
	and r0, r0, #3
	
	// Setup the initial stack per cpu (2KB each)
	ldr r5, =_start
	mov r6, #2048
	mul r6, r0, r6
	sub r5, r5, r6
	mov sp, r5

	// Clear out bss (CPU 0 only)
	cmp r0, #0
	bne 3f

	ldr r4, =_kernel_bss_start
	ldr r9, =_kernel_bss_end
	mov r5, #0
	mov r6, #0
	mov r7, #0
	mov r8, #0
	b       2f
 
1:
	// store multiple at r4.
	stmia r4!, {r5-r8}
 
	// If we are still below bss_end, loop.
2:
	cmp r4, r9
	blo 1b
 
3:
	// Call into C
	ldr r3, =arm_cortex_a7_init
	blx r3
 
// halt
halt:
	wfe
	b halt 

