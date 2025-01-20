#ifndef SEQUENCETEST_H
#define SEQUENCETEST_H

/* Includes -- STD -----------------------------------------------------------*/
#include <stdint.h>
#include <stdbool.h>
#include <stdarg.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

/* Includes -- App -----------------------------------------------------------*/
#include "sequence.h"

//struct testNoteInfo_t
//{
//    float unused;
//    seqNote_t note;
//};

extern NoteList noteList;

extern seqNote_t testData001[];
extern seqNote_t real4bar[];

//class TestSequencePlayer : public SequencePlayer
//{
//public:
//    using SequencePlayer::SequencePlayer;
//    void listSequence(void);
//    void listSequence(bool listHeader, uint32_t start, uint32_t end);
//    void testBasic(void);
//};

void fillListWithTestData(NoteList * list, seqNote_t * data);
void printNoteInfo(seqNote_t * pNote);

#endif
