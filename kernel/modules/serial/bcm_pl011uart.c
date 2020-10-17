#include <config.h>
#include <stddef.h>
#include <stdint.h>
#include <util/stdio.h>

#if defined(PLAT_BCM2835) || defined(PLAT_BCM2836) || \
		defined(PLAT_BCM2837B0) || defined(PLAT_BCM2711)
/**
 * PL011 UART Driver for Broadcom SoCs
 * 
 * Raspberry Pi Devices have multiple UART devices.
 * 
 * Model               | first PL011 (UART0)   | mini UART
 * --------------------|-----------------------|------------
 * Raspberry Pi Zero   | primary               | secondary
 * Raspberry Pi Zero W | secondary (Bluetooth) | primary
 * Raspberry Pi 1      | primary               | secondary
 * Raspberry Pi 2      | primary               | secondary
 * Raspberry Pi 3      | secondary (Bluetooth) | primary
 * Raspberry Pi 4      | secondary (Bluetooth) | primary
 * 
 * This driver will be the primary UART for Pi Zero, Pi 1 and Pi 2
 * 
 **/


// Memory-Mapped I/O output
static inline void mmio_write32(size_t reg, uint32_t data){
	*(volatile uint32_t*)reg = data;
}
 
// Memory-Mapped I/O input
static inline uint32_t mmio_read32(size_t reg){
	return *(volatile uint32_t*)reg;
}
 
// Loop <delay> times in a way that the compiler won't optimize away
static inline void delay(int32_t count)
{
	asm volatile("__delay_%=: subs %[count], %[count], #1; bne __delay_%=\n"
		 : "=r"(count): [count]"0"(count) : "cc");
}

#if defined(PLAT_BCM2835)
  //Pi 0 and Pi 1
  #define BCM_PERIPH_BASE     (0x20000000)

#elif defined(PLAT_BCM2836)
  // Pi 2
  #define BCM_PERIPH_BASE     (0x3F000000)

#elif defined(PLAT_BCM2837)
  // Newer Pi 2 and Pi 3
  #define BCM_PERIPH_BASE     (0x3F000000)

#elif defined(PLAT_BCM2837B0)
  // Pi 3B+ and 3A+
  #define BCM_PERIPH_BASE     (0x3F000000)

#elif defined(PLAT_BCM2711)
  // Pi 4B
  #define BCM_PERIPH_BASE     (0xFE000000) 
#endif

// GPIO Registers
#define BCM_GPIO_BASE       (BCM_PERIPH_BASE + 0x200000)
#define GPFSEL1             (BCM_GPIO_BASE + 0x04)
#define GPSET0              (BCM_GPIO_BASE + 0x1C)
#define GPCLR0              (BCM_GPIO_BASE + 0x28)
#define GPPUD               (BCM_GPIO_BASE + 0x94)
#define GPPUDCLK0           (BCM_GPIO_BASE + 0x98)
  
// The base address for UART.
#define UART0_BASE          (BCM_GPIO_BASE + 0x1000)
#define UART0_DR            (UART0_BASE + 0x00)
#define UART0_RSRECR        (UART0_BASE + 0x04)
#define UART0_FR            (UART0_BASE + 0x18)
#define UART0_ILPR          (UART0_BASE + 0x20)
#define UART0_IBRD          (UART0_BASE + 0x24)
#define UART0_FBRD          (UART0_BASE + 0x28)
#define UART0_LCRH          (UART0_BASE + 0x2C)
#define UART0_CR            (UART0_BASE + 0x30)
#define UART0_IFLS          (UART0_BASE + 0x34)
#define UART0_IMSC          (UART0_BASE + 0x38)
#define UART0_RIS           (UART0_BASE + 0x3C)
#define UART0_MIS           (UART0_BASE + 0x40)
#define UART0_ICR           (UART0_BASE + 0x44)
#define UART0_DMACR         (UART0_BASE + 0x48)
#define UART0_ITCR          (UART0_BASE + 0x80)
#define UART0_ITIP          (UART0_BASE + 0x84)
#define UART0_ITOP          (UART0_BASE + 0x88)
#define UART0_TDR           (UART0_BASE + 0x8C)
 
