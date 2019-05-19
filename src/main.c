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

static void can_setup(void)
{
	/* Enable peripheral clocks. */
	//rcc_periph_clock_enable(RCC_GPIOA);
	rcc_periph_clock_enable(RCC_CAN1);

	/* Configure CAN pin: RX (input pull-up). */
	//gpio_mode_setup(GPIOA, GPIO_MODE_INPUT, GPIO_PUPD_PULLUP, GPIO11);

	/* Configure CAN pin: TX. */
	//gpio_mode_setup(GPIOA, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, GPIO12);
	//gpio_set_output_options(GPIOA, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, GPIO12);
	gpio_set(GPIOA, GPIO12);
	
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

	int sjw = 0;
	// [0, 3] CAN_BTR_SJW_SHIFT
	int ts1 = 0;
	// [0, F] CAN_BTR_TS1_SHIFT
	int ts2 = 0;
	// [0, 7] CAN_BTR_TS2_SHIFT
	int prescaler = 0;

	int mi_chemin[4] = {0, 0, 0, 0};
	int res = 0;
	
	for(prescaler = 0; prescaler < 256; ++prescaler){
		for(sjw = 0; sjw < 0x4; ++sjw){
			for(ts1 = 0; ts1 < 0x10; ++ts1){
				for(ts2 = 0; ts2 < 0x8; ++ts2){
					can_reset(CAN1);
					delay_ms(1);
					led_toggle_status();
					delay_ms(1);
					led_toggle_status();
				        res = can_init(CAN1,
							   false, // Time-triggered communication mode
							   true,  // Automatically recover from bus-off state
							   false, // Automatically wake up from sleep mode
							   false, // No automatic retransmission (false = automatic)
							   false, // FIFO lock function
							   false, // FIFO priority mode (false = id priority mode)
							   sjw << CAN_BTR_SJW_SHIFT,
							   ts1 << CAN_BTR_TS1_SHIFT,
							   ts2 << CAN_BTR_TS2_SHIFT,
							   prescaler,
							   false, // Silent mode
							   false  // Loopback mode
						);
					delay_ms(1);
					if(res == 0){
						mi_chemin[0] = sjw;
						mi_chemin[1] = ts1;
						mi_chemin[2] = ts2;
						mi_chemin[3] = prescaler;
						ts2 = 0xFFFF;
						ts1 = 0xFFFF;
						sjw = 0xFFFF;
						prescaler = 0xFFFF;
					}
				}
			}
		}
	}
		
	
	if(res){
		while(((CAN_MSR(CAN1) & CAN_MSR_INAK) == CAN_MSR_INAK));
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
}

void test_gpio_can_tx() {
	led_on();
	gpio_set(GPIO_CAN_PORT, GPIO_CAN_TX_PIN);
	delay_ms(2000);
	led_off();
	gpio_clear(GPIO_CAN_PORT, GPIO_CAN_TX_PIN);
	delay_ms(2000);
}

#define xRCC_APB1_CAN_MASK BIT25

// Common
#include <libopencm3/cm3/common.h>
#define CLEAR(var, mask) ((var) &= ~((uint32_t) mask))
#define SET(var, mask)   ((var) |=  ((uint32_t) mask))
#define IS_SET(var, bit)  ((var) & (bit))

// RCC
#define xRCC_BASE            0x40021000           
#define xRCC_APB1RST_OFFSET  0x10
#define xRCC_APB1ENR_OFFSET  0x1C
#define xRCC_REG(offset)     (MMIO32((xRCC_BASE) + (offset)))
#define xRCC_APB1RST         (xRCC_REG(xRCC_APB1RST_OFFSET))
#define xRCC_APB1ENR         (xRCC_REG(xRCC_APB1ENR_OFFSET))

// CAN
#define xCAN_BASE            0x40006400
#define xCAN_MCR_OFFSET      0x00
#define xCAN_MSR_OFFSET      0x04
#define xCAN_BTR_OFFSET      0x1C
#define xCAN_FMR_OFFSET      0x200

#define xCAN_REG(offset)     (MMIO32((xCAN_BASE) + (offset)))
#define xCAN_MCR             (xCAN_REG((xCAN_MCR_OFFSET)))
#define xCAN_MSR             (xCAN_REG((xCAN_MSR_OFFSET)))
#define xCAN_BTR             (xCAN_REG((xCAN_BTR_OFFSET)))
#define xCAN_FMR             (xCAN_REG((xCAN_FMR_OFFSET)))	

// CAN Master Control Register
#define xCAN_INRQ  BIT0
#define xCAN_SLEEP BIT1
#define xCAN_TXFP  BIT2
#define xCAN_RFLM  BIT3
#define xCAN_NART  BIT4
#define xCAN_AWUM  BIT5
#define xCAN_ABOM  BIT6
#define xCAN_TTCM  BIT7
#define xCAN_RESET BIT15
#define xCAN_DBF   BIT16

// CAN Master Status Register
#define xCAN_INAK  BIT0
#define xCAN_SLAK  BIT1
#define xCAN_ERRI  BIT2
#define xCAN_WKUI  BIT3
#define xCAN_SLAKI BIT4
#define xCAN_TXM   BIT8
#define xCAN_RXM   BIT9
#define xCAN_SAMP  BIT10
#define xCAN_RX    BIT11

// CAN Bit Timing Register
#define xCAN_LBKM BIT30
#define xCAN_SILM BIT31

// CAN Filter Master Register
#define xCAN_FINIT BIT0

inline void x_can_enable_clock() {
	SET(xRCC_APB1ENR, xRCC_APB1_CAN_MASK);
}

inline void x_can_reset() {
	SET(xRCC_APB1RST, xRCC_APB1_CAN_MASK);
	delay_ms(1);
	CLEAR(xRCC_APB1RST, xRCC_APB1_CAN_MASK);
}

// TODO : proper wait w/ timeout
inline int x_can_go_init() {
	SET(xCAN_MCR, xCAN_INRQ);
	CLEAR(xCAN_MCR, xCAN_SLEEP);
	
	delay_ms(1);
	
	if(!IS_SET(xCAN_MSR, xCAN_INAK)){
		return -1;
	}
	
	return 0;
}

inline int x_can_leave_init() {
	CLEAR(xCAN_MCR, xCAN_INRQ);
	
	delay_ms(100);
	
	if(IS_SET(xCAN_MSR, xCAN_INAK)){ //  || IS_SET(xCAN_MSR, xCAN_SLAK)
		return -1;
	}

	return 0;
}

int celui_la_qui_s_init_pas_enculera_ses_morts_et_la_calotte_de_ses_morts () {
	// Set TXD to recessive state
	// done in gpio_setup

	// Start CAN peripheral
	x_can_enable_clock();
	x_can_reset();
	
	delay_ms(1);
	// Go to init state
	if(x_can_go_init() != 0){
		return -1;
	}

	// Config CAN options
	CLEAR(xCAN_MCR, xCAN_TTCM | xCAN_AWUM | xCAN_NART | xCAN_RFLM | xCAN_TXFP);
	SET(  xCAN_MCR, xCAN_ABOM);

	CLEAR(xCAN_BTR, xCAN_SILM | xCAN_LBKM);

	// Config bit timing
	xCAN_BTR |= 0x80 & 0b111111111;

	// Filters configuration
	// Just ensure the filters are no longer in init mode
	CLEAR(xCAN_FMR, xCAN_FINIT);
		
	// Try to leave init state
	if(x_can_leave_init() != 0){
		return -1;
	}
	
        return 0;
}

#define CAN_RECESSIVE 1
#define CAN_DOMINANT  0
void test_dominant_bit() {
	// Turn off the lights to do dirty stuff
	led_off();
	
	// Set TXD as dominant
	gpio_clear(GPIO_CAN_PORT, GPIO_CAN_TX_PIN);

	// Wait and hope for a dominant signal in RXD
	int timeout = 0xFFFFFF;
	while((--timeout) && (gpio_get(GPIO_CAN_PORT, GPIO_CAN_RX_PIN) != CAN_DOMINANT));

	// Clear the stuff so that the driver is happy
	gpio_set(GPIO_CAN_PORT, GPIO_CAN_TX_PIN);

	// Wait for recessive bit
	while((--timeout) && (gpio_get(GPIO_CAN_PORT, GPIO_CAN_RX_PIN) == CAN_DOMINANT));

	// Blink like a motherfucker if it worked
	if(timeout > 0) {
		led_on();
		delay_ms(50);
		led_off();
	}
}

int main(){
	clock_setup();
	gpio_setup();

	led_off();
	
	int r = celui_la_qui_s_init_pas_enculera_ses_morts_et_la_calotte_de_ses_morts();
	int speed = (r)?(100):(1000);

	while(eternity){
		delay_ms(speed);
		//led_toggle_status();
		test_dominant_bit();
		
	}
	
	return 0;
}
