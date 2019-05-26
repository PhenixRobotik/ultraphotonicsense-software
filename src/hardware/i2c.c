#include "i2c.h"

#include "rcc.h"
#include "stm32f3xx_hal.h"

#define I2C1_SCL_ToF_Pin GPIO_PIN_15
#define I2C1_SCL_ToF_GPIO_Port GPIOA
#define I2C1_SDA_ToF_Pin GPIO_PIN_7
#define I2C1_SDA_ToF_GPIO_Port GPIOB

GPIO_InitTypeDef GPIO_InitStruct;
I2C_HandleTypeDef hi2c1;
int i2c_setup() {
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();
  /**I2C1 GPIO Configuration    
     PA15     ------> I2C1_SCL
     PB7     ------> I2C1_SDA 
  */
  GPIO_InitStruct.Pin = I2C1_SCL_ToF_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_AF_OD;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
  GPIO_InitStruct.Alternate = GPIO_AF4_I2C1;
  HAL_GPIO_Init(I2C1_SCL_ToF_GPIO_Port, &GPIO_InitStruct);

  GPIO_InitStruct.Pin = I2C1_SDA_ToF_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_AF_OD;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
  GPIO_InitStruct.Alternate = GPIO_AF4_I2C1;
  HAL_GPIO_Init(I2C1_SDA_ToF_GPIO_Port, &GPIO_InitStruct);

  /* I2C1 clock enable */
  __HAL_RCC_I2C1_CLK_ENABLE();
  
  hi2c1.Instance = I2C1;
  hi2c1.Init.Timing = 0x0000020B;
  hi2c1.Init.OwnAddress1 = 0;
  hi2c1.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
  hi2c1.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
  hi2c1.Init.OwnAddress2 = 0;
  hi2c1.Init.OwnAddress2Masks = I2C_OA2_NOMASK;
  hi2c1.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
  hi2c1.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
  if (HAL_I2C_Init(&hi2c1) != HAL_OK)
  {
    return -1;
  }
  /** Configure Analogue filter 
  */
  if (HAL_I2CEx_ConfigAnalogFilter(&hi2c1, I2C_ANALOGFILTER_ENABLE) != HAL_OK)
  {
    return -1;
  }
  /** Configure Digital filter 
  */
  if (HAL_I2CEx_ConfigDigitalFilter(&hi2c1, 0) != HAL_OK)
  {
    return -1;
  }

  return 0;
}

#if 0
#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/i2c.h>
#include <libopencm3/stm32/gpio.h>

#define GPIO_I2C_SDA_PORT GPIOB
#define GPIO_I2C_SDA_PIN  GPIO7
#define GPIO_I2C_SCL_PORT GPIOA
#define GPIO_I2C_SCL_PIN  GPIO15
#define GPIO_I2C_AF       GPIO_AF4

void i2c_setup_libopencm3(){
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
#endif

