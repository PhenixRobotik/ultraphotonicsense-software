#include "systick.h"

#ifdef USE_SYSTICK

#include <libopencm3/cm3/systick.h>

void systick_setup(){
	systick_set_clocksource(STK_CSR_CLKSOURCE_AHB);
	// Interrupts each millisec
	systick_set_reload(SYSTICK_PERIOD - 1);
	// clear counter so it starts right away
	systick_clear();
	systick_counter_enable();
	systick_interrupt_enable();
}

volatile uint32_t systicks = 0;
void sys_tick_handler() {
	systicks++;
}

void delay_ms(uint32_t ms) {
	uint32_t count_max = systicks + MILLIS_TO_SYSTICK(ms);
	while(systicks < count_max) {}
}

#endif