#ifndef SHIFT_REG_H
#define SHIFT_REG_H

#include <stdint.h>
#include <stdbool.h>

#define SHIFTREG_GPIO_DATA_PORT  GPIO_TOF_RESET_PORT
#define SHIFTREG_GPIO_DATA_PIN   GPIO_TOF_RESET_DATA_PIN
#define SHIFTREG_GPIO_SHIFT_PORT GPIO_TOF_RESET_PORT
#define SHIFTREG_GPIO_SHIFT_PIN  GPIO_TOF_RESET_SHIFT_PIN

void shiftreg_shift(bool bit);


uint8_t shiftreg_value();
/**
 * Not a physical read, deduced by shift commands.
 */

#endif
