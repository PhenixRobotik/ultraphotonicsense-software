#include "us_sensor.h"

#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/timer.h>
#include <stdint.h>
#include <stdbool.h>
#include <libopencm3/cm3/cortex.h>
#include "gpio.h"
#include "remote_call.h"
#include "rc_server.h"

#define NB_US 2

typedef struct US_Sensor_S{
  int trig_gpio_port;
  int trig_gpio_pin;
  int echo_gpio_port;
  int echo_gpio_pin;
  bool echo_started;
  bool echo_stopped;
  int start_pulse;
  int last_val;
}US_Sensor;

static US_Sensor sensors[NB_US] = {0};

static void reset_pulse(){
  int i;
  for(i = 0; i < NB_US; i++){
    sensors[i].echo_started =
      sensors[i].echo_stopped = false;
  }
}

#define TIM2_CLK 64000000
#define COUNTER_PERIOD_US 10

void init_us_sensors(){
  // Timer
  rcc_periph_clock_enable(RCC_TIM2);
  rcc_periph_reset_pulse(RST_TIM2);
  timer_set_mode(TIM2, TIM_CR1_CKD_CK_INT,TIM_CR1_CMS_EDGE, TIM_CR1_DIR_UP);
  timer_set_prescaler(TIM2, 640);
  timer_disable_preload(TIM2);
  timer_continuous_mode(TIM2);
  timer_set_period(TIM2, 2000);
  timer_set_oc_value(TIM2, TIM_OC1,0);
  timer_enable_counter(TIM2);

  // Enable outputs
  gpio_set(GPIO_US_OE_PORT, GPIO_US_OE_PIN);
  
  // Sensors data
  int i;
  for(i = 0; i < NB_US; ++i){
    sensors[i].last_val = -1;
  }
  

  sensors[0].trig_gpio_port = GPIO_US_TRIG_0_PORT;
  sensors[0].trig_gpio_pin = GPIO_US_TRIG_0_PIN;
  sensors[0].echo_gpio_port = GPIO_US_ECHO_0_PORT;
  sensors[0].echo_gpio_pin = GPIO_US_ECHO_0_PIN;

#if 0
  sensors[1].trig_gpio_port = GPIO_US_TRIG_1_PORT;
  sensors[1].trig_gpio_pin = GPIO_US_TRIG_1_PIN;
  sensors[1].echo_gpio_port = GPIO_US_ECHO_1_PORT;
  sensors[1].echo_gpio_pin = GPIO_US_ECHO_1_PIN;
#endif
  
  sensors[1].trig_gpio_port = GPIO_US_TRIG_1_PORT;
  sensors[1].trig_gpio_pin = GPIO_US_TRIG_1_PIN;
  sensors[1].echo_gpio_port = GPIO_US_ECHO_1_PORT;
  sensors[1].echo_gpio_pin = GPIO_US_ECHO_1_PIN;
}

// timeout 10ms -> max 1,7m

void update_us(){
  while(timer_get_counter(TIM2) > 10); // Les technologies de demain s'imaginent aujourd'hui
  // Trigger SR04
  int i;
  for(i = 0; i < NB_US; ++i){
    gpio_set(sensors[i].trig_gpio_port, sensors[i].trig_gpio_pin);
  }
  while(timer_get_counter(TIM2) < 2);
  for(i = 0; i < NB_US; ++i){
    gpio_clear(sensors[i].trig_gpio_port, sensors[i].trig_gpio_pin);
  }

  reset_pulse();
  int counter;
  while((counter = timer_get_counter(TIM2)) < 1900){
    for(i = 0; i < NB_US; ++i){
      
      if(!sensors[i].echo_started){
	if(gpio_get(sensors[i].echo_gpio_port,
		     sensors[i].echo_gpio_pin) != 0){
	  sensors[i].echo_started = true;
	  sensors[i].start_pulse = counter;
     	}
      } // If echo still active and it stops
      else if((!sensors[i].echo_stopped) &&
	      (gpio_get(sensors[i].echo_gpio_port,
			 sensors[i].echo_gpio_pin) == 0)){
	sensors[i].echo_stopped = true;
        sensors[i].last_val = (counter - sensors[i].start_pulse) * COUNTER_PERIOD_US * 340 * 100 / (2*1000000);
      }
    }
  }
  
  for(i = 0; i < NB_US; ++i){
    if(!sensors[i].echo_stopped){
      sensors[i].last_val = -1;
    }
  }
}

void get_us_data(RC_Server *server){
  RC_Server_Return(server,
		   sensors[0].last_val,
		   sensors[1].last_val);
}
