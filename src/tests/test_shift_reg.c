#include "tests.h"
#include "FreeRTOS.h"
#include "task.h"
#include "hardware/sys_init.h"
#include "hardware/timer.h"
#include "board/shift_reg.h"

static TaskHandle_t taskShiftRegTst;
static void vTaskShiftRegTst(void *arg);

void test_shift_reg()
{
    system_init();
    
    BaseType_t ret = xTaskCreate(
        vTaskShiftRegTst,
        "test_shiftreg",
        configMINIMAL_STACK_SIZE,
        NULL,
        tskIDLE_PRIORITY + 1,
        &taskShiftRegTst);
    if(ret != pdPASS){
        goto err;
    }
    
    vTaskStartScheduler();
    
err:
    for( ;; );
}

static void shift_wait(bool bit)
{
    shiftreg_shift(bit);
    vTaskDelay(pdMS_TO_TICKS(2500));
    led_off();
    vTaskDelay(pdMS_TO_TICKS(2500));
    led_on();
}

static void vTaskShiftRegTst(void *arg)
{
    (void)arg;
    
    led_on();
    
    for( ;; ){
        shift_wait(1);
        for(int i = 0; i < 7; ++i){
            shift_wait(0);
        }
    }
}