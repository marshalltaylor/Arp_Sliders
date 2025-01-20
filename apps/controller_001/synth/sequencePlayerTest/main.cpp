#include <iostream>
#include "sequence.h"
#include "sequenceTest.h"
#include "sequencePlayer.h"
#include "midi47fx.h"
#include "poolTest.h"
#include "bsp.h"

uint32_t midiPulses = 0;

PoolTest linkedListMem(20, 120);

MidiTestFixture MIDI;

NoteList noteList(20, sizeof(seqNote_t));
NoteOnList noteOnList(20, sizeof(seqNote_t));

sequence_t sequence;

SequencePlayer myPlayer;

void sequenceCallback(bool isNoteOn, sequence_t * seq, seqNote_t * pNote)
{
    UNUSED(isNoteOn);
    UNUSED(seq);
    
    printf("*** callback(%d, 0x%02X, %d, %d)\n", isNoteOn, seq->channel, pNote->pitch, pNote->velocity);
}

static void runPulses(uint32_t t, uint32_t step)
{
    for(uint32_t i = 0; i < t; i = i + step)
    {
        // give delta to test function
        myPlayer.process(step);
    }
}

/* Functions -----------------------------------------------------------------*/
int main ()
{
//    SC_logEntry_t logEntry;
//    bool retStatus;
    noteList.setMalloc(&linkedListMem);
    noteOnList.setMalloc(&linkedListMem);
    
    sequence.noteList = &noteList;
    myPlayer.setSequence(&sequence);
    myPlayer.setCallback(&sequenceCallback);
    myPlayer.setNoteOnList(&noteOnList);
    
    myPlayer.clearSequence();
    
    printf("########## Prep the note list and print it before working with the player.\n");
    fillListWithTestData(&noteList, real4bar);
    noteList.printList();
    sequence.bookmark = BOOKMARK_PLAYHEAD;
    sequence.playHead = 0;
    sequence.playing = true;
    sequence.length = 96;
    sequence.syncState = seqType::LENGTH_SET;

    printf("########## Run for half of the pattern and inspect\n");
    myPlayer.printInfo();
    myPlayer.start();
    runPulses(40, 1);
    //
    myPlayer.printInfo();
    
    printf("########## Run beyond the pattern and inspect\n");
    
    myPlayer.start();
    runPulses(70, 1);
    //
    myPlayer.printInfo();


}