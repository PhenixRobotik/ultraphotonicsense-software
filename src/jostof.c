#include "jostof.h"

#include "rcc.h"
#include "tof.h"
#include "shift_reg.h"
#include "vl53l0x_api.h"

ToF_Handler tof[NB_MAX_TOF];

static int init_tof(ToF_Handler *htof, uint8_t i2c_addr);

void init_jostof() {
  // Discover all connected ToFs and start them
  int i;
  for(i = 0; i < NB_MAX_TOF; ++i){
    // Enable ToF_i
    shiftreg_shift(1);

    // Wait for the chip to start
    delay_ms(TOF_DELAY);

    // Init ToF
    if(init_tof(&tof[i], TOF_FIRST_ADDR + 2*(i+1)) == -1){
      tof[i].dev.Present = 0;
    }
  }
}

static int init_tof(ToF_Handler *htof, uint8_t i2c_addr){
  ToF_Params params;

  ToF_Init_Struct(htof);
 
  //Check the initial address
  if(ToF_Poke(&htof->dev) == -1)
    return -1;

  //Set the new address
  if(ToF_Set_Address(&htof->dev, i2c_addr) == -1)
    return -1;
    
  //Init the device
  if(ToF_Init_Device(&htof->dev) == -1)
    return -1;

  //Long range config
  TOF_LONG_RANGE_CONFIG(params);

  //Configure the device
  if(ToF_Configure_Device(&htof->dev, &params, VL53L0X_DEVICEMODE_CONTINUOUS_RANGING)== -1)
    return -1;

  //Start the device
  if(VL53L0X_StartMeasurement(&htof->dev) == -1)
    return -1;

  return 0;
}