// The offsets for Mailbox registers
#define MBOX_BASE           (BCM_PERIPH_BASE + 0xB880)
#define MBOX_READ           (MBOX_BASE + 0x00)
#define MBOX_STATUS         (MBOX_BASE + 0x18)
#define MBOX_WRITE          (MBOX_BASE + 0x20)

// A Mailbox message with set clock rate of PL011 to 3MHz tag
volatile unsigned int  __attribute__((aligned(16))) mbox[9] = {
    9*4, 0, 0x38002, 12, 8, 2, 3000000, 0 ,0
};
 
void putc(int c){
	// Wait for UART to become ready to transmit.
	while (mmio_read32(UART0_FR) & (1 << 5) ) { }
	mmio_write32(UART0_DR, c);
}
 
static int getc(){
  // Wait for UART to have received something.
  while (mmio_read32(UART0_FR) & (1 << 4)) { }
  return mmio_read32(UART0_DR);
}
 
static void puts(char* str) {
  int i;
	for (i = 0; str[i] != '\0'; i++)
		putc((int)str[i]);
}

static stdio_ops_t ops = {
  .putc = putc,
  .puts = puts
};

void bcm_pl011uart_init(){
	// Disable UART0.
	mmio_write32(UART0_CR, 0x00000000);
	// Setup the GPIO pin 14 && 15.
 
	// Disable pull up/down for all GPIO pins & delay for 150 cycles.
	mmio_write32(GPPUD, 0x00000000);
	delay(150);
 
	// Disable pull up/down for pin 14,15 & delay for 150 cycles.
	mmio_write32(GPPUDCLK0, (1 << 14) | (1 << 15));
	delay(150);
 
	// Write 0 to GPPUDCLK0 to make it take effect.
	mmio_write32(GPPUDCLK0, 0x00000000);
 
	// Clear pending interrupts.
	mmio_write32(UART0_ICR, 0x7FF);
 
	// Set integer & fractional part of baud rate.
	// Divider = UART_CLOCK/(16 * Baud)
	// Fraction part register = (Fractional part * 64) + 0.5
	// Baud = 115200.
 
	// TODO: FIX THIS
  // For Raspi3 and 4 the UART_CLOCK is system-clock dependent by default.
	// Set it to 3Mhz so that we can consistently set the baud rate
	// if (raspi >= 3) {
	// 	// UART_CLOCK = 30000000;
	// 	unsigned int r = (((unsigned int)(&mbox) & ~0xF) | 8);
	// 	// wait until we can talk to the VC
	// 	while ( mmio_read(MBOX_STATUS) & 0x80000000 ) { }
	// 	// send our message to property channel and wait for the response
	// 	mmio_write(MBOX_WRITE, r);
	// 	while ( (mmio_read(MBOX_STATUS) & 0x40000000) || mmio_read(MBOX_READ) != r ) { }
	// }
 
	// Divider = 3000000 / (16 * 115200) = 1.627 = ~1.
	mmio_write32(UART0_IBRD, 1);
	// Fractional part register = (.627 * 64) + 0.5 = 40.6 = ~40.
	mmio_write32(UART0_FBRD, 40);
 
	// Enable FIFO & 8 bit data transmission (1 stop bit, no parity).
	mmio_write32(UART0_LCRH, (1 << 4) | (1 << 5) | (1 << 6));
 
	// Mask all interrupts.
	mmio_write32(UART0_IMSC, (1 << 1) | (1 << 4) | (1 << 5) | (1 << 6) |
	                       (1 << 7) | (1 << 8) | (1 << 9) | (1 << 10));
 
	// Enable UART0, receive & transfer part of UART.
	mmio_write32(UART0_CR, (1 << 0) | (1 << 8) | (1 << 9));
    
  RegisterStdioInterface(STDIO_DEFAULT, &ops);
  RegisterStdioInterface(STDIO_KERN_INFO, &ops);
  RegisterStdioInterface(STDIO_KERN_WARN, &ops);
  RegisterStdioInterface(STDIO_KERN_ERR, &ops);
}

#endif
