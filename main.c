#include "lowlevel/clock.h"
#include "lowlevel/gpio.h"
#include "lowlevel/uart.h"


int main() {
  clock_setup();
  gpio_setup();
  uart_setup();

  while (1) {};
  return 0;
}
