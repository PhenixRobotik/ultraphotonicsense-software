#include "hardware.h"

#include "rcc.h"
#include "gpio.h"
#include "can.h"

int hardware_setup(void){
  clock_setup();
  delay_setup();
  gpio_setup();
  if(can_setup() != 0){
    return -1;
  }

  i2c_setup();

  return 0;
}
