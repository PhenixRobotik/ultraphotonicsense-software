#ifndef IRQ_HANDLERS_H
#define IRQ_HANDLERS_H

#include "hardware/can.h"

void can_error_handler(uint32_t can_esr);

#endif
