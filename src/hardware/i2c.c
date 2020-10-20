#include "i2c.h"
#include "gpio.h"
#include "utils.h"
#include "task.h"

/**********************/
/* I2C initialization */
/**********************/
void i2c_setup()
{
    // I2C1
    i2c_peripheral_disable(I2C1);
    i2c_enable_analog_filter(I2C1); // filters up to 50 ns pulses
    i2c_set_speed(I2C1, i2c_speed_fm_400k, I2C1_CLK_MHZ);
    
    // set_speed fix : 125ns @8MHz, see RM0316 §28.4.9 Table 147
    i2c_set_data_hold_time(I2C1, 1);
    
    i2c_set_7bit_addr_mode(I2C1);
    i2c_enable_stretching(I2C1); // required for master mode (default)
    
    // Enable transceiver and peripheral
    gpio_set(GPIO_I2CEN_PORT, GPIO_I2CEN_PIN);
    i2c_peripheral_enable(I2C1);
}

/*********************/
/* I2C communication */
/*********************/

#define i2c_enable_reload(i2c) SET_BITS(I2C_CR2(i2c), I2C_CR2_RELOAD)
#define i2c_disable_reload(i2c) CLEAR_BITS(I2C_CR2(i2c), I2C_CR2_RELOAD)

enum I2C_Dir{
    I2C_READ = 0, I2C_WRITE = 1
};

/**
 * @brief Wait until TXIS or NACKF is set
 * 
 * @param i2c the peripheral address
 * @param start start tick for timeout reference
 * @param timeout maximum amount of time to wait
 * @return I2C_Status status code
 */
static I2C_Status i2c_wait_tx(uint32_t i2c, tick_t start, tick_t timeout);

/**
 * @brief Configure the I2C NBYTES value and the RELOAD bit, given the total
 * number of bytes left to transfer
 *
 * @param i2c the i2c peripheral address
 * @param n total number of bytes to transfer
 * @return uint8_t the NBYTES value which has been set
 */
static uint8_t i2c_set_nbytes(uint32_t i2c, size_t n);

/**
 * @brief Initializes a master transfer
 * 
 * @param i2c peripheral address
 * @param addr slave address
 * @param n bytes to transfer
 * @param dir transfer direction
 * @param stop enable or disable stop generation after transfer
 * @return uint8_t nbytes value
 */
static uint8_t i2c_master_start7(uint32_t i2c, uint8_t addr, size_t n, enum I2C_Dir dir, bool stop);

/**
 * @brief Wait for an I2C transfer to end
 * 
 * @param i2c peripheral address
 * @param stop true if stop generation is enabled, false otherwise
 * @param t_start timeout start reference
 * @param timeout timeout
 * @return I2C_Status status code
 */
static I2C_Status i2c_wait_end(uint32_t i2c, bool stop, tick_t t_start, tick_t timeout);

I2C_Status i2c_master_send7(uint32_t i2c, uint8_t addr,
                            uint8_t *wbuff, size_t wn,
                            tick_t timeout, bool stop)
{
    if(!wn) return I2C_OK;
    tick_t t_start = get_systick();
    
    // Init transfer
    uint8_t nbytes = i2c_master_start7(i2c, addr, wn, I2C_WRITE, stop);
    /*
     * What can go wrong now :
     * -> Arbitration lost
     * -> NACKF
     * -> Timeout (busy bus, may require reset)
     */
    do{
        // Send nbytes data
        for(int i = 0; i < nbytes; ++i){
            // Wait for TX buffer
            I2C_Status ret = i2c_wait_tx(i2c, t_start, timeout);
            if(ret != I2C_OK)
                return ret;
            
            // Write data
            i2c_send_data(i2c, wbuff[i]);
        }
        
        wbuff += nbytes;
        wn -= nbytes;
        if(wn){
            // Reload mode was set, wait for TCR flag
            if(!wait_flag(&I2C_ISR(i2c), I2C_ISR_TCR, SET, t_start, timeout))
                return I2C_ETIME;
            // Configure next transfer
            nbytes = i2c_set_nbytes(i2c, wn);
        }
    }while(wn);
    
    return i2c_wait_end(i2c, stop, t_start, timeout);
}

