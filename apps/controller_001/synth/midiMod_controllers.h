#ifndef MIDIMOD_CONTROLLERS_H
#define MIDIMOD_CONTROLLERS_H
#include "midiMod.h"

#define MIDI_MOD_CONTROLLERS_MAX 6

class Controllers : public MidiModule
{
public:
    Controllers(void);
    //void input(MidiMessage * pMsg, uint8_t inPort);
    void setValue(uint8_t index, uint8_t value);
    void setCtrlNum(uint8_t index, uint8_t ctrlNum);
    void setChannel(uint8_t index, uint8_t channel);
    //void flushOutputList(void);
    void tick(void);
    void printDebug(void);
private:
    void emitMsg(MidiMessage * pMsg);
    uint8_t controllerNumber[MIDI_MOD_CONTROLLERS_MAX];
    uint8_t controllerChannel[MIDI_MOD_CONTROLLERS_MAX];
    uint8_t controllerValue[MIDI_MOD_CONTROLLERS_MAX];
    uint8_t lastControllerValue[MIDI_MOD_CONTROLLERS_MAX];
};

#endif