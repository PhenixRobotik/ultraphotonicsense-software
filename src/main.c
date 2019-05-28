#include "hardware.h"
#include "com_can/link_can.h"
#include "com_can/rc_server.h"
#include "jostof.h"
#include "us_sensor.h"
#include <libopencm3/stm32/timer.h>

#define eternity 1

void hard_fault_handler() {
  return; //mdr
  //while(1);
}

void mem_manage_handler () {
  while(1);
}

void bus_fault_handler () {
  while(1);
}

void usage_fault_handler () {
  while(1);
}

void restart_timer();
volatile int a;
void test_tim2(){
  while(1){

    a = a + 0xdeadbeef;
    while(timer_get_counter(TIM2) < 1000);
    while(timer_get_counter(TIM2) > 10);
    led_toggle_status();
  }
}

int main(){
  int status = hardware_setup();
  int speed = (status)?(5):(50);
  setup_com();

  delay_ms(10);
  init_jostof();
  init_us_sensors();
  
  led_off();
  //test_tim2();
  
  int i = 0;
  while(eternity){

    //delay_ms(20);
    if(!((++i)%speed)){
      led_toggle_status();
    }
    update_jostof();
    update_us();
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
