/* Includes ------------------------------------------------------------------*/
#include "globals.h"

/* Definitions -- (Global Access) --------------------------------------------*/
HardwareSerial mainMidiInterface;
HardwareSerial ctrlMidiInterface;

MIDI_CREATE_INSTANCE(HardwareSerial, mainMidiInterface,  MIDI);
MIDI_CREATE_INSTANCE(HardwareSerial, ctrlMidiInterface, CtrlMIDI);

View view;
Controller controller;

//Unknown range on this timer -- see bsp.h?
uint32_t hwTimerInterval = 100;

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

/* Functions -----------------------------------------------------------------*/
void hwTimerCallback(void)
{
    //bspPrintf("pip\n");
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
    //Thru option:
    MIDI.turnThruOn();
    
    CtrlMIDI.setHandleClock(handleClock2);
    CtrlMIDI.setHandleStart(handleStart2);
    CtrlMIDI.setHandleContinue(handleContinue2);
    CtrlMIDI.setHandleStop(handleStop2);
    CtrlMIDI.setHandleNoteOn(handleNoteOn2);
    CtrlMIDI.setHandleNoteOff(handleNoteOff2);
    //Thru option:
    CtrlMIDI.turnThruOn();
    
    // Write our function address into the hw timer
    timer3setPeriod(hwTimerInterval);
    timer3TickCallback = hwTimerCallback;
}
