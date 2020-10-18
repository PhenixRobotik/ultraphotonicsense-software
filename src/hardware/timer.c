#include "timer.h"

#include "rcc.h"
#include <libopencm3/stm32/timer.h>

static void usleep_setup();
static void led_pwm_setup();

void timer_setup()
{
  usleep_setup();
	led_pwm_setup();
}

static void led_pwm_setup()
{
  // TODO check TIM1 clock source  
   // Set continuous up-counting mode
  timer_set_mode(TIM_LED, TIM_CR1_CKD_CK_INT,
				 TIM_CR1_CMS_EDGE, TIM_CR1_DIR_UP);
  timer_enable_preload(TIM_LED);
  timer_continuous_mode(TIM_LED);
  
  // Configure frequency and periods
  timer_set_counter(TIM_LED, 0);
  timer_set_prescaler(TIM_LED, (SYSTEM_CORE_CLOCK / LED_PWM_CNTFREQ_HZ) - 1);
  timer_set_period(TIM_LED, LED_PWM_STEPS - 1);
  timer_set_repetition_counter(TIM_LED, 0);
  
  // Reload registers on event
  timer_update_on_any(TIM_LED);
  timer_enable_update_event(TIM_LED);
  
  // Set PWM mode
  timer_set_oc_slow_mode(TIM_LED, TIM_LED_OC);
  timer_set_oc_mode(TIM_LED, TIM_LED_OC, TIM_OCM_PWM1);
  timer_set_oc_polarity_high(TIM_LED, TIM_LED_OC);
  timer_enable_oc_preload(TIM_LED, TIM_LED_OC);
  
  // Enable output control
  timer_enable_break_main_output(TIM_LED);
  
  // Set full brightness
  led_set_brightness(LED_PWM_STEPS);
}


static void usleep_setup()
{
	// Auto-Reload register buffering is not required
	timer_disable_preload(TIM_DELAY);
	
	// Keep running on overflow
	timer_continuous_mode(TIM_DELAY);
	
	// Generate an update event on overflow (not yet used)
	timer_update_on_overflow(TIM_DELAY);
	timer_enable_update_event(TIM_DELAY);
	
	// Set the prescaler to count milliseconds
	timer_set_prescaler(TIM_DELAY, (SYSTEM_CORE_CLOCK / 1000000) - 1);
	
	// Use the maximum period (65 536 ms)
	timer_set_period(TIM_DELAY, TIM_DELAY_CNTMSK);
	
	// Start the timer
	timer_enable_counter(TIM_DELAY);
}

void usleep(int us)
{
	uint16_t cnt_last = TIM_CNT(TIM_DELAY) & TIM_DELAY_CNTMSK;
	while(us > 0){
		uint16_t cnt = TIM_CNT(TIM_DELAY) & TIM_DELAY_CNTMSK;
		uint16_t delta = cnt - cnt_last;
		us -= delta;
		cnt_last = cnt;
	}
}

void led_set_brightness(uint16_t level){
#ifdef SCALE_LED_BRIGHTNESS
  if(level > LED_PWM_STEPS){
    level = LED_PWM_STEPS;
  }
  level /= SCALE_LED_BRIGHTNESS;
#endif
	timer_set_oc_value(TIM_LED, TIM_LED_OC, level);
}

void led_on()
{
  // Reset the counter and update the registers (RM0316 §20.3.11)
  timer_generate_event(TIM_LED, TIM_EGR_UG);
  timer_enable_oc_output(TIM_LED, TIM_LED_OC);
  timer_enable_counter(TIM_LED);
}

void led_off()
{
  timer_disable_oc_output(TIM_LED, TIM_LED_OC);
  timer_disable_counter(TIM_LED);
}

void led_toggle_status()
{
  if(TIM_CR1(TIM_LED) & TIM_CR1_CEN){
	  led_off();
  }else{
	  led_on();
  }
}
