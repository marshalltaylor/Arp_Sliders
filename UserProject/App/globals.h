//Header
#ifndef GLOBALS_H_INCLUDED
#define GLOBALS_H_INCLUDED

#include <Arduino.h>
#include <MIDI.h>
#include "midiTime.h"
#include "sourceNoteMixer.h"
#include "outputNoteMixer.h"
#include <TeensyView.h>  // Include the SFE_TeensyView library
#include "SequenceTeensyView.h"
#include "SequencePlayer.h"
#include "SequenceRecorder.h"
#include "SequenceDebug.h"

//LCD
extern SequenceTeensyView oled;

//MIDI hw interfaces
extern midi::MidiInterface<HardwareSerial> MIDI;
extern midi::MidiInterface<HardwareSerial> CtrlMIDI;

//MIDI clocks
extern MidiClock extMidiClock;
extern MidiClock intMidiClock;
extern MidiClockSocket clockSocket;

//Mixers
extern SourceNoteMixer controlNoteMixer;
extern OutputNoteMixer outputNoteMixer;

//Sequence
enum
{
	REG_ARP = 0,
	REG_BASS,
	REG_DRUMS,
	REG_LENGTH
};

extern SequenceRegister sReg[];

extern SequencePlayer myPlayer;
extern SequenceRecorder myRecorder;
extern SequenceDebug sDebug;

#endif // GLOBALS_H_INCLUDED