#include "gpio.h"

#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/gpio.h>

#define GPIO_LED_PORT GPIOA
#define GPIO_LED_PIN  GPIO8

void gpio_setup() {
  rcc_periph_clock_enable(RCC_GPIOA);

  // status led
  gpio_mode_setup(GPIO_LED_PORT, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, GPIO_LED_PIN);
}

void led_toggle_status() {
  gpio_toggle(GPIO_LED_PORT, GPIO_LED_PIN);
}

void led_set_status(uint32_t status) {
  if (status == 0)
    gpio_clear(GPIO_LED_PORT, GPIO_LED_PIN);
  else
    gpio_set  (GPIO_LED_PORT, GPIO_LED_PIN);
}

