#ifndef GLOBALS_H_INCLUDED
#define GLOBALS_H_INCLUDED

/* Includes -- STD -----------------------------------------------------------*/
#include <stdint.h>
#include <stdbool.h>
#include <stdarg.h>
#include <string.h>
#include <stdio.h>

/* Includes -- BSP -----------------------------------------------------------*/
#include "bsp.h"

/* Includes -- modules -------------------------------------------------------*/
#include "logging.h"
#include "midi47fx.h"

/* Includes -- App -----------------------------------------------------------*/
#include "model.h"
#include "view.h"
#include "controller.h"
#include "pool.h"

/* References ----------------------------------------------------------------*/
////Defined in bspTime.h
//extern uint8_t usTicksLocked;
//extern uint32_t usTicks;

//MIDI hw interfaces
extern HardwareSerial mainMidiInterface;
extern HardwareSerial ctrlMidiInterface;

extern midi::MidiInterface<HardwareSerial> MIDI;
extern midi::MidiInterface<HardwareSerial> CtrlMIDI;

extern uint32_t usTicks;

extern View view;
extern Controller controller;

extern uint32_t hwTimerInterval;

#include "TeensyView.h"
extern TeensyView oled;

extern Pool pool_20;
extern Pool pool_32;

#include "midiMod_outputPort.h"

#include "midiMod_outputMixer.h"
extern OutputMixer outputMixer;


//Defined in globals.cpp
float power(float x, int y);

#endif // GLOBALS_H_INCLUDED