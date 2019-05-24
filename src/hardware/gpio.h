#ifndef GPIO_H
#define GPIO_H

#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/gpio.h>

// Leds
#define GPIO_LED_PORT GPIOA
#define GPIO_LED_PIN  GPIO8

// CAN
#define GPIO_CAN_PORT   GPIOA
#define GPIO_CAN_RX_PIN GPIO11
#define GPIO_CAN_TX_PIN GPIO12
#define GPIO_CAN_AF     GPIO_AF9

// Shift register
#define GPIO_TOF_RESET_PORT      GPIOB
#define GPIO_TOF_RESET_DATA_PIN  GPIO4
#define GPIO_TOF_RESET_SHIFT_PIN GPIO5

// I2C
#define GPIO_I2C_SDA_PORT GPIOB
#define GPIO_I2C_SDA_PIN  GPIO7
#define GPIO_I2C_SCL_PORT GPIOA
#define GPIO_I2C_SCL_PIN  GPIO15
#define GPIO_I2C_AF       GPIO_AF4

// UART
#define GPIO_UART1_PORT    GPIOA
#define GPIO_UART1_RX_PIN  GPIO10
#define GPIO_UART1_TX_PIN  GPIO9
#define GPIO_UART1_AF      GPIO_AF7

// Ultrasonic Sensors
#define GPIO_US_OE_PORT GPIOB
#define GPIO_US_OE_PIN  GPIO0
#define GPIO_US_TRIG_0_PORT GPIOA
#define GPIO_US_TRIG_0_PIN  GPIO0
#define GPIO_US_ECHO_0_PORT GPIOA
#define GPIO_US_ECHO_0_PIN  GPIO1
#define GPIO_US_TRIG_1_PORT GPIOA
#define GPIO_US_TRIG_1_PIN  GPIO2
#define GPIO_US_ECHO_1_PORT GPIOA
#define GPIO_US_ECHO_1_PIN  GPIO3
#define GPIO_US_TRIG_2_PORT GPIOA
#define GPIO_US_TRIG_2_PIN  GPIO4
#define GPIO_US_ECHO_2_PORT GPIOA
#define GPIO_US_ECHO_2_PIN  GPIO5
#define GPIO_US_TRIG_3_PORT GPIOA
#define GPIO_US_TRIG_3_PIN  GPIO6
#define GPIO_US_ECHO_3_PORT GPIOA
#define GPIO_US_ECHO_3_PIN  GPIO7

// SWD
#define GPIO_SWDIO_PORT  GPIOA
#define GPIO_SWDIO_PIN   GPIO13
#define GPIO_SWDCLK_PORT GPIOA
#define GPIO_SWDCLK_PIN  GPIO14
#define GPIO_SWD_AF      GPIO_AF0

void gpio_setup();

void led_on();
void led_off();
void led_toggle_status();

#endif
