#include <config.h>
#include <stddef.h>
#include <stdint.h>
#include <util/stdio.h>

#if defined(PLAT_BCM2835) || defined(PLAT_BCM2836)

// Memory-Mapped I/O output
static inline void mmio_write(uint32_t reg, uint32_t data)
{
	*(volatile uint32_t*)reg = data;
}
 
// Memory-Mapped I/O input
static inline uint32_t mmio_read(uint32_t reg)
{
	return *(volatile uint32_t*)reg;
}
 
// Loop <delay> times in a way that the compiler won't optimize away
static inline void delay(int32_t count)
{
	asm volatile("__delay_%=: subs %[count], %[count], #1; bne __delay_%=\n"
		 : "=r"(count): [count]"0"(count) : "cc");
}

#define GPFSEL1 0x20200004
#define GPSET0  0x2020001C
#define GPCLR0  0x20200028
#define GPPUD       0x20200094
#define GPPUDCLK0   0x20200098

#define AUX_ENABLES     0x20215004
#define AUX_MU_IO_REG   0x20215040
#define AUX_MU_IER_REG  0x20215044
#define AUX_MU_IIR_REG  0x20215048
#define AUX_MU_LCR_REG  0x2021504C
#define AUX_MU_MCR_REG  0x20215050
#define AUX_MU_LSR_REG  0x20215054
#define AUX_MU_MSR_REG  0x20215058
#define AUX_MU_SCRATCH  0x2021505C
#define AUX_MU_CNTL_REG 0x20215060
#define AUX_MU_STAT_REG 0x20215064
#define AUX_MU_BAUD_REG 0x20215068

static void putc(int c) {
    while(1){
        if(mmio_read(AUX_MU_LSR_REG)&0x20) break;
    }
    mmio_write(AUX_MU_IO_REG, c);
}
 
static void puts(char* str) {
    for (size_t i = 0; str[i] != '\0'; i ++)
        putc((int)str[i]);
}

static stdio_ops_t ops = {
    .putc = putc,
    .puts = puts
};

void uart_bcm_init(){
    unsigned int ra;
    mmio_write(AUX_ENABLES,1);
    mmio_write(AUX_MU_IER_REG,0);
    mmio_write(AUX_MU_CNTL_REG,0);
    mmio_write(AUX_MU_LCR_REG,3);
    mmio_write(AUX_MU_MCR_REG,0);
    mmio_write(AUX_MU_IER_REG,0);
    mmio_write(AUX_MU_IIR_REG,0xC6);
    mmio_write(AUX_MU_BAUD_REG,270);

    ra=mmio_read(GPFSEL1);
    ra&=~(7<<12); //gpio14
    ra|=2<<12;    //alt5
    mmio_write(GPFSEL1,ra);

    //should we care?
    //PUT32(GPPUD,0);
    //for(ra=0;ra<150;ra++) dummy(ra);
    //PUT32(GPPUDCLK0,(1<<14));
    //for(ra=0;ra<150;ra++) dummy(ra);
    //PUT32(GPPUDCLK0,0);

    mmio_write(AUX_MU_CNTL_REG,2);
    
    RegisterStdioInterface(STDIO_DEFAULT, &ops);
    RegisterStdioInterface(STDIO_KERN_INFO, &ops);
    RegisterStdioInterface(STDIO_KERN_WARN, &ops);
    RegisterStdioInterface(STDIO_KERN_ERR, &ops);
}

#endif
