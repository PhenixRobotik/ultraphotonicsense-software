#include "link_can.h"

#include "../hardware/rcc.h"
#include "robotronik_protocol.h"
#include <libopencm3/stm32/can.h>

/* Interface rpi */
RP_Interface pi_iface;

void setup_com() {
  /* Robotronik Protocol */
  RP_Init_Interface(&pi_iface,
		    (void*)CAN1, // link handler unused
		    TX_CAN_ID,   // ID of the frames to send
		    rp_send,
		    get_systicks);		    
}

void can_error_handler(uint32_t can_esr){
  (void)can_esr;
  // TODO : use it
}

void can_rx_handler(uint8_t fifo, uint8_t pending, bool full, bool overrun){
  bool ext;
  uint8_t fmi, len = 0;
  uint16_t timestamp;
  uint32_t id;
  uint8_t data[8];

  if(fifo != CAN_RX_FIFO || !pending) {
    // Should not happen, i'm out
    return;
  }

  // Receive the Great bits of the CAN, the First of their Frame,
  // Breaker of the CAN filters, Slayer of CPU time
  can_receive(CAN1,
	      CAN_RX_FIFO,
	      true, // release FIFO
	      &id,
	      &ext,
	      false, // RTR : we don't do that here
	      &fmi,
	      &len,
	      data,
	      &timestamp);
  
  if((id & CAN_ID_MASK) != RX_CAN_ID) {
    // All you had to do was filtering the damn trame, ST !
    return;
  }

  // Process data
  RP_Process_Data(&pi_iface, data, len);

  // Don't care
  (void)full;
  (void)overrun;
  (void)ext;
  (void)fmi;
  (void)timestamp;
}

#define ragequit() return -1;

uint8_t rp_send(void* link_handler, uint32_t can_id, uint8_t* data, uint16_t len, uint32_t timeout){
  uint32_t tick_ragequit = get_systicks() + timeout;

  while(len){
    int rage = 0;
    uint16_t frame_size;
    do{
      frame_size = (len < 4)?(len):4;
      rage = can_transmit((uint32_t)link_handler,
			  can_id,
			  false,
			  false,
			  frame_size,
			  data);
    }while(rage && get_systicks() < tick_ragequit);
    if(rage){
      ragequit();
    }
    len -= frame_size;
    data += frame_size;
   }
  
  return 0;
}

void RP_Packet_Received(RP_Interface* interface, RP_Packet *packet){
  (void)interface;
  (void)packet;
}

void RP_Error_Handler(RP_Interface *interface, uint16_t err){
  (void)interface;
  (void)err;
}
