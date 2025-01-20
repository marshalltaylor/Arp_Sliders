#ifndef LINKEDLISTTEST_H
#define LINKEDLISTTEST_H

/* Includes -- STD -----------------------------------------------------------*/

/* Includes -- App -----------------------------------------------------------*/
#include "exampleList.h"

class LinkedListTest : public ExampleList
{
public:
    using ExampleList::ExampleList;
    void printInfo(void);
    void printBookmarks(void);
};

#endif
