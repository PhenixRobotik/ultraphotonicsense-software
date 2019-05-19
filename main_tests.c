#include "lowlevel/clock.h"
#include "lowlevel/gpio.h"
#include "lowlevel/uart.h"

// Here you can list all the test functions
void led_blink();
void all_blink();
void uart_send();
void uart_parrot();

int main() {
  clock_setup();
  gpio_setup();
  uart_setup();

  uart_parrot();
  // led_blink();
  while (1) {};
  return 0;
}


void led_blink() {
  while(true) {
    led_toggle_status();
    delay_ms(1000);
  }
}

void all_blink() {
  while(true) {
    set_all_0();
    delay_ms(1000);
    set_all_1();
    delay_ms(1000);
  }
}

void uart_send() {
  while(true) {
    uart_send_string("hello world\n\r");
    // led_toggle_status();
    delay_ms(300);
  }
}


// Should be kinda implemented in a function ?
void uart_parrot() {
  char line[80];
  uint16_t curr = 0;

  while(true) {
    led_set_status(0);

    if (UART_getc(&line[curr])) {

      led_set_status(1);

      if (line[curr] == '\r' || line[curr] == '\n') {
        line[curr++] = '\n';
        line[curr++] = '\r';
        line[curr++] = '\0';
        uart_send_string(line);

        curr = 0;
      } else {
        curr++;
      }
    }

    delay_ms(30);
  }
}
