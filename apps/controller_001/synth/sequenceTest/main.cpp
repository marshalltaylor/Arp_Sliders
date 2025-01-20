#include <iostream>
#include "sequence.h"
#include "sequenceTest.h"
#include "midi47fx.h"
#include "bsp.h"

#include "poolTest.h"

#define POOL_SIZE 40
#define MAX_DATA_SIZE 20
PoolTest linkedListMem(MAX_DATA_SIZE, POOL_SIZE);

MidiTestFixture MIDI;

NoteList noteList(20, sizeof(seqNote_t));

/* Functions -----------------------------------------------------------------*/
int main ()
{
    seqNote_t noteVar; //Used as IO to the linked list
    noteVar.time = 0;
    
    linkedListMem.printInfo();
    
    printf("Create a linked list of note objects\n");
    printf("  size calculated as %d bytes\n", (int)sizeof(seqNote_t));
    
    noteList.setMalloc(&linkedListMem);
    noteList.printList();

    printf("Add some notes\n");
    noteVar.pitch = 20;
    noteVar.velocity = 100;
    noteVar.length = 24;
    noteVar.accent = 0;
    
    noteList.push(&noteVar);noteList.printList();
    noteVar.pitch = 32;
    noteList.push(&noteVar);noteList.printList();
    noteVar.pitch = 44;
    noteList.push(&noteVar);
    
    noteList.printList();
    linkedListMem.printInfo();
    
    printf("Clear note list\n");
    noteList.clear();
    noteList.printList();
    
    fillListWithTestData(&noteList, testData001);
    noteList.printList();

    printf("Get a note by exact time\n");
    printNoteInfo(&noteVar);
    printf("Try to get a note by time that doesn't exist\n");
    
    printf("Get a note by closest after\n");
    
    printf("Add note by time, within test range\n");
    
    noteVar.time = 40;
    noteVar.length = 24;
    noteVar.pitch = 100;
    noteVar.velocity = 100;
    noteVar.accent = 0;
    noteList.insertByTime(&noteVar);
    noteList.printList();
    
    printf("Add note by time, after last existing\n");
    
    noteVar.time = 101;
    noteVar.length = 24;
    noteVar.pitch = 100;
    noteVar.velocity = 100;
    noteVar.accent = 0;
    noteList.insertByTime(&noteVar);
    noteList.printList();
    
    printf("Example 4 bar data\n");
    noteList.clear();
    fillListWithTestData(&noteList, real4bar);
    noteList.printList();
}