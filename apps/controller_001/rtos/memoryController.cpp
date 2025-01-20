/* Includes -- App -----------------------------------------------------------*/
#include <stdio.h>
#include <stdlib.h>

/* Includes -- Test ----------------------------------------------------------*/
#include "memoryController.h"

/* References ----------------------------------------------------------------*/
#ifndef UNUSED
#define UNUSED(x) (void)x
#endif

#define localPrintf printf

DefaultMalloc builtInMalloc;
/* Functions -----------------------------------------------------------------*/



MemCtl::MemCtl(void)
{
    pAlObj = &builtInMalloc;
}

void MemCtl::setPool(AllocInterface *pInput)
{
    pAlObj = pInput;
}

void *MemCtl::_malloc(int size)
{
    UNUSED(size);
    if(pAlObj != NULL)
    {
        return pAlObj->_malloc(size);
    }
    else
    {
        //This is pretty fundemental, so hard break here
        while(true);
    }
}

void MemCtl::_free(void *pInput)
{
    if(pAlObj != NULL)
    {
        pAlObj->_free(pInput);
    }
    else
    {
        //This is pretty fundemental, so hard break here
        while(true);
    }
}
