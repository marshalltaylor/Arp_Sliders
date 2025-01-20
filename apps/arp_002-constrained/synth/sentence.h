#ifndef SENTENCE_H
#define SENTENCE_H

#include <stdint.h>

#include "linkedList.h"
#include "noteLists.h"

namespace bach
{
    enum chordQual_t
    {
        OPEN = 0,
        M,
        m,
        M7,
        m7
    };
};

struct arpStep_t
{
    int16_t offset;
    uint16_t length;
    chordQual_t qual;
};

class sentence : public LinkedList
{
public:
    arpeggio(listIndex_t maxLengthIn, uint16_t dataSizeIn);
    void insertByTime(seqNote_t * pNote);
    void next(void);
    void markToTime(bookmark_t, float);
    void printList(void);
};

#endif
