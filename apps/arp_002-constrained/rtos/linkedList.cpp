/* Includes -- STD -----------------------------------------------------------*/
#include <stdint.h>
//#include <stdbool.h>
//#include <stdarg.h>
#include <string.h>
//#include <stdio.h>

/* Includes -- BSP -----------------------------------------------------------*/
#include "bsp.h"

/* Includes -- modules -------------------------------------------------------*/
#include "linkedList.h"
#include "pool.h"

/* References ----------------------------------------------------------------*/
// Enable for file debugging
//#define PRINT_DEBUG

// Connect directly to bsp.
#define localPrintf bspPrintf

MemCtl mem;

//**********************************************************************//
//  LinkedList Class
//
//  Creates link list
//
//  Construct with passed max depth or use default
//
LinkedList::LinkedList(listIndex_t maxLengthIn, uint16_t dataSizeIn)
{
    startObjectPtr = &nullObject; //start pointed to null note
    currentPosition = 0 ;
    maxLength = maxLengthIn;
    dataSize = dataSizeIn;
    containedSize = sizeof(listItemContainer_t *) + dataSizeIn;
    nullObject.next = &nullObject; //Points to self.
    memset(nullObject.data, 0, dataSizeIn);
    memset(&errorStats, 0, sizeof(errorStats));
    memset(&bookmarks, 0, sizeof(bookmarks));
}

//Pass listItem_t to be pushed to the stack (top)
void LinkedList::push( listItem_t * objectToPush )
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
#ifdef PRINT_DEBUG
        localPrintf("\nMADE A NEW LIST ITEM\n");
#endif

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
        
        newObject->next = &nullObject;
    }
    else
    {
        errorStats.listFull++;
    }

}

//Pass position, returns void
void LinkedList::drop( listIndex_t positionToDrop )
{
    //pointer for object to drop
    listItemContainer_t * objectToDrop;
    listItemContainer_t * tempObjectPtr;

    if ( positionToDrop >= currentPosition )
    {
        //Too deep!    Do nothing.
        errorStats.badWriteIndex++;
    }
    else
    {
        //Ok, good to do it.
        //Go to the object before positionToDrop
        if ( positionToDrop == 0 )
        {
            //Save the start object
            objectToDrop = startObjectPtr;
            //Move startObjectPtr
            startObjectPtr = startObjectPtr->next;
            //Reduce all bookmarks
            for(int i = 0; i < BOOKMARKS_MAX; i++)
            {
                if(bookmarks[i] > 0) bookmarks[i]--;
            }
        }
        else
        {
            //go to the position before the one to drop
            tempObjectPtr = startObjectPtr;
            for (uint8_t i = 0; i < (positionToDrop - 1); i++ )
            {
                    tempObjectPtr = tempObjectPtr->next;
            }
            //Point out the drop target
            objectToDrop = tempObjectPtr->next;
            //move the older object to skip
            tempObjectPtr->next = objectToDrop->next;
            //Reduce some bookmarks
            for(int i = 0; i < BOOKMARKS_MAX; i++)
            {
                if(positionToDrop < bookmarks[i]) bookmarks[i]--;
            }
        }
        //Drop it like it's not
        _mem._free(objectToDrop);
#ifdef PRINT_DEBUG
        localPrintf("\nDROPPED AN ITEM\n");
#endif
        currentPosition--;
    }

}

