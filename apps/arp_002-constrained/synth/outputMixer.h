#ifndef OUTPUTMIXER_H
#define OUTPUTMIXER_H
#include "midiCommon.h"

class OutputMixer
{
public:
    OutputMixer(void);
    void keyboardInput(MidiMessage * pMsg);
    void playerInput(MidiMessage * pMsg);
    void flushOutputList(void);
    void printDebug(void);
private:
    uint8_t notesOn[128];
};

#endif