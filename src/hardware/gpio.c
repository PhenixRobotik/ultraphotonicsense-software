#include "gpio.h"

#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/gpio.h>

void gpio_setup() {
	rcc_periph_clock_enable(RCC_GPIOA);

	// status led
	gpio_mode_setup(GPIOA, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, GPIO8);

	/* GPIO CAN */
	// CAN RX
	gpio_mode_setup(GPIOA, GPIO_MODE_INPUT, GPIO_PUPD_PULLUP, GPIO11);
	gpio_set_af(GPIOA, GPIO_AF9, GPIO11);
	
	// CAN TX
	gpio_mode_setup(GPIOA, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, GPIO12);
	gpio_set_output_options(GPIOA, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, GPIO12);
	gpio_set(GPIOA, GPIO12);
	gpio_set_af(GPIOA, GPIO_AF9, GPIO12);
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
