#include "hc_sr04.h"

#include "hardware/gpio.h"
#include "hardware/timer.h"

#define ULTRASONIC_CM_PER_USEC (1e2*340./1e6/2.)

void us_sensor_init()
{
    gpio_clear(GPIO_US_PORT, GPIO_US_TRIG_PINS);
    gpio_set(GPIO_US_OE_PORT, GPIO_US_OE_PIN);
}

void us_sensor_trig()
{
    gpio_set(GPIO_US_PORT, GPIO_US_TRIG_PINS);
    usleep(10);
    gpio_clear(GPIO_US_PORT, GPIO_US_TRIG_PINS);
}
