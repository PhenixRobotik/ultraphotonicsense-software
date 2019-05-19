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

void set_all_1() {
  rcc_periph_clock_enable(RCC_GPIOA);
  rcc_periph_clock_enable(RCC_GPIOB);
  rcc_periph_clock_enable(RCC_GPIOF);

  gpio_mode_setup(GPIOA, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, ~0);
  gpio_mode_setup(GPIOB, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, ~0);
  gpio_mode_setup(GPIOF, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, ~0);
  gpio_set(GPIOA, ~0);
  gpio_set(GPIOB, ~0);
  gpio_set(GPIOF, ~0);
}


void set_all_0() {
  rcc_periph_clock_enable(RCC_GPIOA);
  rcc_periph_clock_enable(RCC_GPIOB);
  rcc_periph_clock_enable(RCC_GPIOF);

  gpio_mode_setup(GPIOA, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, ~0);
  gpio_mode_setup(GPIOB, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, ~0);
  gpio_mode_setup(GPIOF, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, ~0);
  gpio_clear(GPIOA, ~0);
  gpio_clear(GPIOB, ~0);
  gpio_clear(GPIOF, ~0);
}
