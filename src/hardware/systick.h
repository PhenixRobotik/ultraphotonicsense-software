#ifndef SYSTICK_H
#define SYSTICK_H

#ifdef USE_SYSTICK
#include "rcc.h"

#define SYSTICK_FREQ_HZ   (1000U)
#define SYSTICK_PERIOD    ((SYSTEM_CORE_CLOCK)/(SYSTICK_FREQ_HZ))
#define MICROS_SYSTICK_RATIO (1000000 / SYSTICK_FREQ_HZ)
#define MILLIS_TO_SYSTICK(ms) (ms * 1000 / MICROS_SYSTICK_RATIO)

/**
 * @brief Initializes and start systick counter and IRQ
 */
void systick_setup();

/**
 * @brief Actively wait for milliseconds
 * @warning This function can be used only if systick is used
 * 
 * @param ms Amount of milliseconds to wait for
 */
void delay_ms(uint32_t ms);
#endif

#endif /* SYSTICK_H */
