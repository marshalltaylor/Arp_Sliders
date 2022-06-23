######################################
# simpleMake Makefile
# Requires compiler only
######################################

#debug
#openocd -f "E:\github\stm32f769i-disco_PROJECTs\Blinker\usbDebug.cfg" -c "init; reset halt"
# C:/STM32/gcc-arm-none-eabi/bin/arm-none-eabi-gdb build/output.elf
#target ext:3333

#######################################
# paths
#######################################
# Build stuff
BUILD_DIR = build
TARGET_EXEC = $(BUILD_DIR)/output.elf
LINKER_FILE = ../CubeProject/STM32F446RETx_FLASH.ld

# Tools
#GCC_PATH = /cygdrive/c/STM32/gcc-arm-none-eabi/bin
GCC_PATH = C:/STM32/gcc-arm-none-eabi/bin
CC = $(GCC_PATH)/arm-none-eabi-gcc
PP = $(GCC_PATH)/arm-none-eabi-g++
AS = $(GCC_PATH)/arm-none-eabi-as
CP = $(GCC_PATH)/arm-none-eabi-objcopy
SZ = $(GCC_PATH)/arm-none-eabi-size

######################################
# srcs/incs -- PROJECT
######################################
include ./Main.mk
# Provides:
#   HAL_C_SOURCES
#   HAL_C_INCLUDES
#   PROJECT_C_SOURCES
#   PROJECT_C_INCLUDES

######################################
# srcs/incs -- Arduino
######################################
include ./Arduino/ArduinoLib.mk
# Provides:
#   ARDUINO_LIB_C_SOURCES
#   ARDUINO_LIB_INCLUDES

include ./Arduino/ArduinoCore.mk
# Provides:
#   ARDUINO_CORE_C_SOURCES
#   ARDUINO_CORE_INCLUDES

HAL_INCLUDES = $(HAL_C_INCLUDES)
ARDUINO_INCLUDES = $(ARDUINO_CORE_INCLUDES) $(ARDUINO_LIB_INCLUDES) $(HAL_INCLUDES)
PROJECT_INCLUDES = $(PROJECT_C_INCLUDES) $(ARDUINO_INCLUDES)

#######################################
# FLAGS
#######################################
# c
#  -DUSE_HAL_DRIVER -DSTM32F7xx  '-D__weak=__attribute__((weak))' '-D__packed=__attribute__((__packed__))' -fmessage-length=0 -Og -ffunction-sections
CFLAGS = -v -mcpu=cortex-m4 -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16 -DSTM32F4xx -DSTM32F446xx -g -Wall -c

# Generate dependency information
CFLAGS += -MMD -MP -MF"$(@:%.o=%.d)"

# Message behavior
# -fsyntax-only -fmax-errors=1 -w -Werror 
CFLAGS += -fmax-errors=1

# cpp
CPPFLAGS = $(CFLAGS) -std=c++11 -fno-rtti

# assembler
ASFLAGS = -mcpu=cortex-m4 -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16 -g

# linker -lstdc++ -lsupc++ 
LDFLAGS = -v -lc -g -lm -mcpu=cortex-m4 -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16 -specs=nosys.specs -specs=nano.specs -T$(LINKER_FILE) -Wl,-Map=$(BUILD_DIR)/output.map -Wl,--gc-sections

# binaries
HEX = $(CP) -O ihex
BIN = $(CP) -O binary -S


#######################################
# build the PROJECTlication
#######################################
# default action: build all
all: make_directories $(TARGET_EXEC) $(BUILD_DIR)/output.hex $(BUILD_DIR)/output.bin

#PROJECT_OBJECTS
PROJECT_OBJECTS = $(addprefix $(BUILD_DIR)/PROJECT/,$(patsubst %s,%o,$(patsubst %c,%o,$(patsubst %cpp,%o,$(notdir $(PROJECT_C_SOURCES))))))
vpath %.c $(sort $(dir $(PROJECT_C_SOURCES)))
vpath %.cpp $(sort $(dir $(PROJECT_C_SOURCES)))
#vpath %.s $(sort $(dir $(PROJECT_C_SOURCES)))

#ARDUINO_OBJECTS
ARDUINO_SOURCES = $(ARDUINO_LIB_C_SOURCES) $(ARDUINO_CORE_C_SOURCES)
ARDUINO_OBJECTS = $(addprefix $(BUILD_DIR)/ARDUINO/,$(patsubst %c,%o,$(patsubst %cpp,%o,$(notdir $(ARDUINO_SOURCES)))))
vpath %.c $(sort $(dir $(ARDUINO_SOURCES)))
vpath %.cpp $(sort $(dir $(ARDUINO_SOURCES)))

