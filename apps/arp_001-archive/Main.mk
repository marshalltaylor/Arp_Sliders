CUBE_PROJECT_DIR = ../CubeProject

HAL_C_SOURCES =  \
Core/Src/main.c \
Core/Src/gpio.c \
Core/Src/adc.c \
Core/Src/adc_ext.c \
Core/Src/debugPins.c \
Core/Src/spiDisplay.c \
Core/Src/dma.c \
Core/Src/spi.c \
Core/Src/tim.c \
Core/Src/usart.c \
Core/Src/stm32f4xx_it.c \
Core/Src/stm32f4xx_hal_msp.c \
Core/Src/unused_int.c \
$(CUBE_PROJECT_DIR)/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_adc.c \
$(CUBE_PROJECT_DIR)/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_adc_ex.c \
$(CUBE_PROJECT_DIR)/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_spi.c \
$(CUBE_PROJECT_DIR)/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_tim.c \
$(CUBE_PROJECT_DIR)/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_tim_ex.c \
$(CUBE_PROJECT_DIR)/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_uart.c \
$(CUBE_PROJECT_DIR)/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_rcc.c \
$(CUBE_PROJECT_DIR)/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_rcc_ex.c \
$(CUBE_PROJECT_DIR)/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_flash.c \
$(CUBE_PROJECT_DIR)/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_flash_ex.c \
$(CUBE_PROJECT_DIR)/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_flash_ramfunc.c \
$(CUBE_PROJECT_DIR)/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_gpio.c \
$(CUBE_PROJECT_DIR)/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_dma_ex.c \
$(CUBE_PROJECT_DIR)/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_dma.c \
$(CUBE_PROJECT_DIR)/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_pwr.c \
$(CUBE_PROJECT_DIR)/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_pwr_ex.c \
$(CUBE_PROJECT_DIR)/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_cortex.c \
$(CUBE_PROJECT_DIR)/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal.c \
Core/Src/system_stm32f4xx.c \
startup_stm32f446xx.s

HAL_C_INCLUDES = \
$(CUBE_PROJECT_DIR)/Drivers/CMSIS/Include \
$(CUBE_PROJECT_DIR)/Drivers/CMSIS/Device/ST/STM32F4xx/Include \
$(CUBE_PROJECT_DIR)/Drivers/STM32F4xx_HAL_Driver/Inc \
./Core/Inc


PROJECT_C_SOURCES = \
./App/App.cpp \
./App/sketch.cpp \
./App/SlidersPanel.cpp \
./App/sourceNoteMixer.cpp \
./App/outputNoteMixer.cpp \
./App/globals.cpp \
./App/graphics/TeensyView.cpp \
./App/MidiUtils/MidiUtils.cpp \
./App/MidiUtils/midiTime.cpp \
./App/SequenceTeensyView/SequenceTeensyView.cpp \
./App/SequencePlayer/SequencePlayer.cpp \
./App/SequenceArpeggiator/SequenceArpeggiator.cpp \
./App/SequenceRecorder/SequenceRecorder.cpp \
./App/SequenceDebug/SequenceDebug.cpp \
./App/Sequences/SequenceCommon.cpp


PROJECT_C_INCLUDES = \
./Core/Inc \
./App/graphics \
./App/MidiUtils \
./App/SequencePlayer \
./App/SequenceArpeggiator \
./App/SequenceRecorder \
./App/SequenceDebug \
./App/SequenceTeensyView \
./App/Sequences \
./App
