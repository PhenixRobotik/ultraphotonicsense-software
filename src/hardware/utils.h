#ifndef UTILS_H
#define UTILS_H

#include "FreeRTOS.h"
#include "task.h"

#define CLEAR_BITS(var, mask) ((var) &= ~((uint32_t) mask))
#define SET_BITS(var, mask)   ((var) |=  ((uint32_t) mask))
#define IS_SET(var, bit) ((var) & (bit))

#define get_systick() xTaskGetTickCount()
#define MAX_DELAY portMAX_DELAY

typedef TickType_t tick_t;
typedef volatile uint32_t reg_t;

#define RESET 0
#define SET 1

/**
 * @brief Wait for a flag to be set or reset on a register.
 * 
 * @param reg pointer on the register
 * @param flag the flag to wait for
 * @param set wait for the flag to be set if true, reset otherwise
 * @param t_start timestamp reference for timeout
 * @param timeout timeout
 * @return true if the condition is satisfied
 * @return false if the condition is not satisfied and timeout occured
 */
bool wait_flag(reg_t *reg, uint32_t flag, bool set, tick_t t_start, tick_t timeout);

#endif /* UTILS_H */
