#include <kernel.h>
#include <modules/gpio/bcm.h>

void test_cpu_perf1();
void test_cpu_perf2();
void test_softspi();

__attribute__((section(".text.main")))
int main() {
  KInfo("HELLO FROM THE USER PROGRAM!\r\n");
  test_cpu_perf1();
  test_cpu_perf2();
  // GPIO Test
  KInfo("Testing the GPIO on RPi. Pin 40 must flip every 100ms\r\n");
  SetGpioDirection(21, GPIO_OUT);

  test_softspi();
//   while(1){
//     SetGpio(16, 1);
//     SetGpio(20, 1);
//     SetGpio(21, 1);
//     DelayMicro(100000); // 100mS
//     SetGpio(16, 0);
//     SetGpio(20, 0);
//     SetGpio(21, 0);
//     DelayMicro(100000);
//   }
  KInfo("Bye!\r\n");
  return 0;
}

void test_softspi(){
    int last_msclk = 0;
    char inpbuf = 0;
    int inpbuf_bit_count = 0;
    bool in_a_frame = false; // SOF: 155 ('\x9B'), EOF:230 ('\xE6')
    
    // PIN 38 (gpio20): MASTER CLOCK
    const int MSCLK = 20;
    SetGpioDirection(MSCLK, GPIO_IN);
    // PIN 40 (gpio21): MASTER DATA_OUT
    const int MSDAT = 21;
    SetGpioDirection(MSDAT, GPIO_IN);
    
    // Main loop
    while(1){
        int msclk = ReadGpio(MSCLK);
        int msdat = ReadGpio(MSDAT);
        if (msclk != last_msclk){
            inpbuf = (inpbuf<<1) | msdat;
            inpbuf_bit_count++;
            // Prepare for the next bit to arrive
            last_msclk = msclk;
            if (in_a_frame && inpbuf_bit_count == 8 && inpbuf!='\xE6'){
                // normal character
                KInfo("%c",inpbuf);
                inpbuf_bit_count = 0;
            } else if (in_a_frame && inpbuf_bit_count == 8 && inpbuf == '\xE6'){
                // EOF
                inpbuf_bit_count = 0;
                in_a_frame = false;
            } else if (!in_a_frame && inpbuf == '\x9B'){
                // SOF
                in_a_frame = true;
                inpbuf_bit_count = 0;
            }
        }
    }
    
}

void test_cpu_perf1(){
  KInfo("Testing the CPU Performance (1000 branches):\r\n");

  // System Timer Test
  register timestamp_t start, end;
  size_t niter = 1000; 
  size_t ninst = 81 * niter;
  volatile size_t count = 80*niter;
  
  start = ReadTimeStamp();
  // measure the processor freq (pranch-prediction friendly version)

  asm volatile("1:"
                "subs %0, %0, #1;"
                "subs %0, %0, #1;"
                "subs %0, %0, #1;"
                "subs %0, %0, #1;"
                "subs %0, %0, #1;"
                "subs %0, %0, #1;"
                "subs %0, %0, #1;"
                "subs %0, %0, #1;"
                "subs %0, %0, #1;"
                "subs %0, %0, #1;"
                "subs %0, %0, #1;"
                "subs %0, %0, #1;"
                "subs %0, %0, #1;"
                "subs %0, %0, #1;"
                "subs %0, %0, #1;"
                "subs %0, %0, #1;"
                "subs %0, %0, #1;"
                "subs %0, %0, #1;"
                "subs %0, %0, #1;"
                "subs %0, %0, #1;"
                "subs %0, %0, #1;"
                "subs %0, %0, #1;"
                "subs %0, %0, #1;"
                "subs %0, %0, #1;"
                "subs %0, %0, #1;"
                "subs %0, %0, #1;"
                "subs %0, %0, #1;"
                "subs %0, %0, #1;"
                "subs %0, %0, #1;"
                "subs %0, %0, #1;"
                "subs %0, %0, #1;"
                "subs %0, %0, #1;"
                "subs %0, %0, #1;"
                "subs %0, %0, #1;"
                "subs %0, %0, #1;"
                "subs %0, %0, #1;"
                "subs %0, %0, #1;"
                "subs %0, %0, #1;"
                "subs %0, %0, #1;"
                "subs %0, %0, #1;"
                "subs %0, %0, #1;"
                "subs %0, %0, #1;"
                "subs %0, %0, #1;"
                "subs %0, %0, #1;"
                "subs %0, %0, #1;"
                "subs %0, %0, #1;"
                "subs %0, %0, #1;"
                "subs %0, %0, #1;"
                "subs %0, %0, #1;"
                "subs %0, %0, #1;"
                "subs %0, %0, #1;"
                "subs %0, %0, #1;"
                "subs %0, %0, #1;"
                "subs %0, %0, #1;"
                "subs %0, %0, #1;"
                "subs %0, %0, #1;"
                "subs %0, %0, #1;"
                "subs %0, %0, #1;"
                "subs %0, %0, #1;"
                "subs %0, %0, #1;"
                "subs %0, %0, #1;"
                "subs %0, %0, #1;"
                "subs %0, %0, #1;"
                "subs %0, %0, #1;"
                "subs %0, %0, #1;"
                "subs %0, %0, #1;"
                "subs %0, %0, #1;"
                "subs %0, %0, #1;"
                "subs %0, %0, #1;"
                "subs %0, %0, #1;"
                "subs %0, %0, #1;"
                "subs %0, %0, #1;"
                "subs %0, %0, #1;"
                "subs %0, %0, #1;"
                "subs %0, %0, #1;"
                "subs %0, %0, #1;"
                "subs %0, %0, #1;"
                "subs %0, %0, #1;"
                "subs %0, %0, #1;"
                "subs %0, %0, #1;"
                "bne 1b;" : "+r"(count));
  end = ReadTimeStamp();
  KInfo("\tRan %d instructions in %d uS\r\n", ninst, (uint32_t)end - (uint32_t)start);
  KInfo("\tCPU SPEED: %d MHz\r\n", ninst/((uint32_t)end - (uint32_t)start));
}

void test_cpu_perf2(){
  KInfo("Testing the CPU Performance (40500 iterations):\r\n");

  // System Timer Test
  register timestamp_t start, end;
  size_t niter = 40500; 
  size_t ninst = 7 * niter;
  register size_t count = 6*niter;
  
  start = ReadTimeStamp();
  // measure the processor freq (pranch-prediction friendly version)
#ifdef ARCH_ARM64
/** In AARCH64 the following branch instructions are not predicted:
  - AArch32 instructions with the S suffix are not predicted because they are typically used when returning from exceptions and have side effects that can change privilege mode and Security state.
  - All mode or Exception level changing instructions.
  */
  asm volatile("1:"
              "sub %0, %0, #1;"
              "sub %0, %0, #1;"
              "sub %0, %0, #1;"
              "sub %0, %0, #1;"
              "sub %0, %0, #1;"
              "sub %0, %0, #1;"
              "cbnz %0, 1b;" : "+r"(count));
#else // ARM32
  asm volatile("1:"
                "sub %0, %0, #1;"
                "sub %0, %0, #1;"
                "sub %0, %0, #1;"
                "sub %0, %0, #1;"
                "sub %0, %0, #1;"
                "subs %0, %0, #1;"
                "bne 1b;" : "+r"(count));
#endif

  end = ReadTimeStamp();
  KInfo("\tRan %d instructions in %d uS\r\n", ninst, (uint32_t)end - (uint32_t)start);
  KInfo("\tCPU SPEED: %d MHz\r\n", ninst/((uint32_t)end - (uint32_t)start));
}



