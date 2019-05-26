#include "i2c.h"

#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/i2c.h>
#include <libopencm3/stm32/gpio.h>

#include "rcc.h"

// I2C
#define GPIO_I2C_SDA_PORT GPIOB
#define GPIO_I2C_SDA_PIN  GPIO7
#define GPIO_I2C_SCL_PORT GPIOA
#define GPIO_I2C_SCL_PIN  GPIO15
#define GPIO_I2C_AF       GPIO_AF4



void i2c_setup(){
  // Enable clocks
  rcc_periph_clock_enable(RCC_I2C1);
  rcc_periph_clock_enable(RCC_GPIOA);
  rcc_periph_clock_enable(RCC_GPIOB);

  rcc_set_i2c_clock_sysclk(I2C1);

  // I2C
  gpio_mode_setup(GPIO_I2C_SDA_PORT,
    GPIO_MODE_AF,
    GPIO_PUPD_PULLUP,
    GPIO_I2C_SDA_PIN
  );
  gpio_set_af(GPIO_I2C_SDA_PORT, GPIO_I2C_AF, GPIO_I2C_SDA_PIN);

  gpio_mode_setup(GPIO_I2C_SCL_PORT,
    GPIO_MODE_AF,
    GPIO_PUPD_PULLUP,
    GPIO_I2C_SCL_PIN
  );
  gpio_set_af(GPIO_I2C_SCL_PORT, GPIO_I2C_AF, GPIO_I2C_SCL_PIN);

  gpio_set_output_options(
    GPIO_I2C_SDA_PORT,
    GPIO_OTYPE_OD,
    GPIO_OSPEED_50MHZ,
    GPIO_I2C_SDA_PIN
  );
  gpio_set_output_options(
    GPIO_I2C_SCL_PORT,
    GPIO_OTYPE_OD,
    GPIO_OSPEED_50MHZ,
    GPIO_I2C_SCL_PIN
  );

  // Reset
  i2c_reset(I2C1);
  i2c_peripheral_disable(I2C1);

  // Configuration
  i2c_enable_analog_filter(I2C1);
  i2c_set_digital_filter(I2C1, 0); // no filter
  i2c_set_speed(I2C1, i2c_speed_fm_400k, 64); // 400kHz mode
  i2c_enable_stretching(I2C1);
  i2c_set_7bit_addr_mode(I2C1);

  // Enable I2C
  i2c_peripheral_enable(I2C1);
}
