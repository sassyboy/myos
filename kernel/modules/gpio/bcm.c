#include <config.h>
#include <modules/gpio/bcm.h>

#if defined(PLAT_BCM2835) || defined(PLAT_BCM2836)

typedef struct {
	unsigned long	GPFSEL[6];	///< Function selection registers.
	unsigned long	Reserved_1;
	unsigned long	GPSET[2];
	unsigned long	Reserved_2;
	unsigned long	GPCLR[2];
	unsigned long	Reserved_3;
	unsigned long	GPLEV[2];
	unsigned long	Reserved_4;
	unsigned long	GPEDS[2];
	unsigned long	Reserved_5;
	unsigned long	GPREN[2];
	unsigned long	Reserved_6;
	unsigned long	GPFEN[2];
	unsigned long	Reserved_7;
	unsigned long	GPHEN[2];
	unsigned long	Reserved_8;
	unsigned long	GPLEN[2];
	unsigned long	Reserved_9;
	unsigned long	GPAREN[2];
	unsigned long	Reserved_A;
	unsigned long	GPAFEN[2];
	unsigned long	Reserved_B;
	unsigned long	GPPUD[1];
	unsigned long	GPPUDCLK[2];
	//Ignoring the reserved and test bytes
} bcm_gpio_regs_t;

#if defined(PLAT_BCM2835)
#define BCM_PERIPH_BASE     (0x20000000)
#elif defined(PLAT_BCM2836)
#define BCM_PERIPH_BASE     (0x3F000000)
#endif

volatile bcm_gpio_regs_t* const pRegs = 
	(bcm_gpio_regs_t *) (BCM_PERIPH_BASE + 0x200000);


void SetGpioFunction(unsigned int pinNum, unsigned int funcNum) {

	int offset = pinNum / 10;

	unsigned long val = pRegs->GPFSEL[offset];	// Read in the original register value.

	int item = pinNum % 10;
	val &= ~(0x7 << (item * 3));
	val |= ((funcNum & 0x7) << (item * 3));
	pRegs->GPFSEL[offset] = val;
}

void SetGpioDirection(unsigned int pinNum, enum GPIO_DIR dir) {
	SetGpioFunction(pinNum,dir);
}

void SetGpio(unsigned int pinNum, unsigned int pinVal) {
	unsigned long offset=pinNum/32;
	unsigned long mask=(1<<(pinNum%32));

	if(pinVal) {
		pRegs->GPSET[offset]|=mask;
	} else {
		pRegs->GPCLR[offset]|=mask;
	}
}

int ReadGpio(unsigned int pinNum) {
	return ((pRegs->GPLEV[pinNum/32])>>(pinNum%32))&1;
}

void EnableGpioDetect(unsigned int pinNum, enum DETECT_TYPE type)
{
// 	unsigned long mask=(1<<pinNum);
// 	unsigned long offset=pinNum/32;
// 	
// 	switch(type) {
// 	case DETECT_RISING:
// 		pRegs->GPREN[offset]|=mask;
// 		break;
// 	case DETECT_FALLING:
// 		pRegs->GPFEN[offset]|=mask;
// 		break;
// 	case DETECT_HIGH:
// 		pRegs->GPHEN[offset]|=mask;
// 		break;
// 	case DETECT_LOW:
// 		pRegs->GPLEN[offset]|=mask;
// 		break;
// 	case DETECT_RISING_ASYNC:
// 		pRegs->GPAREN[offset]|=mask;
// 		break;
// 	case DETECT_FALLING_ASYNC:
// 		pRegs->GPAFEN[offset]|=mask;
// 		break;
// 	case DETECT_NONE:
// 		break;
// 	}
}

void DisableGpioDetect(unsigned int pinNum, enum DETECT_TYPE type)
{
// 	unsigned long mask=~(1<<(pinNum%32));
// 	unsigned long offset=pinNum/32;
// 	
// 	switch(type) {
// 	case DETECT_RISING:
// 		pRegs->GPREN[offset]&=mask;
// 		break;
// 	case DETECT_FALLING:
// 		pRegs->GPFEN[offset]&=mask;
// 		break;
// 	case DETECT_HIGH:
// 		pRegs->GPHEN[offset]&=mask;
// 		break;
// 	case DETECT_LOW:
// 		pRegs->GPLEN[offset]&=mask;
// 		break;
// 	case DETECT_RISING_ASYNC:
// 		pRegs->GPAREN[offset]&=mask;
// 		break;
// 	case DETECT_FALLING_ASYNC:
// 		pRegs->GPAFEN[offset]&=mask;
// 		break;
// 	case DETECT_NONE:
// 		break;
// 	}
}

void ClearGpioInterrupt(unsigned int pinNum)
{
// 	unsigned long mask=(1<<(pinNum%32));
// 	unsigned long offset=pinNum/32;
// 
// 	pRegs->GPEDS[offset]=mask;
}

#endif
