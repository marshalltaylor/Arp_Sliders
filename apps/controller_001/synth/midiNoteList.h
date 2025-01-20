#ifndef MIDINOTELIST_H
#define MIDINOTELIST_H

#include "linkedList.h"
#include "midiCommon.h"

class MidiNoteList : public LinkedList
{
public:
    MidiNoteList(listIndex_t maxLengthIn, uint16_t dataSizeIn);

    listIndex_t seekObjectByNoteValue( MidiMessage * data );

    void printList(void);
};

#endif
