.section ".text.boot"

/** 
 * Entry point for the kernel:
 * r15 -> should begin execution at 0x8000.
 * r0 -> 0x00000000
 * r1 -> 0x00000C42 - machine id
 * r2 -> 0x00000100 - start of ATAGS
 */

.globl _start
_start:

/* Exception Vector Table */
ldr	pc, reset_excep_addr
ldr pc, undefined_excep_addr
ldr pc, swint_excep_addr
ldr pc,	prefetch_excep_addr
ldr pc,	data_excep_addr
ldr pc, unused_excep_addr
ldr pc,	irq_excep_addr
ldr pc,	fiq_excep_addr

reset_excep_addr:
	.word reset_handler_stub
undefined_excep_addr:
	.word undef_inst_handler_stub
swint_excep_addr:
	.word syscall_handler_stub
prefetch_excep_addr:
	.word prefetch_abort_stub
data_excep_addr:
	.word data_abort_stub
unused_excep_addr:
	.word unused_handler_stub
irq_excep_addr:
	.word irq_handler_stub
fiq_excep_addr:
	.word fiq_handler_stub

/*********************************/
reset_handler_stub:
	/* Setup the stack for different CPU modes */
	/* IRQ STACK @ 0x8000 */
	mov r4, 		#0xD2
  msr cpsr_c, r4
	mov sp, 		#0x8000
	/* FIQ STACK @ 0x6000 */
  mov r4,			#0xD1
  msr cpsr_c,	r4
  mov sp,			#0x6000
	/* Initial Kernel STACK @ 0x4000 */
  mov r4,			#0xD3
  msr cpsr_c,	r4
  mov sp,			#0x4000
 
	// Clear out bss.
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
 
	// Call arm1176jzfs_init
	ldr r3, =arm1176jzfs_init
	blx r3
 
	// halt
halt:
	wfe
	b halt

/* Exception Handler Code Stubs */
undef_inst_handler_stub:
	b halt

syscall_handler_stub:
	b halt

prefetch_abort_stub:
	b halt

data_abort_stub:
	b halt

irq_handler_stub:
  push {r0,r1,r2,r3,r4,r5,r6,r7,r8,r9,r10,r11,r12,lr}
  bl bcm_intctrl_dispatcher
  pop  {r0,r1,r2,r3,r4,r5,r6,r7,r8,r9,r10,r11,r12,lr}
  subs pc,lr,#4

fiq_handler_stub:
	b halt

unused_handler_stub:
	b halt