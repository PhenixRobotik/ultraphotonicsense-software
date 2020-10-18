#include "shift_reg.h"

#include "hardware/rcc.h"

static uint8_t shiftreg_val = 0x00;

void shiftreg_shift(bool bit){
  // Set data input
  if(bit){
    gpio_set(SHIFTREG_GPIO_DATA_PORT, SHIFTREG_GPIO_DATA_PIN);
    shiftreg_val = (shiftreg_val << 1) | 1;
  }else{
    gpio_clear(SHIFTREG_GPIO_DATA_PORT, SHIFTREG_GPIO_DATA_PIN);
    shiftreg_val <<= 1;
  }

  // Generate low-to-high clock transition
  gpio_clear(SHIFTREG_GPIO_SHIFT_PORT, SHIFTREG_GPIO_SHIFT_PIN);
  delay_us(1);
  gpio_set(SHIFTREG_GPIO_SHIFT_PORT, SHIFTREG_GPIO_SHIFT_PIN);
  delay_us(1);
}

uint8_t shiftreg_value(){
  return shiftreg_val;
}
