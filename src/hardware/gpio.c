#include "gpio.h"

void gpio_setup() {
  rcc_periph_clock_enable(RCC_GPIOA);
  rcc_periph_clock_enable(RCC_GPIOB);

  // Satus led
  gpio_mode_setup(GPIO_LED_PORT, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, GPIO_LED_PIN);

  // CAN RX
  gpio_mode_setup(GPIO_CAN_PORT, GPIO_MODE_AF, GPIO_PUPD_PULLUP, GPIO_CAN_RX_PIN);
  gpio_set_af(GPIO_CAN_PORT, GPIO_CAN_AF, GPIO11);

  // CAN TX
  gpio_mode_setup(GPIO_CAN_PORT, GPIO_MODE_AF, GPIO_PUPD_NONE, GPIO_CAN_TX_PIN);
  gpio_set_output_options(GPIO_CAN_PORT, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, GPIO_CAN_TX_PIN);
  gpio_set_af(GPIO_CAN_PORT, GPIO_CAN_AF, GPIO_CAN_TX_PIN);
  
  // Shift register
  gpio_mode_setup(GPIO_TOF_RESET_PORT, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, GPIO_TOF_RESET_DATA_PIN);
  gpio_mode_setup(GPIO_TOF_RESET_PORT, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, GPIO_TOF_RESET_SHIFT_PIN);

  // I2C
  gpio_mode_setup(GPIO_I2C_SDA_PORT, GPIO_MODE_AF, GPIO_PUPD_NONE, GPIO_I2C_SDA_PIN);
  gpio_set_af(GPIO_I2C_SDA_PORT, GPIO_I2C_AF, GPIO_I2C_SDA_PIN);
  
  gpio_mode_setup(GPIO_I2C_SCL_PORT, GPIO_MODE_AF, GPIO_PUPD_NONE, GPIO_I2C_SCL_PIN);
  gpio_set_af(GPIO_I2C_SCL_PORT, GPIO_I2C_AF, GPIO_I2C_SCL_PIN);

  // UART
  gpio_mode_setup(GPIO_UART1_PORT, GPIO_MODE_AF, GPIO_PUPD_NONE, GPIO_UART1_RX_PIN);
  gpio_set_af(GPIO_UART1_PORT, GPIO_UART1_AF, GPIO_UART1_RX_PIN);
  
  gpio_mode_setup(GPIO_UART1_PORT, GPIO_MODE_AF, GPIO_PUPD_NONE, GPIO_UART1_TX_PIN);
  gpio_set_af(GPIO_UART1_PORT, GPIO_UART1_AF, GPIO_UART1_TX_PIN);

  // Ultrasonic Sensors
  gpio_mode_setup(GPIO_US_OE_PORT, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, GPIO_US_OE_PIN);
  gpio_mode_setup(GPIO_US_TRIG_0_PORT, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, GPIO_US_TRIG_0_PIN);
  gpio_mode_setup(GPIO_US_ECHO_0_PORT, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, GPIO_US_ECHO_0_PIN);
  gpio_mode_setup(GPIO_US_TRIG_1_PORT, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, GPIO_US_TRIG_1_PIN);
  gpio_mode_setup(GPIO_US_ECHO_1_PORT, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, GPIO_US_ECHO_1_PIN);
  gpio_mode_setup(GPIO_US_TRIG_2_PORT, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, GPIO_US_TRIG_2_PIN);
  gpio_mode_setup(GPIO_US_ECHO_2_PORT, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, GPIO_US_ECHO_2_PIN);
  gpio_mode_setup(GPIO_US_TRIG_3_PORT, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, GPIO_US_TRIG_3_PIN);
  gpio_mode_setup(GPIO_US_ECHO_3_PORT, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, GPIO_US_ECHO_3_PIN);  
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