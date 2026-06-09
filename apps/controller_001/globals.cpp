/* Includes ------------------------------------------------------------------*/
#include "globals.h"
#include "midiCommon.h"

#ifndef UNUSED
#define UNUSED(x) (void)x
#endif

#define localPrintf bspPrintf
/* Definitions -- (Global Access) --------------------------------------------*/
HardwareSerial mainMidiInterface;
HardwareSerial ctrlMidiInterface;

MIDI_CREATE_INSTANCE(HardwareSerial, mainMidiInterface,  MIDI);
MIDI_CREATE_INSTANCE(HardwareSerial, ctrlMidiInterface, CtrlMIDI);

View view;
Controller controller;

TeensyView oled(0,0,0,0,0);

Pool pool_20(20, 500);
Pool pool_32(32, 50);

//Unknown range on this timer -- see bsp.h?
uint32_t hwTimerInterval = 100;

// midiMod modules
OutputPort outMain;
OutputPort outAux;
Merger merger;
Controllers controllers;

/* References ----------------------------------------------------------------*/
extern void handleClock1(void);
extern void handleStart1(void);
extern void handleContinue1(void);
extern void handleStop1(void);
extern void handleClock2(void);
extern void handleStart2(void);
extern void handleContinue2(void);
extern void handleStop2(void);
extern void handleNoteOn1(byte channel, byte pitch, byte velocity);
extern void handleNoteOff1(byte channel, byte pitch, byte velocity);
extern void handleNoteOn2(byte channel, byte pitch, byte velocity);
extern void handleNoteOff2(byte channel, byte pitch, byte velocity);
extern void handleControlChange1(byte channel, byte number, byte value);
extern void handleControlChange2(byte channel, byte number, byte value);
extern void handlePitchBend1(byte channel, int bend);
extern void handlePitchBend2(byte channel, int bend);

/* Functions -----------------------------------------------------------------*/
void hwTimerCallback(void)
{
    //UNUSED.  Previously ticked arp and player modules
}

//Usage:
// This function doesn't have a prototype because the header that would
// have it (globals.h) has c++ linkage which will cause problems if included
// in c code.
//
// Add 'extern void globalsInit(void);' to calling file
extern "C" void globalsInit(void)
{
    mainMidiInterface.initPort(COM1);
    ctrlMidiInterface.initPort(COM2);

    MIDI.setHandleClock(handleClock1);
    MIDI.setHandleStart(handleStart1);
    MIDI.setHandleContinue(handleContinue1);
    MIDI.setHandleStop(handleStop1);
    MIDI.setHandleNoteOn(handleNoteOn1);
    MIDI.setHandleNoteOff(handleNoteOff1);
    MIDI.setHandleControlChange(handleControlChange1);
    MIDI.setHandlePitchBend(handlePitchBend1);
    //Thru option:
    MIDI.turnThruOff();

    CtrlMIDI.setHandleClock(handleClock2);
    CtrlMIDI.setHandleStart(handleStart2);
    CtrlMIDI.setHandleContinue(handleContinue2);
    CtrlMIDI.setHandleStop(handleStop2);
    CtrlMIDI.setHandleNoteOn(handleNoteOn2);
    CtrlMIDI.setHandleNoteOff(handleNoteOff2);
    CtrlMIDI.setHandleControlChange(handleControlChange2);
    CtrlMIDI.setHandlePitchBend(handlePitchBend2);
    //Thru option:
    CtrlMIDI.turnThruOff();

    // Write our function address into the hw timer
    timer3setPeriod(hwTimerInterval);
    timer3TickCallback = hwTimerCallback;

    //Assign memory to note lists
    
    //Route input notes complete to record
    
    // Wrap midi interfaces in midi modules
    outMain.setMidiInterface(&MIDI);
    outAux.setMidiInterface(&CtrlMIDI);

    //// Route midi modules (midi graph!)
    //splitChannels.setOutput(0, &legacyAdapter, 0);
    //splitChannels.setOutput(1, &outputMixer, 0);
    //splitChannels.setOutput(2, &arp, 1);
    //splitChannels.setOutput(3, &auxCtrl, 0);
    //
    //outputMixer.setOutput(0, &arp, 0);
    //arp.setOutput(0, &outMain, 0);
    //arp.setOutput(1, &outAux, 0);

    controllers.setCtrlNum(0, 2); //Breath
    controllers.setChannel(0, 5);
    controllers.setCtrlNum(1, 4); //Pedal
    controllers.setChannel(1, 5);
    controllers.setCtrlNum(2, 2);
    controllers.setChannel(2, 6);
    controllers.setCtrlNum(3, 4);
    controllers.setChannel(3, 6);
    controllers.setCtrlNum(4, 10);
    controllers.setChannel(4, 6);
    controllers.setCtrlNum(5, 11);
    controllers.setChannel(5, 6);
    controllers.setOutput(0, &merger, 0);
    controllers.setOutput(1, &merger, 1);
    merger.setOutput(0, &outMain, 0);
    merger.setOutput(1, &outAux, 0);
    merger.setDebug(false);
}
