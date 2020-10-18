#include "rcc.h"
#include "i2c.h"

#include <libopencm3/stm32/timer.h>

void clock_setup()
{
#ifdef RCC_USE_HSE
	const struct rcc_clock_scale rcc_hse_16to64 = {
		.pllsrc = RCC_CFGR_PLLSRC_HSE_PREDIV, // 16 MHz HSE
		.pllmul = RCC_CFGR_PLLMUL_MUL4, // SYSCLK @ 64 MHz
		.plldiv = RCC_CFGR2_PREDIV_NODIV,
		.usbdiv1 = false,
		.flash_waitstates = 2, // 48 MHz < HCLK <= 72 MHz
		.hpre = RCC_CFGR_HPRE_DIV_NONE, // HCLK (AHB) @ 64 MHz
		.ppre1 = RCC_CFGR_PPRE1_DIV_2, // APB1 @ 32 MHz
		.ppre2 = RCC_CFGR_PPRE2_DIV_NONE, // APB2 @ 64 MHz
		.ahb_frequency = 64000000U,
		.apb1_frequency = 32000000U,
		.apb2_frequency = 64000000U,
	};
	rcc_clock_setup_pll(&rcc_hse_16to64);
#else
	/**
	 * Use 8MHz HSI
	 * Set SYSCLK, AHB and APB2 buses @ 64 MHz
	 * Set APB1 @ 32 MHz
	 */
	rcc_clock_setup_pll(&rcc_hsi_configs[RCC_CLOCK_HSI_64MHZ]);
#endif

	// IO ports 
	rcc_periph_clock_enable(RCC_GPIOA);
	rcc_periph_clock_enable(RCC_GPIOB);
	
	// I2C1
	rcc_set_i2c_clock_hsi(I2C1); // TODO try HSE
	rcc_periph_clock_enable(RCC_I2C1);
	
	// CAN
	rcc_periph_clock_enable(RCC_CAN1);

	// UART
	rcc_periph_clock_enable(RCC_USART1);
	
	// Delay timer
	rcc_periph_clock_enable(TIM_DELAY_RCC);
}

void delay_setup()
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

void delay_us(int n)
{
	uint16_t cnt_last = TIM_CNT(TIM_DELAY) & TIM_DELAY_CNTMSK;
	while(n > 0){
		uint16_t cnt = TIM_CNT(TIM_DELAY) & TIM_DELAY_CNTMSK;
		uint16_t delta = cnt - cnt_last;
		n -= delta;
		cnt_last = cnt;
	}
}