#ifndef AUX_CONTROLLERS_H
#define AUX_CONTROLLERS_H
#include "midiMod.h"

class AuxController : public MidiModule
{
public:
    AuxController(void);
    void input(MidiMessage * pMsg, uint8_t inPort);
private:
    
};

#endif