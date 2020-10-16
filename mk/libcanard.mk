ifndef PROJECT_LIBS
$(error PROJECT_LIBS is not set)
endif

CANARD_DIR	:= $(PROJECT_LIBS)/libcanard
CANARD_INC	:= $(CANARD_DIR)/libcanard
CANARD_SRC	:= $(CANARD_DIR)/libcanard/canard.c

# Edit global variables
INCLUDES += -I$(CANARD_INC)
C_SRCS   += $(CANARD_SRC)
