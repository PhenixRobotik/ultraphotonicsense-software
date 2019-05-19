## This file is inspired by the libopencm3 project,
## whose licence information is pasted below

## <BEGIN OF LIBOPENCM3 LICENCE>
##
## This file is part of the libopencm3 project.
##
## Copyright (C) 2014 Frantisek Burian <BuFran@seznam.cz>
##
## This library is free software: you can redistribute it and/or modify
## it under the terms of the GNU Lesser General Public License as published by
## the Free Software Foundation, either version 3 of the License, or
## (at your option) any later version.
##
## This library is distributed in the hope that it will be useful,
## but WITHOUT ANY WARRANTY; without even the implied warranty of
## MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
## GNU Lesser General Public License for more details.
##
## You should have received a copy of the GNU Lesser General Public License
## along with this library.  If not, see <http://www.gnu.org/licenses/>.
##
## <END OF LIBOPENCM3 LICENCE>

# Target
FP_FLAGS   ?=             \
	-mfloat-abi=hard  \
	-mfpu=fpv4-sp-d16
ARCH_FLAGS :=           \
	--specs=nosys.specs \
	-mthumb         \
	-mcpu=cortex-m4 \
	$(FP_FLAGS)

# CFLAGS
CFLAGS += \
	-std=c99 \
	-g \
	-Og \
	-fdiagnostics-color=always \
	-Wall \
	-Wextra
#	-ffunction-sections \
#	-fdata-sections \
#	-fno-common \
#	--static

CXXFLAGS = \
	-fno-exceptions

ifneq ($(PROJECT_VERBOSITY),1)
Q := @
# Do not print "Entering directory ...".
MAKEFLAGS += --no-print-directory
endif

%.bin: %.elf
	@printf "  OBJCOPY $@\n"
	$(Q)$(OBJCOPY) -Obinary $< $@

%.hex: %.elf
	@printf "  OBJCOPY $@\n"
	$(Q)$(OBJCOPY) -Oihex $< $@

%.srec: %.elf
	@printf "  OBJCOPY $@\n"
	$(Q)$(OBJCOPY) -Osrec $< $@

%.list: %.elf
	@printf "  OBJDUMP $@\n"
	$(Q)$(OBJDUMP) -S $< > $@

%.elf: $(C_OBJS) $(LDSCRIPT) $(LIBDEPS)
	@printf "  LD      $(*).elf\n"
	$(Q)$(LD) $(C_OBJS) $(LDLIBS) $(LDFLAGS) $(ARCH_FLAGS)  -o $@

%.o: %.c
	@printf "  CC      $<\n"
	$(Q)$(CC) $(CFLAGS) $(ARCH_FLAGS) -o $@ -c $<
	$(Q)$(CC) -MM -MP -MT$@ $(CFLAGS) $*.c > $*.d

%.o: %.cpp
	@printf "  CXX     $(*).cpp\n"
	$(Q)$(CXX) $(CXXFLAGS) $(ARCH_FLAGS) -o $@ -c $<
	$(Q)$(CXX) -MM -MP -MT$(dir $*) $(CXXFLAGS) $*.cpp > $*.d
