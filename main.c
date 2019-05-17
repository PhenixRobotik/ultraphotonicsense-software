#include "lowlevel/clock.h"
#include "lowlevel/gpio.h"
#include "lowlevel/debug.h"


int main() {
  clock_setup();
  gpio_setup();
  debug_setup();

  while (1) {};
  return 0;
}
