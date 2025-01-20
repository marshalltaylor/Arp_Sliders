#include <iostream>
#include "sequence.h"
#include "noteOnList.h"
#include "noteOnListTest.h"
#include "bsp.h"

#include "poolTest.h"

PoolTest linkedListMem(16, 40);

NoteOnList noteOnList(20, sizeof(seqNote_t));

/* Functions -----------------------------------------------------------------*/
int main ()
{
    seqNote_t noteVar; //Used as IO to the linked list
    noteVar.time = 0;
    
    linkedListMem.printInfo();
    
    printf("Create a linked list of note objects\n");
    printf("  size calculated as %d bytes\n", (int)sizeof(seqNote_t));
    
    noteOnList.setMalloc(&linkedListMem);
    noteOnList.printList();

    printf("Add some notes\n");
    noteVar.pitch = 20;
    noteVar.velocity = 100;
    noteVar.length = 24;
    noteVar.accent = 0;
    
    noteOnList.push(&noteVar);
    noteVar.pitch = 32;
    noteVar.length = 8;
    noteOnList.push(&noteVar);
    noteVar.pitch = 44;
    noteVar.length = 8;
    noteOnList.push(&noteVar);
    
    noteOnList.printList();
    noteOnList.decrementTimes(10);
    noteOnList.printList();
    
    while(noteOnList.dropExpiredNote(&noteVar))
    {
        //Would send the note off message here, before memory is lost.
        printf("A note has expired\n");
    }
    noteOnList.printList();
    
    //printf("Clear note list\n");
    //noteList.clear();
    //noteList.printList();
    //
    //fillListWithTestData(&noteList, testData001);
    //noteList.printList();
    //
    //printf("Get a note by exact time\n");
    //printNoteInfo(&noteVar);
    //printf("Try to get a note by time that doesn't exist\n");
    //
    //printf("Get a note by closest after\n");
    //
    //printf("Add note by time, within test range\n");
    //
    //noteVar.time = 40;
    //noteVar.length = 24;
    //noteVar.pitch = 100;
    //noteVar.velocity = 100;
    //noteVar.accent = 0;
    //noteList.insertByTime(&noteVar);
    //noteList.printList();
    //
    //printf("Add note by time, after last existing\n");
    //
    //noteVar.time = 101;
    //noteVar.length = 24;
    //noteVar.pitch = 100;
    //noteVar.velocity = 100;
    //noteVar.accent = 0;
    //noteList.insertByTime(&noteVar);
    //noteList.printList();
    //
    //printf("Example 4 bar data\n");
    //noteList.clear();
    //fillListWithTestData(&noteList, real4bar);
    //noteList.printList();
}