#include <kernel.h>

__attribute__((section(".text.main")))
int main() {
  KInfo("HELLO WORLD!\r\n");
  return 16;
}