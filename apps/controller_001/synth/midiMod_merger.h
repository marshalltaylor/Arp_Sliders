#ifndef MIDIMOD_MERGER_H
#define MIDIMOD_MERGER_H
#include "midiMod.h"

class Merger : public MidiModule
{
public:
    Merger(void);
    void input(MidiMessage * pMsg, uint8_t inPort);
    void flushOutputList(void);
    void printDebug(void);
private:
    void emitMsg(MidiMessage * pMsg);
    uint8_t notesOn[16][128];
};

#endif