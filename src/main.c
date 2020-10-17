#include "FreeRTOS.h"
#include "task.h"
#include "hardware.h"


TaskHandle_t taskBlink;
static void vTaskBlink(void *arg);

int main()
{
	hardware_setup();
	
	BaseType_t ret = xTaskCreate(
		vTaskBlink,
		"Blink",
		configMINIMAL_STACK_SIZE,
		NULL,
		tskIDLE_PRIORITY + 1,
		&taskBlink);
	if(ret != pdPASS){
		goto err;
	}
	
	vTaskStartScheduler();
	
err:
	for( ;; );
}

static void vTaskBlink(void *arg)
{
	(void)arg;
	
	for( ;; ){
		vTaskDelay(pdMS_TO_TICKS(500));
		led_toggle_status();
	}
}
