#include <string.h>
#include <stdint.h>
#include <cstdio>

#include "sequence.h"
#include "notesPlayingList.h"
#include "bsp.h"

// Connect directly to bsp.
#define localPrintf bspPrintf

///* Functions -----------------------------------------------------------------*/
NotesPlayingList::NotesPlayingList(listIndex_t maxLengthIn, uint16_t dataSizeIn) : LinkedList(maxLengthIn, dataSizeIn)
{
    ((seqNote_t*)&nullObject)->pitch = 33;
    ((seqNote_t*)&nullObject)->velocity = 0;
}

//Pass listItem_t to be pushed to the stack (top)
void NotesPlayingList::pushModLen( listItem_t * objectToPush, uint16_t lenInClocks )
{
    //This creates a new object, but then forgets the name
    //and doesn't delete the object.
    //
    //If the stack size is too big, new object won't be accepted
    //
    //This is a new 'book', so no bookmarks will be moved
    if ( currentPosition < maxLength )
    {
        //Make a new object to the list
        listItemContainer_t *newObject = (listItemContainer_t *)_mem._malloc(containedSize);
        //Point the start to the new note if this is the first one
        if( currentPosition == 0 )
        {
            startObjectPtr = newObject;
        }
        else
        {
            //Otherwise set the previous note to this new address
            listItemContainer_t * previousObjectPtr = readContainer( currentPosition - 1 );
            previousObjectPtr->next = newObject;
        }
        
        //grow list size
        currentPosition++;
        
        //****CUSTOM OBJECT TYPE CODE****//
        memcpy(newObject->data, objectToPush, dataSize);
        seqNote_t * pNote = (seqNote_t*)newObject->data;
        pNote->length = lenInClocks;
        
        newObject->next = &nullObject;
    }
    else
    {
        errorStats.listFull++;
    }

}

void NotesPlayingList::printList(void)
{
    listItemContainer_t * tempCont;
    //our custom type
    seqNote_t * pData;
    tempCont = startObjectPtr;
    pData = (seqNote_t *)tempCont->data;
    localPrintf("\n%s, %s, %s, %s, %s, %s, %s\n",
                "pos",
                "time",
                "len",
                "pitch",
                "velo",
                "adr",
                "adrNext");
    localPrintf(  "-------------------------\n");
    //Iterate to the depth
    for ( uint8_t i = 0; i < currentPosition; i++ )
    {
        localPrintf("%d, %f, %f, %d, %d,%p, %p",
                    i,
                    pData->time,
                    pData->length,
                    pData->pitch,
                    pData->velocity,
                    (void*)tempCont,
                    (void*)tempCont->next);
        
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

void NotesPlayingList::decrementTimes(uint16_t pulseDelta)
{
    //For all notes in list, subtract delta pulses
    listItemContainer_t * objectToDec;
    objectToDec = startObjectPtr;
    for ( uint8_t i = 0; i < currentPosition; i++ )
    {
        if(objectToDec != &nullObject)
        {
            //This is a valid object
            seqNote_t * pData;
            pData = (seqNote_t *)objectToDec->data;
            pData->length -= pulseDelta;
            //Move to the next object
            objectToDec = objectToDec->next;
            
        }
    }
}

/*
Args
    pNote -- if a note is dropped, it is first copied to this passed reference
returns:
    bool -- true if a note was dropped
*/
bool NotesPlayingList::dropExpiredNote(seqNote_t * pNote)
{
    bool objectFound = false;
    listItemContainer_t * pContainer;
    //Point to beginning of list
    pContainer = startObjectPtr;
    //Iterate through list until an expired item is found
    for ( uint8_t i = 0; (i < currentPosition)&&(!objectFound); i++ )
    {
        if(pContainer != &nullObject)
        {
            //This is a valid object
            seqNote_t * pData;
            pData = (seqNote_t *)pContainer->data;
            if(pData->length <= 0)
            {
                //This note has expired
                memcpy(pNote, pData, sizeof(seqNote_t));
                drop(i);
                objectFound = true;
            }
            //Move to the next object
            pContainer = pContainer->next;
            
        }
    }
    return objectFound;
}