#include "gpio.h"

void gpio_setup() {
  rcc_periph_clock_enable(RCC_GPIOA);

  // Satus led
  gpio_mode_setup(GPIO_LED_PORT, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, GPIO_LED_PIN);

  // CAN RX
  gpio_mode_setup(GPIO_CAN_PORT, GPIO_MODE_AF, GPIO_PUPD_PULLUP, GPIO_CAN_RX_PIN);
  gpio_set_af(GPIO_CAN_PORT, GPIO_AF9, GPIO11);

  // CAN TX
  gpio_mode_setup(GPIO_CAN_PORT, GPIO_MODE_AF, GPIO_PUPD_NONE, GPIO_CAN_TX_PIN);
  gpio_set_output_options(GPIO_CAN_PORT, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, GPIO_CAN_TX_PIN);
  gpio_set_af(GPIO_CAN_PORT, GPIO_AF9, GPIO_CAN_TX_PIN);							   
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
