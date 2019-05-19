#ifndef CAN_H
#define CAN_H

#include <stdint.h>

int can_setup(void);
/**
 * Setup and start the bxCAN peripheral.
 * The CAN GPIOs must be already configured.
 */

/* User interrupt handlers */
void can_error_handler(uint32_t can_esr);
/**
 * See libopencm3/stm32/can.h for CAN_ESR values.
 */

/* CAN_ESR helpers */
typedef enum CAN_Error_Code_E{
  CAN_NO_ERROR,
  CAN_STUFF_ERROR,
  CAN_FORM_ERROR,
  CAN_ACK_ERROR,
  CAN_RECESSIVE_ERROR,
  CAN_DOMINANT_ERROR,
  CAN_CRC_ERROR,
  CAN_SOFT_ERROR // set by software
}CAN_Error_Code;
#define CAN_RX_ERROR_COUNTER(can_esr) ((uint8_t)(((can_esr) & CAN_ESR_REC_MASK) >> 24))
#define CAN_TX_ERROR_COUNTER(can_esr) ((uint8_t)(((can_esr) & CAN_ESR_TEC_MASK) >> 16))
#define CAN_ERROR_BUSOFF(can_esr)     ((uint8_t)(((can_esr) & CAN_ESR_BOFF)     >>  2))
#define CAN_ERROR_PASSIVE(can_esr)    ((uint8_t)(((can_esr) & CAN_ESR_EPVF)     >>  1))
#define CAN_ERROR_WARNING(can_esr)    ((uint8_t)(((can_esr) & CAN_ESR_EWGF)     >>  0))
#define CAN_LAST_ERROR_CODE(can_esr)  ((CAN_Error_Code) ((((can_esr) >> 2) & 0b111))

#endif
