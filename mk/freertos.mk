# Add :
# RTOS_INC : Headers for FreeRTOS
# RTOS_SRC : Sources for FreeRTOS
ifndef PROJECT_LIBS
$(error PROJECT_LIBS is not set)
endif

RTOS_DIR     := $(PROJECT_LIBS)/FreeRTOS

# Configuration
RTOS_PORT    := $(RTOS_DIR)/portable/GCC/ARM_CM4F
RTOS_MEMMANG := $(RTOS_DIR)/portable/MemMang
RTOS_HEAP    := 1

# Includes
RTOS_INC  := $(RTOS_DIR)/include
RTOS_INC  += $(RTOS_PORT)
RTOS_INC  := $(addprefix -I,$(RTOS_INC))

# Sources
RTOS_SRC  := $(wildcard $(RTOS_DIR)/*.c)
RTOS_SRC  += $(RTOS_PORT)/port.c
#RTOS_SRC  += $(RTOS_MEMMANG)/heap_$(RTOS_HEAP).c

# Edit global variables
INCLUDES += $(RTOS_INC)
C_SRCS   += $(RTOS_SRC)
