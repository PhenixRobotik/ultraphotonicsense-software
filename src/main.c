#include "hardware/rcc.h"
#include "hardware/gpio.h"

#include <stddef.h>
#include <libopencm3/cm3/nvic.h>
#include <libopencm3/cm3/systick.h>
#include <libopencm3/stm32/can.h>
#include <libopencm3/stm32/gpio.h>
#include <libopencm3/stm32/flash.h>
#include <libopencm3/stm32/rcc.h>

#define eternity 1

static int can_setup(void)
{
	/* Enable peripheral clocks. */
	rcc_periph_clock_enable(RCC_CAN1);

	/* NVIC setup. */
	nvic_enable_irq(NVIC_USB_LP_CAN1_RX0_IRQ);
	nvic_set_priority(NVIC_USB_LP_CAN1_RX0_IRQ, 1);

	/* Reset CAN. */
	can_reset(CAN1);

	/* CAN cell init.
	 * Setting the bitrate to 1MBit. APB1 = 32MHz, 
	 * prescaler = 2 -> 16MHz time quanta frequency.
	 * 1tq sync + 9tq bit segment1 (TS1) + 6tq bit segment2 (TS2) = 
	 * 16time quanto per bit period, therefor 16MHz/16 = 1MHz
	 */

	int res = 0;
	
	res = can_init(CAN1,
		       false, // Time-triggered communication mode
		       true,  // Automatically recover from bus-off state
		       false, // Automatically wake up from sleep mode
		       false, // No automatic retransmission (false = automatic)
		       false, // FIFO lock function
		       false, // FIFO priority mode (false = id priority mode)
		       CAN_BTR_SJW_1TQ,
		       CAN_BTR_TS1_9TQ,
		       CAN_BTR_TS2_6TQ,
		       8,
		       false, // Silent mode
		       false  // Loopback mode
		);

		
	
	if(res){
	        return res;
	}
	
	/* CAN filter 0 init. */
	can_filter_id_mask_32bit_init(
				0,     /* Filter ID */
				0,     /* CAN ID */
				0,     /* CAN ID mask */
				0,     /* FIFO assignment (here: FIFO0) */
				true); /* Enable the filter. */

	/* Enable CAN RX interrupt. */
	can_enable_irq(CAN1, CAN_IER_FMPIE0);

	return 0;
}


int main(){
	clock_setup();
	gpio_setup();

	led_off();
	
	int r = can_setup();
	int speed = (r)?(100):(1000);

	static uint8_t data[8] = {0, 1, 2, 0, 0, 0, 0, 0};
	
	while(eternity){
		delay_ms(speed);
		led_toggle_status();


		/* We call this handler every 1ms so every 1ms = 0.001s
		 * resulting in 1000Hz message rate.
		 */

		/* Transmit CAN frame. */
		data[0]++;
		#if 0
		can_transmit(CAN1,
			     0,     /* (EX/ST)ID: CAN ID */
			     false, /* IDE: CAN ID extended? */
			     false, /* RTR: Request transmit? */
			     8,     /* DLC: Data length */
			     data);
		#endif
	}
	
	return 0;
}