#HAL_C_OBJECTS
HAL_OBJECTS = $(addprefix $(BUILD_DIR)/HAL/,$(patsubst %s,%o,$(patsubst %c,%o,$(patsubst %cpp,%o,$(notdir $(HAL_C_SOURCES))))))
vpath %.c $(sort $(dir $(HAL_C_SOURCES)))
#vpath %.cpp $(sort $(dir $(HAL_C_SOURCES)))
vpath %.s $(sort $(dir $(HAL_C_SOURCES)))

$(BUILD_DIR)/PROJECT/%.o: %.cpp
	@echo "***** ***** ***** input = $^, output = $@***** ***** *****"
	$(PP) $(CPPFLAGS) -Werror  $(addprefix -I,$(PROJECT_INCLUDES)) $< -o $@
	@echo
	@echo

$(BUILD_DIR)/PROJECT/%.o: %.c
	@echo "***** ***** ***** input = $^, output = $@***** ***** *****"
	$(CC) $(CFLAGS) -Werror  $(addprefix -I,$(PROJECT_INCLUDES))  $< -o $@
	@echo
	@echo

$(BUILD_DIR)/ARDUINO/%.o: %.cpp
	@echo "***** ***** ***** input = $^, output = $@***** ***** *****"
	$(PP) $(CPPFLAGS) $(addprefix -I,$(ARDUINO_INCLUDES)) $< -o $@
	@echo
	@echo

$(BUILD_DIR)/ARDUINO/%.o: %.c
	@echo "***** ***** ***** input = $^, output = $@***** ***** *****"
	$(CC) $(CFLAGS) $(addprefix -I,$(ARDUINO_INCLUDES)) $< -o $@
	@echo
	@echo

$(BUILD_DIR)/HAL/%.o: %.c
	@echo "***** ***** ***** input = $^, output = $@***** ***** *****"
	$(CC) $(CFLAGS) $(addprefix -I,$(HAL_INCLUDES)) $< -o $@
	@echo
	@echo

$(BUILD_DIR)/HAL/%.o: %.s
	@echo "***** ***** ***** input = $^, output = $@***** ***** *****"
	$(AS) $(ASFLAGS) $(addprefix -I,$(HAL_INCLUDES)) $< -o $@
	@echo
	@echo

# list of all objects
OBJECTS = \
$(PROJECT_OBJECTS) \
$(ARDUINO_OBJECTS) \
$(HAL_OBJECTS)

%.d: ;

include $(patsubst %,%.d,$(basename $(OBJECTS)))

# linker
$(BUILD_DIR)/output.elf: $(OBJECTS) Makefile
	$(PP) $(LDFLAGS) $(OBJECTS) -o $(TARGET_EXEC)
	$(SZ) $@
	@echo
	@echo

$(BUILD_DIR)/output.hex: $(TARGET_EXEC)
	$(HEX) $< $@
	
$(BUILD_DIR)/output.bin: $(TARGET_EXEC)
	$(BIN) $< $@	

.PHONY: make_directories

make_directories:
	mkdir -p $(BUILD_DIR)/PROJECT
	mkdir -p $(BUILD_DIR)/ARDUINO
	mkdir -p $(BUILD_DIR)/HAL

info:
	@echo "***** ***** ***** PROJECT ***** ***** *****"
	@echo PROJECT_C_SOURCES
	@echo $(PROJECT_C_SOURCES)
	@echo
	@echo PROJECT_OBJECTS
	@echo $(PROJECT_OBJECTS)
	@echo
	@echo PROJECT_INCLUDES
	@echo $(PROJECT_INCLUDES)
	@echo
	@echo "***** ***** ***** ARDUINO ***** ***** *****"
	@echo ARDUINO_SOURCES
	@echo $(ARDUINO_SOURCES)
	@echo
	@echo ARDUINO_OBJECTS
	@echo $(ARDUINO_OBJECTS)
	@echo
	@echo ARDUINO_INCLUDES
	@echo $(ARDUINO_INCLUDES)
	@echo
	@echo "***** ***** ***** HAL ***** ***** *****"
	@echo HAL_OBJECTS
	@echo $(HAL_OBJECTS)
	@echo
	@echo HAL_C_INCLUDES
	@echo $(HAL_C_INCLUDES)
	@echo
	@echo "***** ***** ***** OBJECTS ***** ***** *****"
	@echo $(OBJECTS)
	@echo
	@echo "***** ***** ***** WHATISTHIS ***** ***** *****"
	@echo $(WHATISTHIS)
	@echo
#######################################
# clean up
#######################################
clean:
	-rm -fR $(BUILD_DIR)
  
#######################################
# dependencies
#######################################
-include $(wildcard $(BUILD_DIR)/*.d)

#debug
# openocd -f "E:\github\stm32f769i-disco_PROJECTs\usbDebugTemplate\AudioTemplate Debug.cfg"
# C:/STM32/gcc-arm-none-eabi/bin/arm-none-eabi-gdb build/output.elf

# *** EOF ***