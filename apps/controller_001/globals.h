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
#include "midiNoteList.h"
#include "pool.h"
#include "midiMod_outputPort.h"

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

extern Pool pool_20;
extern Pool pool_32;

#include "midiTime.h"
extern MidiClock midiClock;

#include "sequence.h"
#include "sequenceTest/sequenceTest.h"
extern NoteList mainNoteList;
extern NoteList altNoteList;
extern NoteList backupNoteList;

extern sequence_t mainSequence;
extern sequence_t altSequence;

#include "sequencePlayer.h"
extern SequencePlayer player;

#include "noteOnList.h"
extern NoteOnList noteOnList;

#include "midiMod_outputMixer.h"
extern OutputMixer outputMixer;

#include "midiMod_splitChannels.h"
extern SplitChannels splitChannels;

#include "legacyAdapter.h"
extern LegacyAdapter legacyAdapter;

#include "aux_controllers.h"
extern AuxController auxCtrl;

#include "inputNoteList.h"
extern InputNoteList inputNoteList;

#include "arp.h"
extern ArpModule arp;
extern NoteOnList ArpNoteOnList;

#include "midiTimeQuantize.h"
extern QCalculator qInputBeat;
extern QCalculator qPattern;
extern QCalculator qPatternHalf;

//Defined in globals.cpp
float power(float x, int y);

#endif // GLOBALS_H_INCLUDED