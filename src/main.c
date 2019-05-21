#include "hardware.h"

#define eternity 1

int main(){
  int status = hardware_setup();
  
  led_off();
  
  int speed = (status)?(100):(1000);

  static uint8_t data[8] = {0, 1, 2, 0, 0, 0, 0, 0};


  
  while(eternity){
    delay_ms(speed);
    led_toggle_status();

    /* Transmit CAN frame. */
    data[0]++;
    can_transmit(CAN1,
		 1,     /* (EX/ST)ID: CAN ID */
		 false, /* IDE: CAN ID extended? */
		 false, /* RTR: Request transmit? */
		 8,     /* DLC: Data length */
		 data);
  }
	
  return 0;
}
