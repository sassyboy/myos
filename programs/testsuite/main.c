#include <kernel.h>
#include <modules/gpio/bcm.h>

__attribute__((section(".text.main")))
int main() {
  KInfo("HELLO WORLD!\r\n");
  KInfo("Testing the System Timer with a 25ms delay\r\n");

  // System Timer Test
  timestamp_t start, end;
  start = ReadTimeStamp();
  DelayMicro(25000);
  end = ReadTimeStamp();
  KInfo("Start TimeStamp: 0x%x\r\n", (uint32_t)start);
  KInfo("End TimeStamp: 0x%x\r\n", (uint32_t)end);
  KInfo("Duration: %d uS\r\n", (uint32_t)end - (uint32_t)start);

  // GPIO Test
  KInfo("Testing the GPIO on Pi0. Pins 36, 38 and 40 should turn on\r\n");
  SetGpioDirection(16, GPIO_OUT);
  SetGpioDirection(20, GPIO_OUT);
  SetGpioDirection(21, GPIO_OUT);
  SetGpio(16, 1);
  SetGpio(20, 1);
  SetGpio(21, 1);

  KInfo("Bye!\r\n");
  return 0;
}