#ifndef I2C_H
#define I2C_H

#include <libopencm3/stm32/i2c.h>
#include "FreeRTOS.h"
#include "utils.h"

#define I2C1_CLK_MHZ    8
#define I2C_MAX_NBYTES 255

typedef enum I2C_Status_E{
    I2C_OK,
    I2C_ETIME, // timeout for unknown reason
    I2C_ENACK, // NACK
}I2C_Status;

/**
 * @brief Set the application-specific I2C configuration
 * @warning GPIOs must be initialized
 */
void i2c_setup();

/**
 * @brief Send data to an I2C device
 * 
 * @param i2c the I2C peripheral address
 * @param addr address of the destination device
 * @param wbuff data buffer to write
 * @param wn number of bytes to write
 * @param timeout maximum amount of ticks to wait
 * @param stop true to send STOP at the end, false otherwise
 * @return I2C_Status return status code
 */
I2C_Status i2c_master_send7(uint32_t i2c, uint8_t addr,
                             uint8_t *wbuff, size_t wn,
                             tick_t timeout, bool stop);

/**
 * @brief Read data from an I2C device
 * 
 * @param i2c the I2C peripheral address
 * @param addr address of the slave
 * @param rbuff data buffer
 * @param rn number of bytes to read
 * @param timeout maximum amount of ticks to wait
 * @param stop true to send STOP at the end, false otherwise
 * @return I2C_Status return status code
 */
I2C_Status i2c_master_recv7(uint32_t i2c, uint8_t addr,
                               uint8_t *rbuff, size_t rn,
                               tick_t timeout, bool stop);

/**
 * @brief Perform an I2C write followed by a read.
 * This function is useful for I2C memory accesses.
 * 
 * @param i2c the I2C peripheral address
 * @param addr the slave address
 * @param wbuff data buffer to write
 * @param wn number of bytes to write
 * @param rbuff data buffer for read
 * @param rn number of bytes to read
 * @param timeout maximum amount of ticks to wait
 * @return I2C_Status return status code
 */
I2C_Status i2c_master_transfer7(uint32_t i2c, uint8_t addr,
                                uint8_t *wbuff, size_t wn,
                                uint8_t *rbuff, size_t rn,
                                tick_t timeout);

#endif /* I2C_H */
