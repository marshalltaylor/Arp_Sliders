#include "globals.h"

MIDI_CREATE_INSTANCE(HardwareSerial, Serial2, MIDI);
MIDI_CREATE_INSTANCE(HardwareSerial, Serial1, CtrlMIDI);

MidiClock extMidiClock;
MidiClock intMidiClock;
MidiClockSocket clockSocket;

//Note objects
SourceNoteMixer controlNoteMixer;
//Loop loops[10];

//LoopPlayer outputPlayer;
OutputNoteMixer outputNoteMixer;

SequenceTeensyView oled(0,0,0,0,0);

SequenceRegister sReg[REG_LENGTH];

SequencePlayer myPlayer;
SequenceRecorder myRecorder;
SequenceDebug sDebug;