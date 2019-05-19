#include "can.h"

#include <libopencm3/stm32/gpio.h>

#define GPIO_CAN_PORT   GPIOA
#define GPIO_CAN_RX_PIN GPIO11
#define GPIO_CAN_TX_PIN GPIO12


void can_setup() {
	// CAN RX
	gpio_mode_setup(GPIO_CAN_PORT, GPIO_MODE_INPUT, GPIO_PUPD_PULLUP, GPIO_CAN_RX_PIN);
	gpio_set_af    (GPIO_CAN_PORT, GPIO_AF9, GPIO_CAN_RX_PIN);

	// CAN TX
	gpio_mode_setup(GPIO_CAN_PORT, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, GPIO_CAN_TX_PIN);
  gpio_set_af    (GPIO_CAN_PORT, GPIO_AF9, GPIO_CAN_TX_PIN);
	gpio_set_output_options(GPIO_CAN_PORT, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, GPIO_CAN_TX_PIN);
}
