#include "tests.h"
#include "FreeRTOS.h"
#include "task.h"
#include "hardware/sys_init.h"
#include "hardware/timer.h"


#include <math.h>
#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif // M_PI

#define SINE_FREQ_HZ 0.5

static TaskHandle_t taskBlinkTst;
static void vTaskBlinkTst(void *arg);

void test_blink()
{
    system_init();
    
    BaseType_t ret = xTaskCreate(
        vTaskBlinkTst,
        "test_blink",
        configMINIMAL_STACK_SIZE,
        NULL,
        tskIDLE_PRIORITY + 1,
        &taskBlinkTst);
    if(ret != pdPASS){
        goto err;
    }
    
    vTaskStartScheduler();
    
err:
    for( ;; );
}

static void vTaskBlinkTst(void *arg)
{
	(void)arg;
	
    led_on();
    
	for( ;; ){
        const int step_us = (1000000 / SINE_FREQ_HZ) / LED_PWM_STEPS;
        for(int i = 0; i < (int)LED_PWM_STEPS; ++i){
            float c = 256. * (1. + sin(i*2*M_PI / (float)LED_PWM_STEPS)) / 2.;
            uint16_t brightness = c;
            led_set_brightness(brightness);
            usleep(step_us);
        }
        led_set_brightness(128);
        for(int i = 0; i < 4; ++i){
            led_off();
            usleep(125000);
            led_on();
            usleep(125000);
        }
	}
}
