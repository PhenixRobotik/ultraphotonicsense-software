#ifndef RCC_H
#define RCC_H

#include <libopencm3/stm32/rcc.h>

#define HSI_FREQ 8000000U
#define HSE_FREQ 16000000U
#define SYSTEM_CORE_CLOCK 64000000U
#define DELAY_TIMER TIM6
#define RCC_USE_HSE

void clock_setup();

#endif
