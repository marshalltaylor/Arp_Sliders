//**********************************************************************//
//  BEERWARE LICENSE
//
//  This code is free for any use provided that if you meet the author
//  in person, you buy them a beer.
//
//  This license block is BeerWare itself.
//
//  Written by:  Marshall Taylor
//  Created:  April 4, 2019
//
//**********************************************************************//
#ifndef MIDIMOD_H
#define MIDIMOD_H

#include <stdint.h>

#include "midiCommon.h"

#define MIDI_MODULE_PORTS_MAX 4

// Midi common interface
class MidiModule
{
public:
    MidiModule(void){};
    virtual void input(MidiMessage * pMsg, uint8_t inPort){};
    void setOutput(uint8_t outPort, MidiModule * downstreamModule, uint8_t inPort);
    void setDebug(bool enabled);
protected:
    MidiModule * pDownstreamModule[MIDI_MODULE_PORTS_MAX] = {0};
    uint8_t downstreamModuleInputPort[MIDI_MODULE_PORTS_MAX] = {0};
    void send(MidiMessage * pMsg, uint8_t channel);
    void dPrintNoteStr(MidiMessage * pMsg, uint8_t channel);
    void dPrintf(const char* fmt, ...);
    bool debugEnabled = false;
};

#endif //MIDIMOD_H