#include "hardware.h"
#include "com_can/link_can.h"
#include "com_can/rc_server.h"
#include "jostof.h"

#define eternity 1

void hard_fault_handler() {

}

void mem_manage_handler () {

}

void bus_fault_handler () {

}

void usage_fault_handler () {

}

int main(){
  int status = hardware_setup();
  int speed = (status)?(100):(1000);
  setup_com();
  init_jostof();
  
  led_off();

  while(eternity){
    delay_ms(speed);
    //led_toggle_status();

    //RC_Server_Poll(&server);
  }
  
  return 0;
}



#if 0
pi_iface.s_packet.data[0] = 0xA;
pi_iface.s_packet.len = 1;

RP_Send(&pi_iface, &pi_iface.s_packet, 100);
    
#if 0
/* Transmit CAN frame. */
data[0]++;
    
can_transmit(CAN1,
	     1,     /* (EX/ST)ID: CAN ID */
	     false, /* IDE: CAN ID extended? */
	     false, /* RTR: Request transmit? */
	     8,     /* DLC: Data length */
	     data);
#endif
#endif
