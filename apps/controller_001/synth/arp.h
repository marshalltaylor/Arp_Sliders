#ifndef ARP_H
#define ARP_H

#include <stdint.h>

#include "midiMod.h"

#include "linkedList.h"
#include "noteOnList.h"
#include "midiTimeQuantize.h"

#define CONTROL_MIDI_KEY_ZERO_OFFSET 48

//struct arpSeqNote_t// : public MidiMessage
//{
//    bool muted;
//    MidiMessage msg;
//};

class ArpPattern : public LinkedList
{
public:
    ArpPattern(listIndex_t maxLengthIn, uint16_t dataSizeIn);
//    void addNote(arpSeqNote_t * note);
//    someNote_t getNoteStatus(uint16_t ticksorsomething);
//    void setTonality(tonality_t tonality);
    void printList(void);
//
private:
};


class ArpModule : public MidiModule
{
public:
    ArpModule(void);
    void setArpPattern(ArpPattern * pList);
    void clearPattern(void);
    void input(MidiMessage * pMsg, uint8_t inPort);
    void enableArp(bool value);
    void enablePatternEntry(bool value);
    void setNoteOnList(NoteOnList * noteOnList);
    void setQuantizeNotePeriod(uint32_t midiTicks);
    void rest(void);

    //Time based things
    void syncToClock(int32_t midiClocks);
    void start(int32_t t_minus_ticks);
    void start(void);
    //Work function
    bool process(uint16_t pulseDelta);

    uint8_t notesGated(void);
    void printInfo(void);

    bool playing;
    bool quantizeStepLength;
    int32_t playHead;
    uint32_t length;
    uint32_t currentNote;
    int gatedCount;
    uint16_t rootNote;
    bool rootNoteDirtyFlag;
    int32_t rootNoteTime;
    seqType::syncState_t syncState;
    inline void goNoteOn(seqNote_t * pNote);
    inline void goNoteOff(seqNote_t * pNote);

protected:
//public: //TODO: Make this private
    NoteOnList * pNoteOnList;
private:
    uint32_t notePeriod;
    bool arpEnabled;
    bool patternEntry;
    ArpPattern * arpPattern;
    QCalculator liveTimeQuantize;
    uint8_t notesOn[128];
};


#endif //SEQUENCE_H

