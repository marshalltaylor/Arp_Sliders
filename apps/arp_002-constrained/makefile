# STM32F4xx bsp uart test
REPO_ROOT := ../..
SYNTH_COMMON_ROOT := $(REPO_ROOT)/synth_common

C_SOURCES =  \
globals.cpp \
customMath.cpp \
midiHandlers.cpp \
main.c \
debugKeyParser.cpp \
rtos/os.c \
rtos/taskConsole.cpp \
rtos/taskLog.cpp \
rtos/taskSystem.cpp \
rtos/taskMVC.cpp \
mvc/model.cpp \
mvc/view.cpp \
mvc/controller.cpp \
mvc/customPanelComponents.cpp \
mvc/customHardwareInterfaces.cpp \
mvc/commands/commands.cpp \
mvc/commands/logControl.cpp \
mvc/commands/midiTest.cpp \
mvc/commands/rtosTest.cpp \
mvc/commands/spiTest.cpp \
mvc/commands/adcTest.cpp \
mvc/commands/settingsEditor.cpp \

SOURCE_DIRS = . \
rtos \
mvc \
mvc/commands \

# TODO: Not all these should be included maybe
# TODO: FreeRTOSConfig.h location?
C_INCLUDES = . \
rtos \
mvc \
mvc/commands \
$(SYNTH_COMMON_ROOT)/FreeRTOS/Source/portable/GCC/ARM_CM4F \
$(SYNTH_COMMON_ROOT)/FreeRTOS \
$(SYNTH_COMMON_ROOT)/FreeRTOS/Source/include \
$(REPO_ROOT)/bsp \
$(SYNTH_COMMON_ROOT)/modules/logging \
$(SYNTH_COMMON_ROOT)/modules/midi47fx \
$(SYNTH_COMMON_ROOT)/modules/uCModules \


STATIC_LIBS = \
$(SYNTH_COMMON_ROOT)/FreeRTOS/build/FreeRTOS.a \
$(SYNTH_COMMON_ROOT)/modules/logging/build/logging.a \
$(SYNTH_COMMON_ROOT)/modules/midi47fx/build/midi47fx.a \
$(SYNTH_COMMON_ROOT)/modules/uCModules/build/uCModules.a \
$(REPO_ROOT)/bsp/build/bsp.a


# objects
OBJECTS = $(patsubst %c,%o,$(patsubst %cpp,%o,$(C_SOURCES)))

# rules
default: all

include $(SYNTH_COMMON_ROOT)/tools/build/catalog.mk

include $(SYNTH_COMMON_ROOT)/tools/build/common.mk

# linker
$(BUILD_DIR)/output.elf: build_dirs $(addprefix $(BUILD_DIR)/obj/, $(OBJECTS))
	@echo "#### Linking into elf"
	$(PP) -o $(TARGET_EXEC) $(LDFLAGS) -std=c++11 $(addprefix $(BUILD_DIR)/obj/, $(OBJECTS)) $(STATIC_LIBS)
	$(SZ) $@
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
	@echo $(addprefix $(BUILD_DIR)/obj/, $(OBJECTS))
	@echo $(C_SOURCES) 
