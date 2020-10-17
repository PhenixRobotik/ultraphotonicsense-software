#ifndef GPIO_H
#define GPIO_H

#include <libopencm3/stm32/gpio.h>

// Leds
#define GPIO_LED_PORT GPIOA
#define GPIO_LED_PIN  GPIO8

// CAN
#define GPIO_CAN_PORT   GPIOA
#define GPIO_CAN_RX_PIN GPIO11
#define GPIO_CAN_TX_PIN GPIO12
#define GPIO_CAN_AF     GPIO_AF9

// I2C
#define GPIO_I2C1_SCL_PORT  GPIOA
#define GPIO_I2C1_SCL_PIN   GPIO15
#define GPIO_I2C1_SDA_PORT  GPIOB
#define GPIO_I2C1_SDA_PIN   GPIO7
#define GPIO_I2C_AF         GPIO_AF4

// PCA9517 enable
#define GPIO_I2CEN_PORT GPIOB
#define GPIO_I2CEN_PIN  GPIO1

// Shift register
#define GPIO_TOF_RST_PORT      GPIOB
#define GPIO_TOF_RST_DATA_PIN  GPIO4
#define GPIO_TOF_RST_SHIFT_PIN GPIO5

// UART
#define GPIO_UART1_PORT     GPIOA
#define GPIO_UART1_RX_PIN   GPIO10
#define GPIO_UART1_TX_PIN   GPIO9
#define GPIO_UART1_AF       GPIO_AF7

// Ultrasonic Sensors
#define GPIO_US_OE_PORT GPIOB
#define GPIO_US_OE_PIN  GPIO0
#define GPIO_US_PORT GPIOA
#define GPIO_US_TRIG_0_PIN  GPIO0
#define GPIO_US_ECHO_0_PIN  GPIO1
#define GPIO_US_TRIG_1_PIN  GPIO2
#define GPIO_US_ECHO_1_PIN  GPIO3
#define GPIO_US_TRIG_2_PIN  GPIO4
#define GPIO_US_ECHO_2_PIN  GPIO5
#define GPIO_US_TRIG_3_PIN  GPIO6
#define GPIO_US_ECHO_3_PIN  GPIO7
#define GPIO_US_TRIG_PINS 0b01010101
#define GPIO_US_ECHO_PINS 0b10101010

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
