#ifndef SPLITCHANNELS_H
#define SPLITCHANNELS_H
#include "midiMod.h"

class SplitChannels : public MidiModule
{
public:
    SplitChannels(void);
    void input(MidiMessage * pMsg, uint8_t inPort);
private:
    
};

#endif