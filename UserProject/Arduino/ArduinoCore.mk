ARDUINO_CORE_DIRECTORY = ./Arduino/Core

ARDUINO_CORE_C_SOURCES = \
$(ARDUINO_CORE_DIRECTORY)/interface.cpp \
$(ARDUINO_CORE_DIRECTORY)/abi.cpp \
$(ARDUINO_CORE_DIRECTORY)/GPIO.cpp \
$(ARDUINO_CORE_DIRECTORY)/ADC.cpp \
$(ARDUINO_CORE_DIRECTORY)/HardwareSerial.cpp \
$(ARDUINO_CORE_DIRECTORY)/STM_ArduinoPins.cpp \
$(ARDUINO_CORE_DIRECTORY)/itoa.cpp \
$(ARDUINO_CORE_DIRECTORY)/Print.cpp \
$(ARDUINO_CORE_DIRECTORY)/Stream.cpp \
$(ARDUINO_CORE_DIRECTORY)/WString.cpp \
$(ARDUINO_CORE_DIRECTORY)/STM_ArduinoPins.cpp

ARDUINO_CORE_INCLUDES = \
$(ARDUINO_CORE_DIRECTORY)