//Pass listItem_t and position
void LinkedList::insert( listItem_t * objectToInsert, listIndex_t positionToBe )
{
    //pointer for object to drop
    listItemContainer_t * tempObjectPtr;
    
    if(( currentPosition < maxLength ) && ( positionToBe <= currentPosition ))
    {
        listItemContainer_t *newObject = (listItemContainer_t *)_mem._malloc(containedSize);
        //If position is 0, do it a different way
        if( positionToBe == 0 )
        {
#ifdef PRINT_DEBUG
            localPrintf("\nINSERTING AN OBJECT AT 0, MADE A NEW LIST ITEM\n");
#endif
            //Point the new object at the previous target
            newObject->next = startObjectPtr;
            //Point the one above to the new one
            startObjectPtr = newObject;
            //Apply the data
            memcpy(newObject->data, objectToInsert, dataSize);
            //grow list size
            currentPosition++;
            //Increase bookmarks past 0
            for(int i = 0; i < BOOKMARKS_MAX; i++)
            {
                if(bookmarks[i] != 0) bookmarks[i]++;
            }
        }
        else
        {
#ifdef PRINT_DEBUG
            localPrintf("\nINSERTING AN OBJECT, MADE A NEW LIST ITEM\n");
#endif
            //Go to the position above, save pointer
            tempObjectPtr = startObjectPtr;
            for (uint8_t i = 0; i < ( (int32_t)positionToBe - 1 ); i++ )
            {
                tempObjectPtr = tempObjectPtr->next;
            }
            //Point the new object at the previous target
            newObject->next = tempObjectPtr->next;
            //Point the one above to the new one
            tempObjectPtr->next = newObject;
            //Apply the data
            memcpy(newObject->data, objectToInsert, dataSize);
            //grow list size
            currentPosition++;
            //Increase bookmarks past 0
            for(int i = 0; i < BOOKMARKS_MAX; i++)
            {
                if(positionToBe < bookmarks[i]) bookmarks[i]++;
            }
        }
    }
    else
    {
        errorStats.badWriteIndex++;
    }
}

//returns listItemContainer_t
//Not protected against out of range reads
listItemContainer_t * LinkedList::readContainer(listIndex_t inputNumber) //position 0 = top
{
    listItemContainer_t * objectToReturn;

    objectToReturn = startObjectPtr;
    //Iterate to the depth
    for ( uint8_t i = 0; i < inputNumber; i++ )
    {
        objectToReturn = objectToReturn->next;
    }

    return objectToReturn;
}

//returns listItem_t
listItem_t * LinkedList::readObject(listIndex_t inputNumber) //position 0 = top
{
    if (inputNumber < maxLength) //Prevent out of range peeks
    {
        listItemContainer_t * container;
        container = readContainer(inputNumber);
        return container->data;
    }
    else
    {
        errorStats.badReadIndex++;
        return nullObject.data;
    }
}

//Bookmarks
void LinkedList::setBookmark(bookmark_t mark, uint32_t value)
{
    bookmarks[mark] = value;
}

uint32_t LinkedList::getBookmark(bookmark_t mark)
{
    return bookmarks[mark];
}

listItem_t * LinkedList::readBookmarked(bookmark_t mark)
{
    return readObject(bookmarks[mark]);
}

void LinkedList::markForward(bookmark_t mark)
{
    if(bookmarks[mark] < currentPosition)
    {
        bookmarks[mark] += 1;
    }
}

void LinkedList::markBackward(bookmark_t mark)
{
    if(bookmarks[mark] > 0)
    {
        bookmarks[mark] -= 1;
    }
}

bool LinkedList::markIsValid(bookmark_t mark)
{
    listIndex_t index = bookmarks[mark];
    if((index >= 0)&&(index < currentPosition))
    {
        return true;
    }
    return false;
}

//Clear out the list, point afresh
void LinkedList::clear( void )
{
    while( currentPosition > 0 )
    {
        drop( 0 );
    }
    memset(&bookmarks, 0, sizeof(bookmarks));
}

//returns depth of stack.
listIndex_t LinkedList::listLength( void )
{
    return currentPosition;
}

listErrorStats_t * LinkedList::getErrorStats(void)
{
    return &errorStats;
}

void LinkedList::clearErrorStats(void)
{
    memset(&errorStats, 0, sizeof(errorStats));
}

void LinkedList::setMalloc(AllocInterface * itf)
{
    Pool * pPool = (Pool*)itf;
    if(containedSize > pPool->dataWidth)
    {
#ifdef PRINT_DEBUG
        localPrintf("*** ERROR ***: Container size greater than pool data width\n");
        localPrintf(" (in bytes) container: %d, pool: %d\n", containedSize, pPool->dataWidth);
#endif
    }
    _mem.setPool(itf);
}
