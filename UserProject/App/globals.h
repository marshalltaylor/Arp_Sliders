//Header
#ifndef GLOBALS_H_INCLUDED
#define GLOBALS_H_INCLUDED

#include <Arduino.h>
#include <MIDI.h>
#include "midiTime.h"
#include "sourceNoteMixer.h"
#include "outputNoteMixer.h"
#include <TeensyView.h>  // Include the SFE_TeensyView library
#include "musicView.h"

extern midi::MidiInterface<HardwareSerial> MIDI;
extern midi::MidiInterface<HardwareSerial> CtrlMIDI;

extern MidiClock extMidiClock;
extern MidiClock intMidiClock;
extern MidiClockSocket clockSocket;

//Note objects
extern SourceNoteMixer controlNoteMixer;
//extern Loop loops[10];
//extern LoopPlayer outputPlayer;
extern OutputNoteMixer outputNoteMixer;

extern OLEDFunctions oled;

//extern PatternContainer pattern;

#endif // GLOBALS_H_INCLUDED