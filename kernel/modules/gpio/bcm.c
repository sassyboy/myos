#include <kernel.h>
#include <modules/gpio/bcm.h>

#if defined(PLAT_BCM2835) || defined(PLAT_BCM2836)|| defined(PLAT_BCM2711)

typedef struct {
	uint32_t	GPFSEL[6];	///< Function selection registers.
	uint32_t	Reserved_1;
	uint32_t	GPSET[2];
	uint32_t	Reserved_2;
	uint32_t	GPCLR[2];
	uint32_t	Reserved_3;
	uint32_t	GPLEV[2];
	uint32_t	Reserved_4;
	uint32_t	GPEDS[2];
	uint32_t	Reserved_5;
	uint32_t	GPREN[2];
	uint32_t	Reserved_6;
	uint32_t	GPFEN[2];
	uint32_t	Reserved_7;
	uint32_t	GPHEN[2];
	uint32_t	Reserved_8;
	uint32_t	GPLEN[2];
	uint32_t	Reserved_9;
	uint32_t	GPAREN[2];
	uint32_t	Reserved_A;
	uint32_t	GPAFEN[2];
	uint32_t	Reserved_B;
	uint32_t	GPPUD[1];
	uint32_t	GPPUDCLK[2];
	//Ignoring the reserved and test bytes
} bcm_gpio_regs_t;

#if defined(PLAT_BCM2835)
#define BCM_PERIPH_BASE     (0x20000000)
#elif defined(PLAT_BCM2836)
#define BCM_PERIPH_BASE     (0x3F000000)
#elif defined(PLAT_BCM2711)
  // RPi4
	#define BCM_PERIPH_BASE		(0xFE000000) 
#endif

volatile bcm_gpio_regs_t* const pRegs = 
	(bcm_gpio_regs_t *) (BCM_PERIPH_BASE + 0x200000);


void SetGpioFunction(unsigned int pinNum, unsigned int funcNum) {

	int offset = pinNum / 10;

	uint32_t val = pRegs->GPFSEL[offset];	// Read in the original register value.

	int item = pinNum % 10;
	val &= ~(0x7 << (item * 3));
	val |= ((funcNum & 0x7) << (item * 3));
	pRegs->GPFSEL[offset] = val;
}

void SetGpioDirection(unsigned int pinNum, enum GPIO_DIR dir) {
	SetGpioFunction(pinNum,dir);
}

void SetGpio(unsigned int pinNum, unsigned int pinVal) {
	uint32_t offset=pinNum/32;
	uint32_t mask=(1<<(pinNum%32));

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
// 	uint32_t mask=(1<<pinNum);
// 	uint32_t offset=pinNum/32;
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
// 	uint32_t mask=~(1<<(pinNum%32));
// 	uint32_t offset=pinNum/32;
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
// 	uint32_t mask=(1<<(pinNum%32));
// 	uint32_t offset=pinNum/32;
// 
// 	pRegs->GPEDS[offset]=mask;
}

#endif
