#ifndef EXAMPLELIST_H
#define EXAMPLELIST_H

#include "linkedList.h"


struct exampleListItem_t
{
    // Data fields
    char name[20];
    int number;
};

class ExampleList : public LinkedList
{
public:
    ExampleList(listIndex_t maxLengthIn, uint16_t dataSizeIn);
    void printList(void);
};

#endif
