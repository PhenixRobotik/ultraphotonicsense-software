#include "jostof.h"

#include <libopencm3/cm3/cortex.h>
#include "rcc.h"
#include "tof.h"
#include "shift_reg.h"
#include "vl53l0x_api.h"
#include "rc_server.h"

ToF_Handler tof[NB_MAX_TOF];
volatile int tof_val[NB_MAX_TOF];

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

    tof_val[i] = 0;
  }
}

void update_jostof() {
  int i;
  for(i = 0; i < NB_MAX_TOF; ++i){
    ToF_Poll_Measurement_Data(&tof[i]);
  }
  cm_disable_interrupts();
  for(i = 0; i < NB_MAX_TOF; ++i){
    tof_val[i] = ToF_Get_Last_Range(&tof[i]);
  }
  cm_enable_interrupts();
}

void get_tof_data(RC_Server *server){
  RC_Server_Return(server,
		   tof_val[0],
		   tof_val[1],
		   tof_val[2],
		   tof_val[3]);
}

extern I2C_HandleTypeDef hi2c1;
static int init_tof(ToF_Handler *htof, uint8_t i2c_addr){
  ToF_Params params;

  ToF_Init_Struct(htof, &hi2c1);
 
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

