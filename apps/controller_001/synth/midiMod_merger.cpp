#include "globals.h"
#include "midiMod_merger.h"
#include "midiCommon.h"

#define localPrintf bspPrintf

Merger::Merger(void)
{
    memset(notesOn, 0x00, sizeof(notesOn));
}

void Merger::emitMsg(MidiMessage * pMsg)
{
    //Main output
    send(pMsg, 0);
    //Aux output
    send(pMsg, 1);
}

//void OutputMixer::input(MidiMessage * pMsg, uint8_t inPort)
//{
//    dPrintf("OutputMixer: input()\n");
//    dPrintNoteStr(pMsg, inPort);
//    
//    uint8_t key = pMsg->value;// short-hand
//    pMsg->channel = model.outputChannel; //Force channel to msg
//    //Module accppts keyboard on 0, player on 1
//    if (inPort == 0)
//    {
//        if(pMsg->controlMask == NoteOn)
//        {
//            if(notesOn[key] == 1)
//            {
//                //already had a note on, oh well
//                //send a note off first
//                pMsg->controlMask = NoteOff;
//                emitMsg(pMsg);
//            }
//            notesOn[key] = 1;
//            //send a note on
//            pMsg->controlMask = NoteOn;
//            emitMsg(pMsg);
//        }
//        else if(pMsg->controlMask == NoteOff)
//        {
//            if(notesOn[key] == 0)
//            {
//                //Oh well, the key wasn't on.  Probably ok to send
//            }
//            //This is the last note that turned off,
//            //Send note-off
//            emitMsg(pMsg);
//            notesOn[key] = 0;
//        }
//    }
//    else if (inPort == 1)
//    {
//        //Player input is not as high of priority as the keyboard input
//        //(We don't want player notes retriggering a held emotive key)
//        if(pMsg->controlMask == NoteOn)
//        {
//            notesOn[key]++;
//            if(notesOn[key] == 1)
//            {
//                //First note played
//                //Send note on
//                emitMsg(pMsg);
//            }
//        }
//        else if(pMsg->controlMask == NoteOff)
//        {
//            if(notesOn[key] > 0)
//            {
//                notesOn[key]--;
//                if(notesOn[key] == 0)
//                {
//                    //This is the last note that turned off,
//                    //Send note-off
//                    emitMsg(pMsg);
//                }
//            }
//        }
//    }
//}


//This merges two without regrigger
void Merger::input(MidiMessage * pMsg, uint8_t inPort)
{
    dPrintf("Merger: input()\n");
    dPrintNoteStr(pMsg, inPort);
    dPrintf("\n");
    
    // short-hand
    uint8_t key = pMsg->value;
    uint8_t chan = pMsg->channel;
    
    switch (pMsg->controlMask)
    {
        case NoteOn:
        {
            notesOn[chan][key]++;
            if(notesOn[chan][key] == 1)
            {
                //First note played
                //Send
                emitMsg(pMsg);
            }
        }
        break;
        case NoteOff:
        {
            if(notesOn[key] > 0)
//            {
//                notesOn[key]--;
//                if(notesOn[key] == 0)
//                {
//                    //This is the last note that turned off,
//                    //Send note-off
//                    emitMsg(pMsg);
//                }
//            }
            if(notesOn[chan][key] > 0)
            {
                notesOn[chan][key]--;
                if(notesOn[chan][key] == 0);
                //This is the last note that turned off,
                //Send note-off
                emitMsg(pMsg);
            }
        }
        break;
        //Exclude these
        case InvalidType:
        case TimeCodeQuarterFrame:
        case SongPosition:
        case SongSelect:
        case TuneRequest:
        case Clock:
        case Start:
        case Continue:
        case Stop:
        case ActiveSensing:
        case SystemReset:
        break;
        default:
        emitMsg(pMsg);
        break;
    }
}


void Merger::flushOutputList(void)
{
    MidiMessage msg = {
        .controlMask = NoteOff,
        .data = 0,
        .tick = 0,
    };
    for (uint8_t chan = 0; chan < 16; chan++)
    {
        for (uint8_t key = 0; key < 128; key++)
        {
            while(notesOn[chan][key] > 0)
            {
                //send note off for each
                msg.channel = chan;
                msg.value = key;
                emitMsg(&msg);
                notesOn[chan][key]--;
            }
        }
    }
}

void Merger::printDebug(void)
{
    localPrintf("Merger notes on\n");
    for (uint8_t chan = 0; chan < 16; chan++)
    {
        bool printedSectionHeading = false;
        for (uint8_t key = 0; key < 128; key++)
        {
            if(notesOn[chan][key] > 0)
            {
                if (!printedSectionHeading)
                {
                    localPrintf("Channel %d:\n", chan);
                    printedSectionHeading = true;
                }
                localPrintf("key, value: %d, %d\n", key, notesOn[chan][key]);
            }
        }
    }
    localPrintf("\n");
}
