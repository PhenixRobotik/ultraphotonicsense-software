#include "i2c.h"

#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/i2c.h>

#include "rcc.h"

void i2c_setup(){
  // Enable clocks
  rcc_periph_clock_enable(RCC_I2C1);
  rcc_set_i2c_clock_hsi(I2C1);

  // Reset
  i2c_reset(I2C1);
  i2c_peripheral_disable(I2C1);

  // Configuration
  i2c_enable_analog_filter(I2C1);
  i2c_set_digital_filter(I2C1, 0); // no filter
  i2c_set_speed(I2C1, i2c_speed_sm_100k, HSI_FREQ / 1000000); // 100kHz mode
  i2c_set_7bit_addr_mode(I2C1);

  // Enable I2C
  i2c_peripheral_enable(I2C1);
}
