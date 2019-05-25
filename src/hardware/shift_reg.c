#include "shift_reg.h"
#include "gpio.h"
#include "rcc.h"

uint8_t shiftreg_val = 0x00;

void shiftreg_shif(bool bit){
  // Set data input
  if(bit){
    gpio_set(SHIFTREG_GPIO_DATA_PORT, SHIFTREG_GPIO_DATA_PIN);
  }else{
    gpio_clear(SHIFTREG_GPIO_DATA_PORT, SHIFTREG_GPIO_DATA_PIN);
  }

  // Generate low-to-high clock transition
  gpio_clear(SHIFTREG_GPIO_SHIFT_PORT, SHIFTREG_GPIO_SHIFT_PIN);
  delay_ms(1); // TODO : use a microseconds delay
  gpio_set(SHIFTREG_GPIO_SHIFT_PORT, SHIFTREG_GPIO_SHIFT_PIN);

  // Store value
  shiftreg_val = (shiftreg_val << 1) | ((bit)?1:0);
}

uint8_t shiftreg_value(){
  return shiftreg_val;
}
