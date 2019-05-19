#include "can.h"

#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/f3/nvic.h>
#include <libopencm3/stm32/can.h>

static void can_enable_irqs();

int can_setup(void){
  /* Enable bxCAN clock */
  rcc_periph_clock_enable(RCC_CAN1);
  
  /* Reset bxCAN */
  can_reset(CAN1);

  /* Init and start CAN */
  /**
   * Note on the bitrate :
   * Given the frequency of APB1 (f), we have :
   * bitrate = f / (nb_tq * prescaler)
   * So if f = 32MHz, and we have 16 TQ, a prescaler of 4 would mean 500 kbps
   */
  int err = can_init(CAN1,
		     false, // Time-triggered communication mode
		     true,  // Automatically recover from bus-off state
		     false, // Automatically wake up from sleep mode
		     false, // No automatic retransmission (false = automatic)
		     false, // FIFO lock function
		     false, // FIFO priority mode (false = id priority mode)
		     CAN_BTR_SJW_1TQ,
		     CAN_BTR_TS1_9TQ,
		     CAN_BTR_TS2_6TQ,
		     4,
		     false, // Silent mode
		     false  // Loopback mode
    );
  // TODO : baud rate enum ?
  // TODO : add parameters for baud rate
  
  if(err){
    return -1;
  } else {
    // be happy :)
  }

  /* CAN filter 0 init */
  can_filter_id_mask_32bit_init(
    0,     /* Filter ID */
    0,     /* CAN ID */
    0,     /* CAN ID mask */
    0,     /* FIFO assignment (here: FIFO0) */
    true); /* Enable the filter. */
  // TODO : understand wtf this does

  /* Enable interrupts */
  can_enable_irqs();
  
  return 0;
}

static void can_enable_irqs() {
  /* Setup NVIC */
  // RX FIFO 0 interrupt
  nvic_enable_irq(NVIC_USB_LP_CAN1_RX0_IRQ);
  nvic_set_priority(NVIC_USB_LP_CAN1_RX0_IRQ, 1);

  // Status Change and Errors interrupt
  nvic_enable_irq(NVIC_CAN1_SCE_IRQ);
  nvic_set_priority(NVIC_CAN1_SCE_IRQ, 1);

  // TODO : handlers
  // TODO : check priority relevance
  /**
   * Other interrupts are :
   * NVIC_USB_HP_CAN1_TX_IRQ for TX events
   * NVIC_CAN1_RX1_IRQ for RX fifo 1 events
   *
   * Note : needed events must also be enabled in the peripheral.
   */

  // Trigger an interrupt whenever a new message is available in RX FIFO 0
  can_enable_irq(CAN1, CAN_IER_FMPIE0);

  // Interrupt when an error is set in the Error Status Register
  can_enable_irq(CAN1, CAN_IER_ERRIE);
}

/* System interrupt handlers */
void usb_hp_can1_tx_isr(void) {
  // TODO
}

void usb_lp_can1_rx0_isr(void) {
  
}

void can1_rx1_isr(void) {
  // TODO
}

void can1_sce_isr(void) {
  can_error_handler(CAN_ESR(BX_CAN1_BASE));
}

/* User interrupt handlers */
__attribute__((weak)) void can_error_handler(uint32_t can_esr);
