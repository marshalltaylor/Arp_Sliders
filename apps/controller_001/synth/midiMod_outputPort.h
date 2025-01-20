#ifndef OUTPUTPORT_H
#define OUTPUTPORT_H
#include "midi47fx.h"
#include "midiMod.h"

class OutputPort : public MidiModule
{
public:
    OutputPort(void);
    void setMidiInterface(midi::MidiInterface<HardwareSerial> * itf);
    void input(MidiMessage * pMsg, uint8_t inPort);
private:
    midi::MidiInterface<HardwareSerial> * midiItf;
};

#endif