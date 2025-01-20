#include "globals.h"
#include "legacyAdapter.h"
#include "midiCommon.h"

#define localPrintf bspPrintf

#define UNUSED(x)

LegacyAdapter::LegacyAdapter(void)
{
    
}

void LegacyAdapter::input(MidiMessage * pMsg, uint8_t inPort)
{
    dPrintf("LegacyAdapter: input()\n");
    dPrintNoteStr(pMsg, inPort);
    dPrintf("\n");
    UNUSED(inPort); //Is this OK?

    //Convert to type for linked list
    seqNoteRecorded_t noteRec;
    noteRec.note.pitch = pMsg->value;
    noteRec.note.velocity = pMsg->data;
    if (pMsg->controlMask == 0x90)
    {
        //Note on
        dPrintf("LegacyAdapter: Note on\n");
        //Signal the first note played after recording started
        controller.checkFirstInput();
        noteRec.note.time = midiClock.ticks;//pMsg->tick;
        noteRec.recordedFlag = model.recording;
        inputNoteList.noteOn(&noteRec);
    }
    else if (pMsg->controlMask == 0x80)
    {
        //Note off
        dPrintf("LegacyAdapter: Note off\n");
        noteRec.recordedFlag = model.recording; //Maybe used for notes on during rec start?
        inputNoteList.noteOff(&noteRec);
    }
}
