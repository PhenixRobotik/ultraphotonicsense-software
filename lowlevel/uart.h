#pragma once

#include <stdint.h>
#include <stdbool.h>

/***************************************
simple uart configuration for debugging
***************************************/

// DEFINES FOR DEBUG_UART
#define DEBUG_RCC_USART RCC_USART1
#define DEBUG_USART USART1
#define DEBUG_UART_SPEED (9600*2)
// Bug in libopencm3 ? Need to *2 because of APB2/APB1 weird things.

#define DEBUG_PORT_TX GPIOA
#define DEBUG_PORT_TX_RCC RCC_GPIOA // clock of GPIO port
#define DEBUG_PIN_TX GPIO9
#define DEBUG_AF_TX GPIO_AF7

#define DEBUG_PORT_RX GPIOA
#define DEBUG_PORT_RX_RCC RCC_GPIOA
#define DEBUG_PIN_RX GPIO10
#define DEBUG_AF_RX GPIO_AF7

void uart_setup();

// Sending functions
void uart_send_string(char* chain); // NULL-ended string
void uart_send_int(int integer);

// Warning : hard-limit on string length to 80 chars
void uart_send_string_formatted(const char *fmt, ...);


// Receiving functions
bool    received_line_finished();
uint8_t new_line_received();
char* get_last_received_line(); // Will clear last buffer
int UART_getc(char *c);

//rec=usart_recv_blocking(DEBUG_USART);//to receive a byte
