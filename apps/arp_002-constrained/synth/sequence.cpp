#include <string.h>
#include <stdint.h>
#include <cstdio>

#include "midi47fx.h"
#include "sequence.h"
#include "bsp.h"

// Connect directly to bsp.
#define localPrintf bspPrintf

// Enable for file debugging
//#define PRINT_DEBUG

///* Functions -----------------------------------------------------------------*/
NoteList::NoteList(listIndex_t maxLengthIn, uint16_t dataSizeIn) : LinkedList(maxLengthIn, dataSizeIn)
{
    ((seqNote_t*)&nullObject)->pitch = 33;
    ((seqNote_t*)&nullObject)->velocity = 0;
}

//seqNote_t * NoteList::getNoteAfterTime( float time )
//{
//    //Walk the list to find the index of the note first exceeding time.
//    //If none are found, great.  We're at the end of the list.
//    listItemContainer_t * iObject = startObjectPtr;
//    listIndex_t iCount;
//    for(iCount = 0; iCount < currentPosition; iCount++)
//    {
//        seqNote_t * testNote = (seqNote_t *)&iObject->data;
//        if(testNote->time > objectToPush->time)
//        {
//            //localPrintf("Found\n");
//            break;
//        }
//        iObject = iObject->next;
//    }
//    //iCount is now pointing to the end of list, or where the note should be inserted.
//    //localPrintf("Insert at %d\n", iCount);
//    insert(objectToPush, iCount);
//}

void NoteList::insertByTime( seqNote_t * pNote )
{
    //Walk the list to find the index of the note first exceeding time.
    //If none are found, great.  We're at the end of the list.
    listItemContainer_t * iObject = startObjectPtr;
    listIndex_t iCount;
    for(iCount = 0; iCount < currentPosition; iCount++)
    {
        seqNote_t * testNote = (seqNote_t *)&iObject->data;
        if(testNote->time > pNote->time)
        {
#ifdef PRINT_DEBUG
            localPrintf("Found place\n");
#endif
            break;
        }
        iObject = iObject->next;
    }
    //iCount is now pointing to the end of list, or where the note should be inserted.
#ifdef PRINT_DEBUG
    localPrintf("Insert at %d\n", iCount);
#endif
    insert(pNote, iCount);
}

void NoteList::markToTime(bookmark_t mark, float time)
{
    //Walk the list to find the index of the note first exceeding time.
    //If none are found, great.  We're at the end of the list.
    listItemContainer_t * iObject = startObjectPtr;
    listIndex_t iCount;
    for(iCount = 0; iCount < currentPosition; iCount++)
    {
        seqNote_t * testNote = (seqNote_t *)&iObject->data;
        if(testNote->time >= time)
        {
#ifdef PRINT_DEBUG
            localPrintf("Found mark\n");
#endif
            break;
        }
        iObject = iObject->next;
    }
    setBookmark(mark, iCount);
}

void NoteList::printList(void)
{
    listItemContainer_t * tempCont;
    //our custom type
    seqNote_t * pData;
    tempCont = startObjectPtr;
    pData = (seqNote_t *)tempCont->data;
    localPrintf("\n%s, %s, %s, %s, %s, %s, %s\n",
                "pos",
                "time",
                "length",
                "pitch",
                "velo",
                "adr",
                "adrNext");
    localPrintf(  "-------------------------\n");
    //Iterate to the depth
    for ( uint8_t i = 0; i < currentPosition; i++ )
    {
#if 0
        //Print as floats
        localPrintf("%d, %f, %f, %d, %d,%p, %p",
                    i,
                    pData->time,
                    pData->length,
                    pData->pitch,
                    pData->velocity,
                    (void*)tempCont,
                    (void*)tempCont->next);
#else
        //Print as ints
        localPrintf("%d, %d, %d, %d, %d,%p, %p",
                    i,
                    (uint32_t)pData->time,
                    (uint32_t)pData->length,
                    pData->pitch,
                    pData->velocity,
                    (void*)tempCont,
                    (void*)tempCont->next);
#endif
        //move index
        tempCont = tempCont->next;
        pData = (seqNote_t *)tempCont->data;
        //Print bookmarks
        for(int j = 0; j < BOOKMARKS_MAX; j++)
        {
            if(i == bookmarks[j])
            {
                localPrintf("[%d]",j);
            }
        }
        localPrintf("\n");
  }
  localPrintf("\ncurrentPosition: %d\n", currentPosition);
  
}

void seqcpy(NoteList *dst, NoteList *src)
{
    dst->clear();
    listIndex_t len = src->listLength();
    for(int i = 0; i < len; i++)
    {
        dst->push(src->readObject(i));
    }
    for(bookmark_t mark = 0; mark < BOOKMARKS_MAX; mark++)
    {
        dst->setBookmark(mark, src->getBookmark(mark));
    }
}
