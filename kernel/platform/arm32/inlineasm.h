#ifndef PLAT_ARM32_INLINEASM_H
#define PLAT_ARM32_INLINEASM_H
#include <stdint.h>

/**
 * @file inlineasm.h wraps frequently used inline assembly instructions
 * inside more readable C functions.
 * I hate to see cryptic assembly scattered in my clean C code :D
 */

/**
 * Access methods for the CP15 Registers
 */
static inline uint32_t read_c0_main_id_reg(){
  uint32_t val;
  asm volatile("mrc p15,0,%0,c0,c0,0;" : "=r"(val));
  return val;
}

static inline uint32_t read_c0_cache_type_reg(){
  uint32_t val;
  asm volatile("mrc p15,0,%0,c0,c0,1;" : "=r"(val));
  return val;
}

static inline uint32_t read_c0_tlb_type_reg(){
  uint32_t val;
  asm volatile("mrc p15,0,%0,c0,c0,3;" : "=r"(val));
  return val;
}

static inline uint32_t read_c0_cpu_id_reg(){
  uint32_t val;
  asm volatile("mrc p15,0,%0,c0,c0,5;" : "=r"(val));
  return val;
}


/**
 * Access methods for the CP15, C1 - Control Register
 * C1_CTRL_XP:1 = subpage AP bits disabled.
 * C1_CTRL_U: Enables unaligned data access operation.
 * C1_CTRL_V: Location of exception vectors:
 *          0 = address range = 0x00000000-0x0000001C (normal exception vectors)
 *          1 = address range = 0xFFFF0000-0xFFFF001C (high exception vectors)
 * C1_CTRL_I: 1 = instruction cache enabled.
 * C1_CTRL_Z: 1 = program flow prediction enabled.
 * C1_CTRL_R: ROM protection. (Deprecated).
 * C1_CTRL_S: System protection. (Deprecated).
 * C1_CTRL_C: 1 = data cache enabled.
 * C1_CTRL_A: 1 = strict alignment fault checking enabled.
 * C1_CTRL_M: 1 = MMU enabled.
 */
#define C1_CTRL_XP    (1ul<<23)
#define C1_CTRL_U     (1ul<<22)
#define C1_CTRL_V     (1ul<<13)
#define C1_CTRL_I     (1ul<<12)
#define C1_CTRL_Z     (1ul<<11)
#define C1_CTRL_R     (1ul<<09)
#define C1_CTRL_S     (1ul<<08)
#define C1_CTRL_C     (1ul<<02)
#define C1_CTRL_A     (1ul<<01)
#define C1_CTRL_M     (1ul<<00)

static inline uint32_t read_c1_ctrl_reg(){
  uint32_t val;
  asm volatile("mrc p15,0,%0,c1,c0,0;" : "=r"(val));
  return val;
}

static inline void write_c1_ctrl_reg(uint32_t val){
  asm volatile("mcr p15,0,%0,c1,c0,0;" : : "r"(val));
}

/**
 * Access methods for the CP15, C2 Registers:
 * Translation Table Base Register 0 (Process Specific Translation Tables)
 * Translation Table Base Register 1 (Global Translation Tables)
 * Translation Table Base Control Register
 */
static inline uint32_t read_c2_ttbr0(){
  uint32_t val;
  asm volatile("mrc p15, 0, %0, c2, c0, 0;" : "=r"(val));
  return val;
}
static inline void write_c2_ttbr0(uint32_t val){
  asm volatile("mcr p15, 0, %0, c2, c0, 0;" : : "r"(val));
}

static inline uint32_t read_c2_ttbr1(){
  uint32_t val;
  asm volatile("mrc p15, 0, %0, c2, c0, 1;" : "=r"(val));
  return val;
}
static inline void write_c2_ttbr1(uint32_t val){
  asm volatile("mcr p15, 0, %0, c2, c0, 1;" : : "r"(val));
}

static inline uint32_t read_c2_ttbcr(){
  uint32_t val;
  asm volatile("mrc p15, 0, %0, c2, c0, 2;" : "=r"(val));
  return val;
}
static inline void write_c2_ttbcr(uint32_t val){
  asm volatile("mcr p15, 0, %0, c2, c0, 2;" : : "r"(val));
}

/**
 * Access methods for the CP15, C3 Domain Access Control Register
 */
#define DOMAC_NO_ACCESS   0
#define DOMAC_CLIENT      1
#define DOMAC_MANAGER     3
static inline uint32_t read_c3_dacr(){
  uint32_t val;
  asm volatile("mrc p15, 0, %0, c3, c0, 0;" : "=r"(val));
  return val;
}
static inline void write_c3_dacr(uint32_t val){
  asm volatile("mcr p15, 0, %0, c3, c0, 0;" : : "r"(val));
}
#endif