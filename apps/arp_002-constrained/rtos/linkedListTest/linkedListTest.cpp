/* Includes -- App -----------------------------------------------------------*/
#include <stdio.h>

/* Includes -- Test ----------------------------------------------------------*/
#include "linkedListTest.h"

/* References ----------------------------------------------------------------*/
#define SAFETY_LIMIT 10000 //Max index in case of unterminated loops

char debugStr[] = "l list";

//static uint32_t subpulses = 0;
///* Functions -----------------------------------------------------------------*/
void LinkedListTest::printInfo(void)
{
    printf("[%-10s] Info on XXXX @ %p\n", debugStr, (void*)this);
    printf("[%-10s]   Current pos: %d, Max len: %d, Data sz: %d\n", debugStr, listLength(), maxLength, dataSize);
    listErrorStats_t * pErr = getErrorStats();
    printf("[%-10s]   Errors, full: %d, read: %d, write: %d\n", debugStr, pErr->listFull, pErr->badReadIndex, pErr->badWriteIndex);

}

void LinkedListTest::printBookmarks(void)
{
    printf("--- bookmarks ---\n");
    printf("Len: %d, ", listLength());
    for(int i = 0; i < BOOKMARKS_MAX; i++)
    {
        printf("[% 3d]", bookmarks[i]);
    }
    printf("\n");
    
}
