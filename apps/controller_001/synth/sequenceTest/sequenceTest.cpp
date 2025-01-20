/* Includes -- App -----------------------------------------------------------*/
#include "bsp.h"
#include "sequence.h"

/* Includes -- Test ----------------------------------------------------------*/
#include "sequenceTest.h"

/* References ----------------------------------------------------------------*/
// Connect directly to bsp.
#define localPrintf bspPrintf

/* Functions -----------------------------------------------------------------*/
void printNoteInfo(seqNote_t * pNote)
{
    localPrintf("Time: %f, Len: %f, pitch: %d, velo: %d, accent: %d\n",
                pNote->time,
                pNote->length,
                pNote->pitch,
                pNote->velocity,
                pNote->accent);
}

void fillListWithTestData(NoteList * list, seqNote_t * data)
{
    UNUSED(list);
    //printf("Address %p\n", (void*)&data[0]);
    //printf("len %d\n", (int)sizeof(*data));
    seqNote_t * pNote;
    pNote = data;
    int len;
    for(len = 0; pNote->time >= 0.0; len++)
    {
        //printf("Address %p\n", (void*)pNote);
        //printNoteInfo(pNote);
        list->push(pNote);
        pNote++;
    }
    
}
