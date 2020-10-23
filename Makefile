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
V ?= 0

CFLAGS   ?=
INCLUDES ?=
LDFLAGS  ?=
C_SRCS   =

# Project dependencies
include $(PROJECT_MK)/libopencm3.mk
include $(PROJECT_MK)/libcanard.mk
include $(PROJECT_MK)/freertos.mk
include $(PROJECT_MK)/o1heap.mk
include $(PROJECT_MK)/vl53l0x.mk

# Project sources
# Add project subdirectories (with a depth of one max, though)
#SRC_DIRS  = $(sort $(dir $(wildcard $(PROJECT_SRC)/*/)))
SRC_DIRS = src/board src/hardware src/tests src/vl53l0x
SRC_DIRS += $(PROJECT_SRC)
C_SRCS   += $(wildcard $(addsuffix /*.c,$(SRC_DIRS)))
INCLUDES += $(addprefix -I,$(SRC_DIRS))

# Objects and includes
C_OBJS  = $(C_SRCS:.c=.o)
CFLAGS += $(INCLUDES)
LDFLAGS += \
	--specs=nano.specs \
	-Wl,--gc-sections \
	-Wl,--print-memory-usage \
	-nostartfiles \
	-lm \
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

# Project build
all: libs $(PROJECT_NAME).hex

include $(PROJECT_MK)/bmp.mk

flash: $(PROJECT_NAME).flash
debug: $(PROJECT_NAME).debug

# Library targets
libs: libopencm3

# Project clean
clean:
	-rm -f $(C_OBJS) $(C_DEPS) $(PROJECT_NAME).elf $(PROJECT_NAME).hex

clean-libs: libopencm3-clean

cleanall: clean clean-libs
