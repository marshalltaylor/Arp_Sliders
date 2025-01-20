#ifndef SEQUENCE_H
#define SEQUENCE_H

#include <stdint.h>

#include "linkedList.h"
#include "noteLists.h"


#define STEPS_PER_BEAT 4
#define QUARTER_PER_SEQUENCE 8
#define SEQ_LEN (STEPS_PER_BEAT * QUARTER_PER_SEQUENCE) //elements in data
#define PULSES_PER_QUARTER 24
#define SUBS_PER_PULSE 20
#define SUBS_PER_SEQ (SUBS_PER_PULSE * PULSES_PER_QUARTER * QUARTER_PER_SEQUENCE)
#define SUBS_PER_RECORD (SUBS_PER_PULSE * PULSES_PER_QUARTER / STEPS_PER_BEAT)

#define SEG_FLAG_USED 0x01
#define SEG_FLAG_ON 0x02

#define BOOKMARK_PLAYHEAD 0
#define BOOKMARK_TEMP 1

class NoteList : public LinkedList
{
public:
    NoteList(listIndex_t maxLengthIn, uint16_t dataSizeIn);
//    seqNote_t * NoteList::getNoteAfterTime( float time )
    void insertByTime(seqNote_t * pNote);
    void markToTime(bookmark_t, float);
    void printList(void);
};

void seqcpy(NoteList *dst, NoteList *src);

namespace seqType
{
    enum syncState_t
    {
        NO_ZERO_TIME = 0,
        LENGTH_UNKNOWN,
        LENGTH_SET,
    };
};

struct sequence_t
{
    uint8_t channel;
    bool playing;
    int16_t playHead;
    NoteList * noteList;
    uint8_t bookmark;
    uint32_t length;
    seqType::syncState_t syncState;
};
#endif //SEQUENCE_H
