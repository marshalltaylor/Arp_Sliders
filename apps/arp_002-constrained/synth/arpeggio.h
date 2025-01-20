#ifndef ARPEGGIO_H
#define ARPEGGIO_H

#include <stdint.h>

#include "linkedList.h"
#include "noteLists.h"

//The thing that defines the pattern of notes which will be repeated in the arpeggiation

struct arpStep_t
{
    int16_t offset
    uint16_t length
};

class arpeggio : public LinkedList
{
public:
    arpeggio(listIndex_t maxLengthIn, uint16_t dataSizeIn);
    void insertByTime(seqNote_t * pNote);
    void markToTime(bookmark_t, float);
    void printList(void);
};


#endif
