#ifndef GPIO_H
#define GPIO_H

#define GPIO_LED_PORT GPIOA
#define GPIO_LED_PIN  GPIO8

#define GPIO_CAN_PORT   GPIOA
#define GPIO_CAN_RX_PIN GPIO11
#define GPIO_CAN_TX_PIN GPIO12

#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/gpio.h>

void gpio_setup();

void led_on();
void led_off();
void led_toggle_status();

#endif
