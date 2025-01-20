#include "globals.h"
#include "midiMod_splitChannels.h"
#include "midiCommon.h"

#define localPrintf bspPrintf

#define UNUSED(x)

SplitChannels::SplitChannels(void)
{
    
}

void SplitChannels::input(MidiMessage * pMsg, uint8_t inPort)
{
    dPrintf("SplitChannels: input()\n");
    dPrintNoteStr(pMsg, inPort);
    dPrintf("\n");
    UNUSED(inPort); //Is this OK?
    //Switch on input port
    switch (pMsg->channel)
    {
        case 1: //main keyboard
        {
            dPrintf("SplitChannels: input channel 1\n");
            //For now, block all but note on/off to looper
            if ((pMsg->controlMask == NoteOn) ||
                (pMsg->controlMask == NoteOff));
            {
                send(pMsg, 0);
                send(pMsg, 1);
            }
            break;
        }
        case 2: //aux keyboard
        {
            dPrintf("SplitChannels: input channel 2\n");
            //Only send notes to arp control input
            if ((pMsg->controlMask == NoteOn) ||
                (pMsg->controlMask == NoteOff));
            {
                send(pMsg, 2);
            }
            //Send all to control brain
            send(pMsg, 3);
            break;
        }
        default:
        if (debugEnabled)
        {
            dPrintf("SplitChannels: rejected input\n");
        }
        break;
    }
}
