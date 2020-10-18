#include "hardware/timer.h"
/**
 * @brief Exception handlers
 * TODO manage exceptions
 */
 
void hard_fault_handler() {
  while(1);
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

void damn()
{
  led_on();
  while(1);
}