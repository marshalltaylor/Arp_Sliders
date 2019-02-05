ARDUINO_LIB_DIRECTORY = ./Arduino/Libs

ARDUINO_LIB_C_SOURCES = \
$(ARDUINO_LIB_DIRECTORY)/uCModules/src/flagMessaging.cpp \
$(ARDUINO_LIB_DIRECTORY)/uCModules/src/HardwareInterfaces.cpp \
$(ARDUINO_LIB_DIRECTORY)/uCModules/src/Panel.cpp \
$(ARDUINO_LIB_DIRECTORY)/uCModules/src/PanelComponents.cpp \
$(ARDUINO_LIB_DIRECTORY)/uCModules/src/PanelDataObjects.cpp \
$(ARDUINO_LIB_DIRECTORY)/uCModules/src/timeKeeper.cpp \
$(ARDUINO_LIB_DIRECTORY)/uCModules/src/timeKeeper32.cpp \
$(ARDUINO_LIB_DIRECTORY)/uCModules/src/timerModule.cpp \
$(ARDUINO_LIB_DIRECTORY)/uCModules/src/timerModule32.cpp \
$(ARDUINO_LIB_DIRECTORY)/arduino_midi_library/src/MIDI.cpp

ARDUINO_LIB_INCLUDES = \
$(ARDUINO_LIB_DIRECTORY)/uCModules/src \
$(ARDUINO_LIB_DIRECTORY)/arduino_midi_library/src
