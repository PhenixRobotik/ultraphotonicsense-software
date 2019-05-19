#ifndef RCC_H
#define RCC_H

#include <stdint.h>

#define SYSTICK_FREQ_HZ   (1000U)
#define SYSTICK_PERIOD    ((SYSTEM_CORE_CLOCK)/(SYSTICK_FREQ_HZ))

#define HSI_FREQ (64000000U) // /!\ not used in rcc.c
#define SYSTEM_CORE_CLOCK (HSI_FREQ)

#define MICROS_SYSTICK_RATIO (1000000 / SYSTICK_FREQ_HZ)
#define MILLIS_TO_SYSTICK(ms) (ms * 1000 / MICROS_SYSTICK_RATIO)

void clock_setup();

uint32_t get_frequency();
uint32_t get_systicks();
uint32_t get_uptime_ms();

void delay_ms(uint32_t ms);void clock_setup();

void delay_ms(uint32_t ms);

#endif
