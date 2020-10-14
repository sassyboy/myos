
/* SPDX-License-Identifier: BSD-2-Clause */
/*
 * Copyright (c) 2015, Linaro Limited
 */

.syntax unified
.text
.globl ret_idivmod_values
ret_idivmod_values:
  bx lr

.globl ret_uidivmod_values
ret_uidivmod_values:
  bx lr


/*
 * __value_in_regs lldiv_t __aeabi_ldivmod( long long n, long long d)
 */
 .globl __aeabi_ldivmod
__aeabi_ldivmod:
	push	{ip, lr}
	push	{r0-r3}
	mov	r0, sp
	bl	__l_divmod
	pop	{r0-r3}
	pop	{ip, pc}


/*
 * __value_in_regs ulldiv_t __aeabi_uldivmod(
 *		unsigned long long n, unsigned long long d)
 */
.globl __aeabi_uldivmod
__aeabi_uldivmod:
	push	{ip, lr}
	push	{r0-r3}
	mov	r0, sp
	bl	__ul_divmod
	pop	{r0-r3}
	pop	{ip, pc}
