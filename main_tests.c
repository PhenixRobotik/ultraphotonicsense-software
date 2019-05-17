#include "lowlevel/clock.h"
#include "lowlevel/gpio.h"
#include "lowlevel/debug.h"

// Here you can list all the test functions
void led_blink();
void all_blink();
void uart_send();

int main() {
  clock_setup();
  gpio_setup();
  // debug_setup();

  led_blink();
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
    echo("hello world (sure ?)\n\r");
    led_toggle_status();
    delay_ms(300);
  }
}
