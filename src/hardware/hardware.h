#ifndef HARDWARE_H
#define HARDWARE_H

#include "rcc.h"
#include "gpio.h"
#include "can.h"

int hardware_setup(void);
/**
 * Set-up all the STM32 peripherals.
 * Returns 0 on success, -1 on error.
 */

#endif
