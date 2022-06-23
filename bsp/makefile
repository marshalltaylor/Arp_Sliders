# STM32F4xx bsp
REPO_ROOT = ..
SYNTH_COMMON_ROOT := $(REPO_ROOT)/synth_common

C_SOURCES_COMMON := \
src/bspSys.c \
src/bspIO.c \
src/bspSerial.c \
src/bspUart.c \
src/bspTime.c \
src/bspADC.c \
src/bspPins.c \
src/bspSPI.c

#src/bspDAC.c \

# Saved: I think this might be useful for forcing

# allow no board for clean target
#else ifeq ('$(MAKECMDGOALS)','clean')
#C_SOURCES_BOARD :=

#else
## Stop
#$(error board variable not defined!  ex 'make board=dot2')
#endif

C_SOURCES = $(C_SOURCES_COMMON) $(C_SOURCES_BOARD) \
UserProject/Src/main_cubemx.c \
UserProject/Src/gpio.c \
UserProject/Src/adc.c \
UserProject/Src/debugUtilities.c \
UserProject/Src/dma.c \
UserProject/Src/spi.c \
UserProject/Src/tim.c \
UserProject/Src/usart.c \
UserProject/Src/stm32f4xx_it.c \
UserProject/Src/stm32f4xx_hal_msp.c \
UserProject/Src/unused_int.c \
CubeProject/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_adc.c \
CubeProject/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_adc_ex.c \
UserProject/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_dac.c \
CubeProject/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_dac_ex.c \
CubeProject/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_tim.c \
CubeProject/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_tim_ex.c \
CubeProject/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_rcc.c \
CubeProject/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_rcc_ex.c \
CubeProject/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_flash.c \
CubeProject/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_flash_ex.c \
CubeProject/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_flash_ramfunc.c \
CubeProject/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_gpio.c \
CubeProject/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_dma_ex.c \
UserProject/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_dma.c \
CubeProject/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_pwr.c \
CubeProject/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_pwr_ex.c \
CubeProject/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_cortex.c \
CubeProject/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal.c \
UserProject/Src/system_stm32f4xx.c \
UserProject/Src/stm32f4xx_hal_timebase_TIM.c \
CubeProject/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_ll_usart.c \
CubeProject/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_ll_gpio.c \
CubeProject/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_ll_dma.c \
CubeProject/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_ll_rcc.c \
CubeProject/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_spi.c \
CubeProject/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_ll_utils.c \
CubeProject/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_ll_exti.c \
src/startup_stm32f446xx.s

#UserProject/AltDrivers/custom_stm32f4xx_hal_uart.c \

# The startup file above ***MUST*** be last in the list for weak linking
# The output archive of the bsp should also be linked last

# List of directories which contain C_SOURCES.
# This list is created within the BUILD_DIR and used for artifacts
SOURCE_DIRS = .\
src \
CubeProject/Drivers/STM32F4xx_HAL_Driver/Src \
UserProject/Drivers/STM32F4xx_HAL_Driver/Src \
UserProject/Src

C_INCLUDES = .\
src \
UserProject/Drivers/STM32F4xx_HAL_Driver/Inc \
UserProject/Inc \
CubeProject/Drivers/STM32F4xx_HAL_Driver/Inc \
CubeProject/Drivers/CMSIS/Include \
CubeProject/Drivers/CMSIS/Device/ST/STM32F4xx/Include

#PROJECT_OBJECTS
OBJECTS = $(patsubst %s,%o,$(patsubst %c,%o,$(patsubst %cpp,%o,$(C_SOURCES))))

OUTPUT_LIB_NAME = bsp.a

default: all

include $(SYNTH_COMMON_ROOT)/tools/build/common.mk

all: $(BUILD_DIR)/$(OUTPUT_LIB_NAME)

info:
	@echo "***** ***** ***** DEBUG ***** ***** *****"
#	@echo $(BUILD_PATHS)
	@echo $(addprefix $(BUILD_DIR)/obj/, $(OBJECTS))
#	@echo $(addprefix -I,$(C_INCLUDES))
