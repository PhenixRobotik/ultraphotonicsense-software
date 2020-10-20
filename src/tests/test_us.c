#include "tests.h"
#include "FreeRTOS.h"
#include "task.h"
#include "hardware/sys_init.h"
#include "hardware/gpio.h"
#include "hardware/timer.h"
#include "board/hc_sr04.h"

static TaskHandle_t taskUSTst;
static void vTaskUSTst(void *arg);

void test_us()
{
    system_init();
    
    BaseType_t ret = xTaskCreate(
        vTaskUSTst,
        "test_us",
        configMINIMAL_STACK_SIZE,
        NULL,
        tskIDLE_PRIORITY + 1,
        &taskUSTst);
    if(ret != pdPASS){
        goto err;
    }
    
    vTaskStartScheduler();
    
err:
    for( ;; );
}

static uint16_t get_us_dist()
{
    us_sensor_trig();
    usleep(10);
    uint16_t time_us;
    bool rising_edge = false;
    uint16_t rise_time = 0;
    uint8_t h_cnt = 0;
    uint8_t l_cnt = 0;
    for(time_us = 0; time_us < 32768; time_us++){
        if(gpio_port_read(GPIO_US_PORT) & GPIO_US_ECHO_3_PIN){
            l_cnt = 0;
            h_cnt++;
            if((h_cnt > 5) && !rise_time){
                rise_time = time_us;
                rising_edge = true;
            }
            
        }else{
            h_cnt = 0;
            l_cnt++;
            if(rising_edge && (l_cnt > 5)){
                break;
            }
        }
        
        usleep(1);
    }
    return time_us - rise_time;
}

#define ULTRASONIC_CM_PER_USEC (1e2*340./1e6/2.)
static void vTaskUSTst(void *arg)
{
	(void)arg;
    
    us_sensor_init();
    
    led_set_brightness(0);
    led_on();
	for( ;; ){
        uint16_t pulse = get_us_dist();
        float d = pulse * ULTRASONIC_CM_PER_USEC;
        float b_led = 256 - d;
        if(b_led < 0) b_led = 0;
        led_set_brightness(b_led);
        usleep(60000-pulse);
	}
}
