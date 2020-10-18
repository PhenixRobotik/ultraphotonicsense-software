#ifndef RCC_H
#define RCC_H

#include <libopencm3/stm32/rcc.h>

#define HSI_FREQ 8000000U
#define HSE_FREQ 16000000U
#define SYSTEM_CORE_CLOCK 64000000U
#define DELAY_TIMER TIM6
#define RCC_USE_HSE

#define TIM_DELAY_RCC RCC_TIM6
#define TIM_DELAY TIM6
#define TIM_DELAY_CNTMSK 0xFFFF

void clock_setup();

/**
 * @brief Setup the basic timer TIM6 for microsecond delays
 *
 */
void delay_setup();

/**
 * @brief Actively wait for a given amount of microseconds
 * @warning This function doesn't disable interrupts nor uses a critical section
 * by itself. The delay may be wrong if the function has been preempted either
 * for more than :
 * - the remaining microseconds to wait for
 * - the counter range (65 ms)
 *
 * @param n The number of microseconds to wait for
 */
void delay_us(int n);

#endif
