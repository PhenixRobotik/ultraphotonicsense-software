#include "sys_init.h"

#include "hardware/rcc.h"
#include "hardware/gpio.h"
#include "hardware/i2c.h"
#include "hardware/can.h"
#include "hardware/timer.h"

int system_init(void){
  clock_setup();
  gpio_setup();
  timer_setup();
  if(can_setup() != 0){
    return -1;
  }
  i2c_setup();

  return 0;
}
