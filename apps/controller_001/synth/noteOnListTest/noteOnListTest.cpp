/* Includes -- App -----------------------------------------------------------*/
#include "bsp.h"
#include "sequence.h"

/* Includes -- Test ----------------------------------------------------------*/
#include "noteOnListTest.h"

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
