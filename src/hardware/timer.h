#ifndef TIMER_H
#define TIMER_H

#include <stdint.h>

#define TIM_LED_RCC RCC_TIM1
#define TIM_LED TIM1
#define TIM_LED_OC TIM_OC1 // CH1, Output Compare

#define TIM_DELAY_RCC RCC_TIM6
#define TIM_DELAY TIM6
#define TIM_DELAY_CNTMSK 0xFFFF

#define LED_PWM_HZ 1000U
#define LED_PWM_STEPS 256U
#define LED_PWM_CNTFREQ_HZ (LED_PWM_STEPS * LED_PWM_HZ)
#define LED_FULL_ON 256U

#define SCALE_LED_BRIGHTNESS 8

void timer_setup();

/**
 * @brief Actively wait for a given amount of microseconds
 * @warning This function doesn't disable interrupts nor uses a critical section
 * by itself. The delay may be wrong if the function has been preempted either
 * for more than :
 * - the remaining microseconds to wait for
 * - the counter range (65 ms)
 *
 * @param us The number of microseconds to wait for
 */
void usleep(int us);

void led_on();
void led_off();
void led_toggle_status();
/**
 * @brief Set the LED brightness
 * Any value equal or greater to 256 sets the maximum brightness.
 *
 * @param level the value to set
 */
void led_set_brightness(uint16_t level);

#endif /* TIMER_H */
