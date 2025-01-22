#include "globals.h"
#include "midiMod_controllers.h"
#include "midiCommon.h"

#define localPrintf bspPrintf

Controllers::Controllers(void)
{
}

void Controllers::emitMsg(MidiMessage * pMsg)
{
    //Main output
    send(pMsg, 0);
    //Aux output
    send(pMsg, 1);
}

void Controllers::setValue(uint8_t index, uint8_t value)
{
    if (index >= MIDI_MOD_CONTROLLERS_MAX)
    {
        return;
    }
    controllerValue[index] = value;
}

void Controllers::setCtrlNum(uint8_t index, uint8_t ctrlNum)
{
    if (index >= MIDI_MOD_CONTROLLERS_MAX)
    {
        return;
    }
    controllerNumber[index] = ctrlNum;
}

void Controllers::setChannel(uint8_t index, uint8_t channel)
{
    if (index >= MIDI_MOD_CONTROLLERS_MAX)
    {
        return;
    }
    controllerChannel[index] = channel;
}

//void Controllers::input(MidiMessage * pMsg, uint8_t inPort)
//{
//    dPrintf("Controllers: input()\n");
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

//void Controllers::flushOutputList(void)
//{
//    for(int i = 0; i < 128; i++)
//    {
//        if(notesOn[i] > 0)
//        {
//            for(int j = 0; j < notesOn[i]; j++)
//            {
//                //send note off for each
//                MIDI.sendNoteOff(i, 0, model.outputChannel);
//                if(model.notesOnCtrlMidi)
//                {
//                    CtrlMIDI.sendNoteOff(i, 0, model.outputChannel);
//                }
//            }
//        }
//    }
//}


void Controllers::tick(void)
{
    for(int i = 0; i < MIDI_MOD_CONTROLLERS_MAX; i++)
    {
        if (lastControllerValue[i] != controllerValue[i])
        {
            //send it
            MidiMessage Msg = {
                .controlMask = ControlChange,
                .channel = controllerChannel[i],
                .value = controllerNumber[i],
                .data = controllerValue[i],
                .tick = 0,
            };
            emitMsg(&Msg);
            lastControllerValue[i] = controllerValue[i];
        }
    }
}

void Controllers::printDebug(void)
{
    localPrintf("Controllers data\n");
    for(int i = 0; i < MIDI_MOD_CONTROLLERS_MAX; i++)
    {
        localPrintf("[ctrl %d:", controllerNumber[i]);
        localPrintf("%d,", controllerValue[i]);
        localPrintf("%d] ", lastControllerValue[i]);
        if((i+1)%12 == 0)
        {
            localPrintf("\n");
        }
    }
    localPrintf("\n");
}
