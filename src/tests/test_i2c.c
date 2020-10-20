#include "tests.h"
#include "FreeRTOS.h"
#include "task.h"
#include "hardware/sys_init.h"
#include "hardware/timer.h"
#include "hardware/i2c.h"
#include "board/shift_reg.h"


static TaskHandle_t taskI2C_Test;
static void vTaskI2C_Test(void *arg);

void test_i2c()
{
    system_init();
    
    BaseType_t ret = xTaskCreate(
        vTaskI2C_Test,
        "test_i2c",
        configMINIMAL_STACK_SIZE,
        NULL,
        tskIDLE_PRIORITY + 1,
        &taskI2C_Test);
    if(ret != pdPASS){
        goto err;
    }
    
    vTaskStartScheduler();
    
err:
    for( ;; );
}

static void vTaskI2C_Test(void *arg)
{
	(void)arg;
	
    led_on();
    
    // First ensure the shiftreg is 0
    for(int i = 0; i < 8; ++i){
        shiftreg_shift(0);
    }
    // Then start the device
    shiftreg_shift(1);
    usleep(50000);
    
    uint8_t msg[3] = {
        [0] = 0, // index MSB
        [1] = 1, // index LSB (I2C slave device address)
        [2] = (0x52 >> 1) + 1, // 7-bit shifted new address
    };
	
    
	for( ;; ){
        msg[2] = (0x52 >> 1) + 1;
        // Send index
        I2C_Status ret =
            i2c_master_send7(I2C1, 0x52 >> 1,
                    msg, 3, pdMS_TO_TICKS(10), true);
        if(ret != I2C_OK){
            led_off();
        }
        usleep(50000);
        msg[2] = (0x52 >> 1);
        ret =
            i2c_master_send7(I2C1, (0x52 >> 1) + 1,
                    msg, 3, pdMS_TO_TICKS(10), true);
        if(ret != I2C_OK){
            led_off();
        }
        usleep(50000);
	}
}
