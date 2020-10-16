ifndef PROJECT_LIBS
$(error PROJECT_LIBS is not set)
endif

VL53L0X_DIR		:= $(PROJECT_LIBS)/vl53l0x_api
VL53L0X_CDEFS	:= -DUSE_I2C_2V8
VL53L0X_INC		:= $(VL53L0X_DIR)/inc
VL53L0X_SRC		:= $(wildcard $(VL53L0X_DIR)/src/*.c)

# Edit global variables
CFLAGS   += $(VL53L0X_CDEFS)
INCLUDES += -I$(VL53L0X_INC)
C_SRCS   += $(VL53L0X_SRC)
