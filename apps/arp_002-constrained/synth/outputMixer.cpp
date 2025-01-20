#include "globals.h"
#include "outputMixer.h"
#include "midiCommon.h"

#define localPrintf bspPrintf

OutputMixer::OutputMixer(void)
{
    memset(notesOn, 0x00, sizeof(notesOn));
}

void OutputMixer::keyboardInput(MidiMessage * pMsg)
{
    uint8_t key = pMsg->value;// short-hand
    if(pMsg->controlMask == NoteOn)
    {
        if(notesOn[key] == 1)
        {
            //already had a note on, oh well
            //send a note off first
            MIDI.sendNoteOff(pMsg->value, pMsg->data, model.outputChannel);
            if(model.notesOnCtrlMidi)
            {
                CtrlMIDI.sendNoteOff(pMsg->value, pMsg->data, model.outputChannel);
            }
        }
        notesOn[key] = 1;
        //send a note on
        MIDI.sendNoteOn(pMsg->value, pMsg->data, model.outputChannel);
        if(model.notesOnCtrlMidi)
        {
            CtrlMIDI.sendNoteOn(pMsg->value, pMsg->data, model.outputChannel);
        }
    }
    else if(pMsg->controlMask == NoteOff)
    {
        if(notesOn[key] == 0)
        {
            //Oh well, the key wasn't on.  Probably ok to send
        }
        //This is the last note that turned off,
        MIDI.sendNoteOff(pMsg->value, pMsg->data, model.outputChannel);
        if(model.notesOnCtrlMidi)
        {
            CtrlMIDI.sendNoteOff(pMsg->value, pMsg->data, model.outputChannel);
        }
        notesOn[key] = 0;
    }
    
}

//Player input is not as high of priority as the keyboard input
//(We don't want player notes retriggering a held emotive key)
void OutputMixer::playerInput(MidiMessage * pMsg)
{
    uint8_t key = pMsg->value;// short-hand
    if(pMsg->controlMask == NoteOn)
    {
        notesOn[key]++;
        if(notesOn[key] == 1)
        {
            //First note played
            MIDI.sendNoteOn(pMsg->value, pMsg->data, model.outputChannel);
            if(model.notesOnCtrlMidi)
            {
                CtrlMIDI.sendNoteOn(pMsg->value, pMsg->data, model.outputChannel);
            }
        }
    }
    else if(pMsg->controlMask == NoteOff)
    {
        if(notesOn[key] > 0)
        {
            notesOn[key]--;
            if(notesOn[key] == 0)
            {
                //This is the last note that turned off,
                MIDI.sendNoteOff(pMsg->value, pMsg->data, model.outputChannel);
                if(model.notesOnCtrlMidi)
                {
                    CtrlMIDI.sendNoteOff(pMsg->value, pMsg->data, model.outputChannel);
                }
            }
        }
    }
}

void OutputMixer::flushOutputList(void)
{
    for(int i = 0; i < 128; i++)
    {
        if(notesOn[i] > 0)
        {
            for(int j = 0; j < notesOn[i]; j++)
            {
                //send note off for each
                MIDI.sendNoteOff(i, 0, model.outputChannel);
                if(model.notesOnCtrlMidi)
                {
                    CtrlMIDI.sendNoteOff(i, 0, model.outputChannel);
                }
            }
        }
    }
}

void OutputMixer::printDebug(void)
{
    localPrintf("outputMixer note on list\n");
    for(int i = 0; i < 128; i++)
    {
        localPrintf("%02d ", notesOn[i]);
        if((i+1)%12 == 0)
        {
            localPrintf("\n");
        }
    }
    localPrintf("\n");
}
