# STM32F4xx bsp test
REPO_ROOT := ../..
SYNTH_COMMON_ROOT := $(REPO_ROOT)/synth_common


ifeq ($(strip $(board)),)
ifneq ('$(MAKECMDGOALS)','clean')
$(info no board specified, setting to dot2)
board = dot2
endif
endif

C_SOURCES =  \
main.c

SOURCE_DIRS = . \

C_INCLUDES = \
$(REPO_ROOT)/bsp \
$(REPO_ROOT)/bsp/UserProject/Core/Inc \
$(REPO_ROOT)/bsp/CubeProject/Drivers/STM32F4xx_HAL_Driver/Inc \
$(REPO_ROOT)/bsp/CubeProject/Drivers/CMSIS/Include \
$(REPO_ROOT)/bsp/CubeProject/Drivers/CMSIS/Device/ST/STM32F4xx/Include \
$(REPO_ROOT)/bsp/UserProject/AltDrivers

STATIC_LIBS = \
$(REPO_ROOT)/bsp/build/bsp_$(board).a

# objects
OBJECTS = $(patsubst %c,%o,$(patsubst %cpp,%o,$(C_SOURCES)))

# rules
default: all

include $(SYNTH_COMMON_ROOT)/tools/build/catalog.mk

include $(SYNTH_COMMON_ROOT)/tools/build/common.mk

# linker
$(BUILD_DIR)/output.elf: build_dirs $(addprefix $(BUILD_DIR)/obj/, $(OBJECTS))
	@echo "#### Linking into elf"
	$(PP) -o $(TARGET_EXEC) $(LDFLAGS) $(addprefix $(BUILD_DIR)/obj/, $(OBJECTS)) $(STATIC_LIBS)
	$(SZ) $@
	@echo ""
	@echo "Built for $(board) board"
	@echo ""
	$(OBJD) -S --disassemble $(TARGET_EXEC) > $(TARGET_EXEC).dump
	@echo

$(BUILD_DIR)/output.hex: build_dirs $(TARGET_EXEC)
	@echo "#### Elf to hex"
	$(HEX) $< $@
	@echo

$(BUILD_DIR)/output.bin: build_dirs $(TARGET_EXEC)
	@echo "#### Elf to bin"
	$(BIN) $< $@
	@echo

.PHONY: $(STATIC_LIBS)

all: $(STATIC_LIBS) $(TARGET_EXEC)

clean:: clean_all

openocd:
	cygstart cmd /k $(shell cygpath -w $(PATH_OPENOCD)/launch_openocd.bat)

gdb:
	@echo ""
	@echo "gdb needs special ctrl-c treatment."
	@echo "Launch cmd.exe from start and enter these commands:"
	@echo ""
	@echo "E:"
	@echo "cd $(shell cygpath -w $(shell pwd))"
	@echo "$(shell cygpath -w $(GDB)) -x gdbinit.txt build/output.elf"

info:
	@echo "***** ***** ***** DEBUG ***** ***** *****"
	@echo $(SOURCE_OBJECTS) $(STARTUP_OBJECT)
	@echo $(OBJECTS)
