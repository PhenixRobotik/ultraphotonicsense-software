#include "tests.h"
#include "FreeRTOS.h"
#include "task.h"
#include "hardware/sys_init.h"
#include "hardware/gpio.h"

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
	
    led_off();
	for( ;; ){
		delay_us(500000);
		led_toggle_status();
	}
}
