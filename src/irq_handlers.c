#include "irq_handlers.h"

void can_error_handler(uint32_t can_esr){
  volatile uint32_t esr = can_esr;
  for(int i = 0; i < 10; i++);
}

void can_rx_handler(uint8_t fifo, uint8_t pending, bool full, bool overrun){
  
}
