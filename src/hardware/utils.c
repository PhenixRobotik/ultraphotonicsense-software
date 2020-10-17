#include "utils.h"

static inline bool check_flag(uint32_t val, uint32_t flag, bool set)
{
    bool is_set = (val & flag)?true:false;
    return set?is_set:(!is_set);
    // This isn't the fastest way but nobody cares, we're doing active wait.
}

bool wait_flag(volatile uint32_t *reg, uint32_t flag, bool set, tick_t t_start, tick_t timeout)
{
    while(!check_flag(*reg, flag, set)){
        if(timeout == MAX_DELAY)
            continue;
        else if((timeout == 0)
                || ((get_systick() - t_start) > timeout))
            return false;
    }
    
    return true;
}

/**
 * Note on timeout management :
 * - Waiting for ticks is a lot of time, but these functions are waiting on
 *   hardware peripherals which *should* set their flags on microseconds. Losing
 *   some ticks under specific circumstances can be acceptable. Higher level
 *   software should ensure that this can't occurs too frequently.
 * - The choice of '>' comparison guarantees that a timeout of one tick would
 *   wait for at least one tick.
 * - The subtraction of unsigned timestamps is computed modulo (UINT32_MAX + 1),
 *   which handles the case 'a < b' because the real tick difference shouldn't
 *   exceed UINT32_MAX between two checks.
 * - Finally, providing the start timestamp instead of an updated timeout is
 *   necessary to disambiguate the case "no timeout" and "one more tick to wait"
 *   (the updated tick count would be 0).
 */
 