#ifndef LEGACYADAPTER_H
#define LEGACYADAPTER_H
#include "midiMod.h"

class LegacyAdapter : public MidiModule
{
public:
    LegacyAdapter(void);
    void input(MidiMessage * pMsg, uint8_t inPort);
private:
    
};

#endif