#include "gpio.h"

void gpio_setup() {

  /* Status LED */
  gpio_set_output_options(GPIO_LED_PORT, GPIO_OTYPE_PP,
                          GPIO_OSPEED_2MHZ, GPIO_LED_PIN);
  gpio_mode_setup(GPIO_LED_PORT, GPIO_MODE_OUTPUT,
                  GPIO_PUPD_NONE, GPIO_LED_PIN);

  /* bxCAN */
  // CAN RX
  gpio_set_af(GPIO_CAN_PORT, GPIO_CAN_AF, GPIO_CAN_RX_PIN);
  gpio_mode_setup(GPIO_CAN_PORT, GPIO_MODE_AF,
                  GPIO_PUPD_PULLUP, GPIO_CAN_RX_PIN);
  
  // CAN TX
  gpio_set_af(GPIO_CAN_PORT, GPIO_CAN_AF, GPIO_CAN_TX_PIN);
  gpio_set_output_options(GPIO_CAN_PORT, GPIO_OTYPE_PP,
                          GPIO_OSPEED_50MHZ, GPIO_CAN_TX_PIN);
  gpio_mode_setup(GPIO_CAN_PORT, GPIO_MODE_AF,
                  GPIO_PUPD_NONE, GPIO_CAN_TX_PIN);  
  
  /* I2C */
  gpio_set_af(GPIO_I2C1_SCL_PORT, GPIO_I2C_AF, GPIO_I2C1_SCL_PIN);
  gpio_set_af(GPIO_I2C1_SDA_PORT, GPIO_I2C_AF, GPIO_I2C1_SDA_PIN);
	gpio_set_output_options(GPIO_I2C1_SCL_PORT, GPIO_OTYPE_OD,
                          GPIO_OSPEED_100MHZ,	GPIO_I2C1_SCL_PIN);
  gpio_set_output_options(GPIO_I2C1_SDA_PORT, GPIO_OTYPE_OD,
                          GPIO_OSPEED_100MHZ,	GPIO_I2C1_SDA_PIN);
	gpio_mode_setup(GPIO_I2C1_SCL_PORT, GPIO_MODE_AF,
                  GPIO_PUPD_PULLUP, GPIO_I2C1_SCL_PIN);
  gpio_mode_setup(GPIO_I2C1_SDA_PORT, GPIO_MODE_AF,
                  GPIO_PUPD_PULLUP, GPIO_I2C1_SDA_PIN);
  
  // I2C enable
  gpio_set_output_options(GPIO_I2CEN_PORT, GPIO_OTYPE_PP,
                          GPIO_OSPEED_2MHZ, GPIO_I2CEN_PIN);
  gpio_mode_setup(GPIO_I2CEN_PORT, GPIO_MODE_OUTPUT,
                  GPIO_PUPD_PULLUP, GPIO_I2CEN_PIN);
  
  /* Shift register */
  gpio_set_output_options(GPIO_TOF_RST_PORT, GPIO_OTYPE_PP, GPIO_OSPEED_2MHZ,
                          GPIO_TOF_RST_DATA_PIN | GPIO_TOF_RST_SHIFT_PIN);
  gpio_mode_setup(GPIO_TOF_RST_PORT, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE,
                  GPIO_TOF_RST_DATA_PIN | GPIO_TOF_RST_SHIFT_PIN);

  /* UART1 */
  gpio_set_af(GPIO_UART1_PORT, GPIO_UART1_AF,
              GPIO_UART1_RX_PIN | GPIO_UART1_TX_PIN);
  gpio_set_output_options(GPIO_UART1_PORT, GPIO_OTYPE_PP,
                          GPIO_OSPEED_100MHZ, GPIO_UART1_TX_PIN);
  gpio_mode_setup(GPIO_UART1_PORT, GPIO_MODE_AF, GPIO_PUPD_PULLUP,
                  GPIO_UART1_RX_PIN | GPIO_UART1_TX_PIN);
  
  /* Ultrasonic sensors */
  // Output enable
  gpio_set_output_options(GPIO_US_OE_PORT, GPIO_OTYPE_PP,
                          GPIO_OSPEED_2MHZ, GPIO_US_OE_PIN);
  gpio_mode_setup(GPIO_US_OE_PORT, GPIO_MODE_OUTPUT,
                  GPIO_PUPD_NONE, GPIO_US_OE_PIN);
  // Trigger pins
  gpio_set_output_options(GPIO_US_PORT, GPIO_OTYPE_PP,
                          GPIO_OSPEED_2MHZ, GPIO_US_TRIG_PINS);
  gpio_mode_setup(GPIO_US_PORT, GPIO_MODE_OUTPUT,
                  GPIO_PUPD_NONE, GPIO_US_TRIG_PINS);
  // Echo pins
  gpio_mode_setup(GPIO_US_PORT, GPIO_MODE_INPUT,
                  GPIO_PUPD_PULLDOWN, GPIO_US_ECHO_PINS);
  
  // Lock configuration
  gpio_port_config_lock(GPIOA, 0xFFFF);
  gpio_port_config_lock(GPIOB, 0xFFFF);
}

void led_on() {
  gpio_set(GPIO_LED_PORT, GPIO_LED_PIN);
}

void led_off() {
  gpio_clear(GPIO_LED_PORT, GPIO_LED_PIN);
}

void led_toggle_status() {
  gpio_toggle(GPIO_LED_PORT, GPIO_LED_PIN);
}
