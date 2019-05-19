# Project configuration makefile
default: all

# Project
PROJECT_ROOT_ABS  := $(dir $(realpath $(firstword $(MAKEFILE_LIST))))
# Absolute path can be used, but is potentially unsafe (think of rm
# -rf commands in clean targets !)
PROJECT_ROOT      := .
PROJECT_NAME      ?= $(notdir $(patsubst %/,%,$(dir $(PROJECT_ROOT_ABS))))
PROJECT_LIBS      := $(PROJECT_ROOT)/lib
PROJECT_MK        := $(PROJECT_ROOT)/mk
PROJECT_SRC       := $(PROJECT_ROOT)/src
PROJECT_BIN       := $(PROJECT_ROOT)/bin
PROJECT_LDSCRIPT  ?= $(PROJECT_ROOT)/stm32f303.ld
PROJECT_VERBOSITY ?= 0

CFLAGS   ?=
INCLUDES ?=
LDFLAGS  ?=
C_SRCS   =

# Libraries
include $(PROJECT_LIBS)/libopencm3.mk
# Adds libopencm3, libopencm3-clean targets
# Variables :
# OPENCM3_CDEFS                : Compiler defines required for opencm3
# OPENCM3_INCLUDES             : Headers of opencm3
# OPENCM3_LIB, OPENCM3_LIBNAME : path and name of opencm3
CFLAGS   += $(OPENCM3_CDEFS)
INCLUDES += -I$(OPENCM3_INC)
LDFLAGS  += -L$(OPENCM3_LIB) -l$(OPENCM3_LIBNAME)

# FreeRTOS
include $(PROJECT_MK)/freertos.mk
# Add :
# RTOS_INC : Headers for FreeRTOS
# RTOS_SRC : Sources for FreeRTOS
INCLUDES += $(RTOS_INC)
#C_SRCS   += $(RTOS_SRC)

# Project sources
# Add project subdirectories (with a depth of one max, though)
SRC_DIRS  = $(sort $(dir $(wildcard $(PROJECT_SRC)/*/)))
C_SRCS   += $(wildcard $(addsuffix *.c,$(SRC_DIRS)))
INCLUDES += $(addprefix -I,$(SRC_DIRS))

# Objects and includes
C_OBJS  = $(C_SRCS:.c=.o)
CFLAGS += $(INCLUDES)
LDFLAGS += \
	-Wl,--gc-sections \
	-nostartfiles \
	-lm \
	-lstdc++_nano \
	-lc \
	-lg \
	-lrdimon

# Objects dependencies
# This allow make to take track of modified header files
C_DEPS := $(C_OBJS:.o=.d)
-include $(C_DEPS)

# Linker script
LDFLAGS += -T$(PROJECT_LDSCRIPT)


# Toolchain
include $(PROJECT_MK)/toolchain.mk
include $(PROJECT_MK)/rules.mk

# Openocd configuration
OPENOCD_CFG = /usr/share/openocd/scripts/board/st_nucleo_f3.cfg



# Project build
all: libs $(PROJECT_NAME).hex

flash: $(PROJECT_NAME).flash
debug: $(PROJECT_NAME).debug

%.flash: %.hex
	openocd -f $(OPENOCD_CFG) \
		-c "init" \
		-c "reset init" \
		-c "flash write_image erase $^" \
		-c "reset" \
		-c "shutdown"

%.debug: %.elf
	$(GDB) $^ --command=gdb/attach.gdb

# Library targets
libs: libopencm3

# Project clean
clean:
	-rm -f $(C_OBJS) $(C_DEPS) $(PROJECT_NAME).elf $(PROJECT_NAME).hex

clean-libs: libopencm3-clean

cleanall: clean clean-libs
