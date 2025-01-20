#include <stdarg.h>

#include "bsp.h"

#include "midiCommon.h"
#include "midiMod.h"

// Connect directly to bsp.
#define localPrintf bspPrintf

void MidiModule::setOutput(uint8_t outPort, MidiModule * downstreamModule, uint8_t inPort)
{
    if (outPort < MIDI_MODULE_PORTS_MAX)
    {
        pDownstreamModule[outPort] = downstreamModule;
        //Save the input port for the module connected to this output
        downstreamModuleInputPort[outPort] = inPort;
    }
    return;
};

void MidiModule::setDebug(bool enabled)
{
    debugEnabled = enabled;
};

//Internal function to provide clean output to channel syntax
void MidiModule::send(MidiMessage * pMsg, uint8_t channel)
{
    //protect
    if (pDownstreamModule[channel] != NULL)
    {
        pDownstreamModule[channel]->input(pMsg, downstreamModuleInputPort[channel]);
    }
}

//Print only if debug enabled
void MidiModule::dPrintf(const char* fmt, ...)
{
    //Setup
    va_list args;
    va_start(args, fmt);
    //Send to serial
    if (debugEnabled)
    {
        bspVPrintf_t pPrintFn = bspGetSerialConsoleVPrintf();
        pPrintFn(fmt, args);
    }
    va_end(args);
}

void MidiModule::dPrintNoteStr(MidiMessage * pMsg, uint8_t channel)
{
    dPrintf(" [port %d] msk: 0x%02X, [%d], %d, %d, t=%d",
                    channel,
                    pMsg->controlMask,
                    pMsg->channel,
                    pMsg->value,
                    pMsg->data,
                    pMsg->tick);
}
