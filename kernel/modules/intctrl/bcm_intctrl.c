#include <kernel.h>

/**
 * Interrupt Controller Driver for Broadcom SoCs
 **/
#if defined(PLAT_BCM2835) || defined(PLAT_BCM2836)

#if defined(PLAT_BCM2835)
#define BCM_PERIPH_BASE     (0x20000000)
#elif defined(PLAT_BCM2836)
#define BCM_PERIPH_BASE     (0x3F000000)
#endif

#define BCM_INTC_TOTAL_IRQ		(64 + 8)

#define BCM_INTC_BASE         (BCM_PERIPH_BASE + 0xB000)
#define BCM_INTC_REGFILE_OFF  (BCM_INTC_BASE + 0x200)

typedef struct {
	uint32_t	pending_basic;
	uint32_t	pending1;
	uint32_t	pending2;
	uint32_t	fiq_ctrl;
	uint32_t	enable1;
	uint32_t	enable2;
	uint32_t	enable_basic;
	uint32_t	disable1;
	uint32_t	disable2;
	uint32_t	disable_basic;
} bcm_intc_regfile_t;


static volatile bcm_intc_regfile_t * const regs = 
  (bcm_intc_regfile_t *)BCM_INTC_REGFILE_OFF;

// IRQ Numbers:
// #define BCM2835_IRQ_SYSTIMER_CM1	1
// #define BCM2835_IRQ_SYSTIMER_CM3	3
// #define BCM2835_IRQ_ID_AUX		    29
// #define BCM2835_IRQ_ID_SPI_SLAVE 	43
// #define BCM2835_IRQ_ID_PWA0			  45
// #define BCM2835_IRQ_ID_PWA1		   	46
// #define BCM2835_IRQ_ID_SMI 			  48
// #define BCM2835_IRQ_ID_GPIO_0 		49
// #define BCM2835_IRQ_ID_GPIO_1 		50
// #define BCM2835_IRQ_ID_GPIO_2 		51
// #define BCM2835_IRQ_ID_GPIO_3 		52
// #define BCM2835_IRQ_ID_I2C 			  53
// #define BCM2835_IRQ_ID_SPI 			  54
// #define BCM2835_IRQ_ID_PCM 			  55
// #define BCM2835_IRQ_ID_UART 		  57

// #define BCM2835_IRQ_ID_TIMER_0 		64
// #define BCM2835_IRQ_ID_MAILBOX_0	65
// #define BCM2835_IRQ_ID_DOORBELL_0 66
// #define BCM2835_IRQ_ID_DOORBELL_1 67
// #define BCM2835_IRQ_ID_GPU0_HALTED 68

int bcm_intctrl_irq_unmask(int irq){
	/**
   * Writing a 1 to a bit will set the corresponding IRQ enable bit.
   * All other IRQ enable bits are unaffected.
   **/
	uint32_t mask= 1 << (irq % 32);
	if(irq >=0 && irq <=31) {
		regs->enable1 = mask;
	} else
	if(irq >=32 && irq <=63){
		regs->enable2 = mask;
	} else
	if(irq >= 64 && irq <= 71) {	// Basic IRQ enables
		regs->enable_basic = mask;
	} else
		return -1; // Invalid IRQ number
	return 0;
}

int bcm_intctrl_irq_mask(int irq){

	uint32_t mask = 1 << (irq % 32);

	if(irq >=0 && irq <=31) {
		regs->disable1 = mask;
	} else
	if(irq >=32 && irq <=63){
		regs->disable2 = mask;
	} else
	if(irq >= 64 && irq <= 71) {
		regs->disable_basic = mask;
	} else
		return -1; // Invalid IRQ number
	return 0;
}

static irq_handler_t handler[BCM_INTC_TOTAL_IRQ];
void bcm_intctrl_reg_handler(int irqn, irq_handler_t func){
  if (irqn > -1 && irqn < BCM_INTC_TOTAL_IRQ){
    handler[irqn] = func;
  }
}

// count leading zeros
#define clz(a) \
 ({ unsigned long __value, __arg = (a); \
     asm ("clz\t%0, %1": "=r" (__value): "r" (__arg)); \
     __value; })

void bcm_intctrl_dispatcher(){
  // Check which interrupt happend by looking at the pending registers
  // pending_basic:
  //  - Bits 7 through 0 represent interrupts 64-71
  //  - Bit 8 indicates interrupts in pending1 (interrupts 31-0)
  //  - Bit 9 indicates interrupts in Pending2 (interrupts 63-32)
  //  - Bits 10 through 20 represent GPU IRQs 7 (We don't care now)
	register uint32_t mask;
	register uint32_t irqn;
  
	mask = regs->pending_basic;
	if (mask & 0xFF) {
		irqn= 64 + 31;
	} else if(mask & 0x100) {
		mask = regs->pending1;
		irqn = 0 + 31;
	}	else if(mask & 0x200) {
		mask = regs->pending2;
		irqn = 32 + 31;
	}	else {
		// False Alarm! No interrupt :D
		return;
	}
	/* Keep only least significant bit, in case multiple interrupts have occured */
	mask &= -mask;
	/* Some magic to determine number of interrupt to serve */
	irqn = irqn - clz(mask);
	/* Call interrupt handler */
  KInfo("INT %d!\r\n", irqn);
  while(1){}
	handler[irqn](irqn);
}

void bcm_intctrl_init(){
  // Make sure all handlers are NULL
  int i;
  for (i = 0 ; i < BCM_INTC_TOTAL_IRQ ; i++){
    handler[i] = NULL;
  }
}

#endif