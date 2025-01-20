#include <stdint.h>
#include "bsp.h"

#include "exampleList.h"

// Connect directly to bsp.
#define localPrintf bspPrintf

///* Functions -----------------------------------------------------------------*/
ExampleList::ExampleList(listIndex_t maxLengthIn, uint16_t dataSizeIn) : LinkedList(maxLengthIn, dataSizeIn)
{
    ((exampleListItem_t*)&nullObject)->number = 5;
}

void ExampleList::printList(void)
{
    listItemContainer_t * tempCont;
    //our custom type
    exampleListItem_t * pData;
    tempCont = startObjectPtr;
    pData = (exampleListItem_t *)tempCont->data;
    localPrintf("\n Pos, number, address, nextObjectAddress \n");
    localPrintf(  "-------------------------\n");
    //Iterate to the depth
    for ( uint8_t i = 0; i < currentPosition; i++ )
    {
        localPrintf("%d, %d, 0x%p, 0x%p",
                    i,
                    pData->number,
                    (void*)tempCont,
                    (void*)tempCont->next);
        //move index
        tempCont = tempCont->next;
        pData = (exampleListItem_t *)tempCont->data;
        for(int j = 0; j < BOOKMARKS_MAX; j++)
        {
            if(i == bookmarks[j])
            {
                printf("[%d]",j);
            }
        }
        printf("\n");
  }
  localPrintf("\ncurrentPosition: %d\n", currentPosition);
  
}
