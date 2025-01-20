#include "midiCommon.h"
#include "midi47fx.h"
#include "midiMod_outputPort.h"


OutputPort::OutputPort(void)
{
    midiItf = NULL;
}

void OutputPort::setMidiInterface(midi::MidiInterface<HardwareSerial> * itf)
{
    midiItf = itf;
}

void OutputPort::input(MidiMessage * pMsg, uint8_t inPort)
{
    dPrintf("OutputPort: input()\n");
    dPrintNoteStr(pMsg, inPort);
    dPrintf("\n");
    //printItfMessage(pMsg);
    //Module only accepts 1 input
    if(inPort != 0)
    {
        return;
    }
    if(midiItf != NULL)
    {
        //Is fairly stupid, just send it out
        midiItf->send((midi::MidiType)pMsg->controlMask, pMsg->value, pMsg->data, pMsg->channel);
    }
}
