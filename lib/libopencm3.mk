# Adds libopencm3, libopencm3-clean targets
# Variables :
# OPENCM3_CDEFS                : Compiler defines required for opencm3
# OPENCM3_INCLUDES             : Headers of opencm3
# OPENCM3_LIB, OPENCM3_LIBNAME : path and name of opencm3
ifndef PROJECT_LIBS
$(error PROJECT_LIBS is not set)
endif

# Paths
OPENCM3_DIR    := $(PROJECT_LIBS)/libopencm3
OPENCM3_LIB    := $(OPENCM3_DIR)/lib
OPENCM3_INC    := $(OPENCM3_DIR)/include

# Target selection
OPENCM3_LIBNAME   = opencm3_stm32f3
OPENCM3_TARGETS   = stm32/f3
OPENCM3_CDEFS     = -DSTM32F3
OPENCM3_LDFLAGS   = -L$(OPENCM3_LIB) -l$(OPENCM3_LIBNAME)
OPENCM3_VERBOSITY = 0

# Build library if not found
libopencm3: $(OPENCM3_LIB)/lib$(OPENCM3_LIBNAME).a

$(OPENCM3_LIB)/lib$(OPENCM3_LIBNAME).a:
ifeq (,$(wildcard $@))
	$(warning $(OPENCM3_LIBNAME).a not found, attempting to rebuild in $(OPENCM3_DIR))
	$(MAKE) -C $(OPENCM3_DIR) -j TARGETS=$(OPENCM3_TARGETS) V=$(OPENCM3_VERBOSITY)
endif

# Clean library
libopencm3-clean:
	$(MAKE) -C $(OPENCM3_DIR) clean

