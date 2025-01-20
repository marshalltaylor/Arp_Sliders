#ifndef NOTEONLIST_H
#define NOTEONLIST_H

#include <stdint.h>

#include "sequence.h"

class NoteOnList : public LinkedList
{
public:
    NoteOnList(listIndex_t maxLengthIn, uint16_t dataSizeIn);
//    seqNote_t * NoteList::getNoteAfterTime( float time )
//    void insertByTime(seqNote_t *);
    void decrementTimes(uint16_t pulseDelta);
    bool dropExpiredNote(seqNote_t * pNote);
    void printList(void);
    void pushModLen(listItem_t *, uint16_t lenInClocks );
};

#endif
