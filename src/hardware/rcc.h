#ifndef RCC_H
#define RCC_H

#include <stdint.h>

#define HSI_FREQ (64000000U)
#define SYSTEM_CORE_CLOCK (HSI_FREQ)

#define MICROS_SYSTICK_RATIO (1000000 / SYSTICK_FREQ_HZ)
#define MILLIS_TO_SYSTICK(ms) (ms * 1000 / MICROS_SYSTICK_RATIO)

void clock_setup();

void delay_ms(uint32_t ms);

#endif
