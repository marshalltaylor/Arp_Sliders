//**********************************************************************//
//  BEERWARE LICENSE
//
//  This code is free for any use provided that if you meet the author
//  in person, you buy them a beer.
//
//  This license block is BeerWare itself.
//
//  Written by:  Marshall Taylor
//  Created:  May 5, 2022
//
//**********************************************************************//
#ifndef INPUTNOTELIST_H_INCLUDED
#define INPUTNOTELIST_H_INCLUDED

#include <stdint.h>

#include "linkedList.h"

struct seqNoteRecorded_t
{
    seqNote_t note;
    bool recordedFlag;
};

//This is for output so use simpler note type
typedef void (*pNoteCompleteCB_t)(seqNote_t *);

class InputNoteList : public LinkedList
{
public:
    InputNoteList(listIndex_t maxLengthIn, uint16_t dataSizeIn);
    void noteOn(seqNoteRecorded_t * pNote);
    void noteOff(seqNoteRecorded_t * pNote);
    void setNoteCompleteCB(pNoteCompleteCB_t cb);
    void printList(void);
private:
    int16_t pitchInList(uint8_t pitch);
    pNoteCompleteCB_t pNoteCompleteCB;
};

#endif
