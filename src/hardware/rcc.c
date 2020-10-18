#include "rcc.h"
#include "i2c.h"
#include "timer.h"

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
	
	// LED timer
	rcc_periph_clock_enable(TIM_LED_RCC);
}
