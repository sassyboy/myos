#include <config.h>
#include <stddef.h>
#include <stdint.h>
#include <util/stdio.h>

#if defined(PLAT_BCM2835) || defined(PLAT_BCM2836)

static inline void mmio_write(uint32_t reg, uint32_t data){
	*(volatile uint32_t*)reg = data;
}
static inline uint32_t mmio_read(uint32_t reg){
	return *(volatile uint32_t*)reg;
}

#define BCMSYSTIMER_BASE  (0x20003000)
#define BCMSYSTIMER_CS    (BCMSYSTIMER_BASE + 0x0)
#define BCMSYSTIMER_CLO   (BCMSYSTIMER_BASE + 0x4)
#define BCMSYSTIMER_CHI   (BCMSYSTIMER_BASE + 0x8)
#define BCMSYSTIMER_C0    (BCMSYSTIMER_BASE + 0xC)
#define BCMSYSTIMER_C1    (BCMSYSTIMER_BASE + 0x10)
#define BCMSYSTIMER_C2    (BCMSYSTIMER_BASE + 0x14)
#define BCMSYSTIMER_C3    (BCMSYSTIMER_BASE + 0x18)

uint64_t systimer_bcm_read_tsc(){
  uint64_t val = 0;
  val = mmio_read(BCMSYSTIMER_CHI);
  val <<= 32;
  val |= mmio_read(BCMSYSTIMER_CLO);
  return val;
}

void systimer_bcm_init(){

}

#endif