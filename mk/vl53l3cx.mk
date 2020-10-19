ifndef PROJECT_LIBS
$(error PROJECT_LIBS is not set)
endif

VL53L3CX_DIR	:= $(PROJECT_LIBS)/vl53l3cx_api
VL53L3CX_CDEFS	:= -DUSE_I2C_2V8
VL53L3CX_INC	:= $(VL53L3CX_DIR)/inc
VL53L3CX_SRC	:= $(wildcard $(VL53L3CX_DIR)/src/*.c)

# Edit global variables
CFLAGS   += $(VL53L3CX_CDEFS)
INCLUDES += -I$(VL53L3CX_INC)
C_SRCS   += $(VL53L3CX_SRC)