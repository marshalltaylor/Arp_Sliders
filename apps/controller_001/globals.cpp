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

Pool pool_20(20, 500);
Pool pool_32(32, 50);

MidiClock midiClock;

//Note lists
NoteList mainNoteList(200, sizeof(seqNote_t));
NoteList altNoteList(200, sizeof(seqNote_t));
NoteList backupNoteList(100, sizeof(seqNote_t));

//Sequences.  These contain note lists
//A sequence also carries channel, and some other properties
sequence_t mainSequence;
sequence_t altSequence;

//Sequence Player
SequencePlayer player;
//Note on list for player
NoteOnList noteOnList(20, sizeof(seqNote_t));

//Unknown range on this timer -- see bsp.h?
uint32_t hwTimerInterval = 100;

// midiMod modules
OutputPort outMain;
OutputPort outAux;
OutputMixer outputMixer;
SplitChannels splitChannels;
LegacyAdapter legacyAdapter;

AuxController auxCtrl;

ArpModule arp;
ArpPattern arpSequence(100, sizeof(MidiMessage));
//Note on list for arp output
NoteOnList ArpNoteOnList(20, sizeof(seqNote_t));

InputNoteList inputNoteList(15, sizeof(seqNoteRecorded_t));

QCalculator qInputBeat;
QCalculator qPattern;
QCalculator qPatternHalf;

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

/* Functions -----------------------------------------------------------------*/
void hwTimerCallback(void)
{
    //This is where midi ticks are counted and the information is given to time digesting modules
    int32_t dT = midiClock.getDeltaTicks();
    if(dT > 0)
    {
        player.process(dT);
        arp.process(dT);
    }
}

void recNote( seqNote_t * pNote )
{
    //localPrintf("recNote callback\n");
    player.recordNote(pNote);
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
    //Thru option:
    CtrlMIDI.turnThruOff();
    
    // Write our function address into the hw timer
    timer3setPeriod(hwTimerInterval);
    timer3TickCallback = hwTimerCallback;

    //Assign memory to note lists
    mainNoteList.setMalloc(&pool_20);
    altNoteList.setMalloc(&pool_20);
    backupNoteList.setMalloc(&pool_20);
    noteOnList.setMalloc(&pool_32);
    ArpNoteOnList.setMalloc(&pool_32);
    inputNoteList.setMalloc(&pool_32);
    arpSequence.setMalloc(&pool_20);
    
    //Route input notes complete to record
    inputNoteList.setNoteCompleteCB(&recNote);
    
    mainSequence.bookmark = BOOKMARK_PLAYHEAD;
    mainSequence.playHead = 0;
    mainSequence.playing = false;
    mainSequence.length = 10000;
    mainSequence.syncState = seqType::NO_ZERO_TIME;
    mainSequence.noteList = &mainNoteList;
    
    altSequence.bookmark = BOOKMARK_PLAYHEAD;
    altSequence.playHead = 0;
    altSequence.playing = false;
    altSequence.length = 10000;
    altSequence.syncState = seqType::NO_ZERO_TIME;
    altSequence.noteList = &altNoteList;
    
    player.setSequence(&mainSequence);
    player.setNoteOnList(&noteOnList);
    player.setUndoNoteList(&backupNoteList);
    player.setOutput(0, &outputMixer, 1);
    
    arp.setArpPattern(&arpSequence);
    arp.setNoteOnList(&ArpNoteOnList);
    
    // Wrap midi interfaces in midi modules
    outMain.setMidiInterface(&MIDI);
    outAux.setMidiInterface(&CtrlMIDI);

    // Route midi modules (midi graph!)
    splitChannels.setOutput(0, &legacyAdapter, 0);
    splitChannels.setOutput(1, &outputMixer, 0);
    splitChannels.setOutput(2, &arp, 1);
    splitChannels.setOutput(3, &auxCtrl, 0);

    outputMixer.setOutput(0, &arp, 0);
    arp.setOutput(0, &outMain, 0);
    arp.setOutput(1, &outAux, 0);

    qInputBeat.setDivisorPower(2);
    qInputBeat.setEnable(true);
}
