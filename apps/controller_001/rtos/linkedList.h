#ifndef LINKEDLIST_H
#define LINKEDLIST_H

/*
List allocates through memoryController for pool or stdlib operation

List has bookmarks that can move when 'books' are inserted in a list

To use, decorate with your own types, see exampleList.h

*/

#include "memoryController.h"

#define BOOKMARKS_MAX 2

struct listErrorStats_t
{
    uint16_t listFull;
    uint16_t badReadIndex;
    uint16_t badWriteIndex;
};

// Goals:
// use only void * to interface list
typedef void listItem_t;
typedef uint8_t bookmark_t;

struct listItemContainer_t
{
    //Pointer to next object
    listItemContainer_t * next;
    //Data
    uint8_t data[1]; //Actually flexible array
};

//Depth into the stack type
typedef int16_t listIndex_t;

class LinkedList
{
public:
    LinkedList(listIndex_t maxLengthIn, uint16_t dataSizeIn); //Construct with passed max depth

// User Interface
    //Item manipulation
    void push(listItem_t *); //Pass object pointer
    void drop(listIndex_t); //Pass position, returns object
    void insert(listItem_t * , listIndex_t); //Pass mmqObject_t and position
    void clear(void);
    listIndex_t listLength(void); //returns depth of stack.
    listItem_t * readObject(listIndex_t);
    //Bookmarks
    void setBookmark(bookmark_t, uint32_t);
    uint32_t getBookmark(bookmark_t);
    listItem_t * readBookmarked(bookmark_t);
    void markForward(bookmark_t);
    void markBackward(bookmark_t);
    bool markIsValid(bookmark_t);
    void setMalloc(AllocInterface * p);
    listErrorStats_t * getErrorStats(void);
    void clearErrorStats(void);
// Should be private, for child use
    //List query tools
    listItemContainer_t * readContainer( listIndex_t );
    
    listIndex_t maxLength;
    listIndex_t currentPosition;
    uint16_t dataSize;
    uint16_t containedSize;
    //A empty note to point to when the list is empty
    listItemContainer_t nullObject;
    
    //A pointer to the next object, list items enumerate from this
    listItemContainer_t * startObjectPtr;
    
    listErrorStats_t errorStats;
protected:
    listIndex_t bookmarks[BOOKMARKS_MAX];
    MemCtl _mem;
private:
};

#endif
