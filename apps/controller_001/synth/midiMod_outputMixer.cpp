#include "globals.h"
#include "midiMod_outputMixer.h"
#include "midiCommon.h"

#define localPrintf bspPrintf

OutputMixer::OutputMixer(void)
{
    memset(notesOn, 0x00, sizeof(notesOn));
}

void OutputMixer::emitMsg(MidiMessage * pMsg)
{
    //Main output
    send(pMsg, 0);
    //Aux output
    send(pMsg, 1);
}

void OutputMixer::input(MidiMessage * pMsg, uint8_t inPort)
{
    dPrintf("OutputMixer: input()\n");
    dPrintNoteStr(pMsg, inPort);
    
    uint8_t key = pMsg->value;// short-hand
    pMsg->channel = model.outputChannel; //Force channel to msg
    //Module accppts keyboard on 0, player on 1
    if (inPort == 0)
    {
        if(pMsg->controlMask == NoteOn)
        {
            if(notesOn[key] == 1)
            {
                //already had a note on, oh well
                //send a note off first
                pMsg->controlMask = NoteOff;
                emitMsg(pMsg);
            }
            notesOn[key] = 1;
            //send a note on
            pMsg->controlMask = NoteOn;
            emitMsg(pMsg);
        }
        else if(pMsg->controlMask == NoteOff)
        {
            if(notesOn[key] == 0)
            {
                //Oh well, the key wasn't on.  Probably ok to send
            }
            //This is the last note that turned off,
            //Send note-off
            emitMsg(pMsg);
            notesOn[key] = 0;
        }
    }
    else if (inPort == 1)
    {
        //Player input is not as high of priority as the keyboard input
        //(We don't want player notes retriggering a held emotive key)
        if(pMsg->controlMask == NoteOn)
        {
            notesOn[key]++;
            if(notesOn[key] == 1)
            {
                //First note played
                //Send note on
                emitMsg(pMsg);
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
                    //Send note-off
                    emitMsg(pMsg);
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
