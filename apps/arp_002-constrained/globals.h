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


float power(float x, int y);

#endif // GLOBALS_H_INCLUDED