I2C_Status i2c_master_recv7(uint32_t i2c, uint8_t addr,
                               uint8_t *rbuff, size_t rn,
                               TickType_t timeout, bool stop)
{
    if(!rn) return I2C_OK;
    tick_t t_start = get_systick();
    
    // Init transfer
    uint8_t nbytes = i2c_master_start7(i2c, addr, rn, I2C_READ, stop);

    do{
        // Read nbytes of data
        for(int i = 0; i < nbytes; ++i){
            // Wait for RX buffer
            if(!wait_flag(&I2C_ISR(i2c), I2C_ISR_RXNE, SET, t_start, timeout)){
                return I2C_ETIME;
            }
            // Read data
            rbuff[i] = i2c_get_data(i2c);
        }
        
        rbuff += nbytes;
        rn -= nbytes;
        if(rn){
            // Reload mode was set, wait for TCR flag
            if(!wait_flag(&I2C_ISR(i2c), I2C_ISR_TCR, SET, t_start, timeout))
                return I2C_ETIME;
            // Configure next transfer
            nbytes = i2c_set_nbytes(i2c, rn);
        }
    }while(rn);
    
    return i2c_wait_end(i2c, stop, t_start, timeout);
}

I2C_Status i2c_master_transfer7(uint32_t i2c, uint8_t addr,
                                uint8_t *wbuff, size_t wn,
                                uint8_t *rbuff, size_t rn,
                                tick_t timeout)
{
    I2C_Status ret;
    ret = i2c_master_send7(i2c, addr, wbuff, wn, timeout, rn == 0);
    if(ret != I2C_OK)
        return ret;
    return i2c_master_recv7(i2c, addr, rbuff, rn, timeout, true);
}

static I2C_Status i2c_wait_tx(uint32_t i2c, tick_t start, tick_t timeout)
{
    while(!i2c_transmit_int_status(i2c)){
        if(i2c_nack(i2c)){
            // Don't wait anymore, TXIS won't be set
            I2C_ICR(i2c) = I2C_ICR_NACKCF;
            return I2C_ENACK;
        }else if(timeout == MAX_DELAY){
            // And I think it's gonna be a long long time
            continue;
        }else if((timeout == 0)
                 || ((get_systick() - start) > timeout)){
            return I2C_ETIME;
        }
    }
    
    return I2C_OK;
}

static uint8_t i2c_set_nbytes(uint32_t i2c, size_t n)
{
    uint8_t nbytes;
    if(n > I2C_MAX_NBYTES){
        nbytes = I2C_MAX_NBYTES;
        i2c_enable_reload(i2c);
    }else{
        nbytes = n;
        i2c_disable_reload(i2c);
    }
    i2c_set_bytes_to_transfer(i2c, n);
    
    return nbytes;
}

static uint8_t i2c_master_start7(uint32_t i2c, uint8_t addr, size_t n, enum I2C_Dir dir, bool stop)
{
    // 7-bit address mode is expected to be set
    i2c_set_7bit_address(i2c, addr);
    
    // Direction
    if(dir == I2C_READ){
        i2c_set_read_transfer_dir(i2c);
    }else{
        i2c_set_write_transfer_dir(i2c);
    }
    
    uint8_t nbytes = i2c_set_nbytes(i2c, n);
    
    // Clear any old NACK
    I2C_ICR(i2c) = I2C_ICR_NACKCF;
    
    i2c_send_start(i2c);
    /*
     * Checking for a busy bus condition before START is useless because the
     * BUSY flag could be set just after the read.
     */
     
    // Set autoend (for the last transfer)
    if(stop){
        i2c_enable_autoend(i2c);
    }else{
        i2c_disable_autoend(i2c);
    }
    /*
     * AUTOEND mode is set after start to avoid sending a stop if the user wants
     * a restart
     */
    
    return nbytes;
}

static I2C_Status i2c_wait_end(uint32_t i2c, bool stop, tick_t t_start, tick_t timeout)
{
    if(stop){
        // Wait stop flag
        if(!wait_flag(&I2C_ISR(i2c), I2C_ISR_STOPF, SET, t_start, timeout)){
            return I2C_ETIME;
        }
        I2C_ICR(i2c) = I2C_ICR_STOPCF;
    }else{
        // Wait transfer complete flag
        if(!wait_flag(&I2C_ISR(i2c), I2C_ISR_TC, SET, t_start, timeout)){
            return I2C_ETIME;
        }
        // The flag will be cleared with a future STOP or START
    }
    
    if(i2c_nack(i2c)){
        I2C_ICR(i2c) = I2C_ICR_NACKCF;
        return I2C_ENACK;
    }
    
    return I2C_OK;
}
