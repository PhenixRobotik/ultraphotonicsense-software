ifndef PROJECT_LIBS
$(error PROJECT_LIBS is not set)
endif

O1HEAP_DIR	:= $(PROJECT_LIBS)/o1heap
O1HEAP_INC	:= $(O1HEAP_DIR)/o1heap
O1HEAP_SRC	:= $(O1HEAP_DIR)/o1heap/o1heap.c

# Edit global variables
INCLUDES += -I$(O1HEAP_INC)
C_SRCS   += $(O1HEAP_SRC)
