#include <kernel.h>
#include <modules/gpio/bcm.h>

__attribute__((section(".text.main")))
int main() {
  KInfo("HELLO WORLD!\r\n");
  KInfo("Testing the System Timer\r\n");

  // System Timer Test
  register timestamp_t start, end;
  register uint32_t count = 1000000;
  start = ReadTimeStamp();
  // measure the processor freq
  asm volatile("1: subs %0, %0, #1;"
                "bne 1b;" : "+r"(count));
  end = ReadTimeStamp();
  KInfo("Start TimeStamp: 0x%x\r\n", (uint32_t)start);
  KInfo("End TimeStamp: 0x%x\r\n", (uint32_t)end);
  KInfo("Duration: %d uS\r\n", (uint32_t)end - (uint32_t)start);
  KInfo("CPU SPEED: %d MHz\r\n", 2000000/((uint32_t)end - (uint32_t)start));

  // GPIO Test
  KInfo("Testing the GPIO on RPi. Pin 40 must flip every 100ms\r\n");
  SetGpioDirection(21, GPIO_OUT);

  while(1){
    // SetGpio(16, 1);
    // SetGpio(20, 1);
    SetGpio(21, 1);
    DelayMicro(100000); // 100mS
    // SetGpio(16, 0);
    // SetGpio(20, 0);
    SetGpio(21, 0);
    DelayMicro(100000);
  }
  KInfo("Bye!\r\n");
  return 0;
}