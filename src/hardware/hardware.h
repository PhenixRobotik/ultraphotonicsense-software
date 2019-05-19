#ifndef HARDWARE_H
#define HARDWARE_H

#include "rcc.h"
#define SYSTICK_FREQ_HZ   (1000U)
#define SYSTICK_PERIOD    ((SYSTEM_CORE_CLOCK)/(SYSTICK_FREQ_HZ))

#endif
