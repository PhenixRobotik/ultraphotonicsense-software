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


    /* We call this handler every 1ms so every 1ms = 0.001s
     * resulting in 1000Hz message rate.
     */

    /* Transmit CAN frame. */
    data[0]++;
#if 0
    can_transmit(CAN1,
		 0,     /* (EX/ST)ID: CAN ID */
		 false, /* IDE: CAN ID extended? */
		 false, /* RTR: Request transmit? */
		 8,     /* DLC: Data length */
		 data);
#endif
  }
	
  return 0;
}
