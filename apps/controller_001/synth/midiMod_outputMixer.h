#ifndef OUTPUTMIXER_H
#define OUTPUTMIXER_H
#include "midiMod.h"

class OutputMixer : public MidiModule
{
public:
    OutputMixer(void);
    void input(MidiMessage * pMsg, uint8_t inPort);
    void flushOutputList(void);
    void printDebug(void);
private:
    void emitMsg(MidiMessage * pMsg);
    uint8_t notesOn[128];
};

#endif